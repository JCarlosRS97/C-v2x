/* -*- c++ -*- */
/*
* Copyright 2019 <+YOU OR YOUR COMPANY+>.
*
* This is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3, or (at your option)
* any later version.
*
* This software is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this software; see the file COPYING.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street,
* Boston, MA 02110-1301, USA.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "psss_time_sync_impl.h"
#include <volk/volk.h>
#include <fftw3.h>



namespace gr {
  namespace cv2x {

    psss_time_sync::sptr
    psss_time_sync::make(int fftl, int syncPeriod)
    {
      return gnuradio::get_initial_sptr
      (new psss_time_sync_impl(fftl, syncPeriod));
    }

    /*
    * The private constructor
    */
    psss_time_sync_impl::psss_time_sync_impl(int fftl, int syncPeriod)
    : gr::sync_decimator("psss_time_sync",
    gr::io_signature::make(1, 1, sizeof(gr_complex)*fftl),
    gr::io_signature::make(0, 0, 0),2),
    syncPeriod(syncPeriod),
    d_fftl(fftl),
    d_cpl(144*fftl/2048),
    d_cpl0(160*fftl/2048),
    d_slotl(7*fftl+6*d_cpl+d_cpl0),
    d_N_id_2(-1),
    d_sync_frame_start(0),
    d_corr_val(0.0),
    d_lock_count(0),
    d_is_locked(false)
    {
      d_port_lock = pmt::string_to_symbol("lock");
      message_port_register_out(d_port_lock);
      d_port_sync_frame_start = pmt::string_to_symbol("sync_frame");
      message_port_register_out(d_port_sync_frame_start);
      d_port_N_id_2 = pmt::string_to_symbol("N_id_2");
      message_port_register_out(d_port_N_id_2);

      gr_complex *d_chu_f   = (gr_complex*) fftwf_malloc(sizeof(gr_complex)*fftl);
      gr_complex *d_chu_t    =  (gr_complex*) fftwf_malloc(sizeof(gr_complex)*fftl);
      d_chu0_t   = (gr_complex*) volk_malloc(sizeof(gr_complex)*fftl*2, volk_get_alignment());
      d_chu1_t   = (gr_complex*) volk_malloc(sizeof(gr_complex)*fftl*2, volk_get_alignment());
      d_corr_in   = (gr_complex*) volk_malloc(sizeof(gr_complex)*fftl*2, volk_get_alignment());

      fftwf_plan d_plan_r = fftwf_plan_dft_1d(fftl, reinterpret_cast<fftwf_complex*>(d_chu_f), reinterpret_cast<fftwf_complex*>(d_chu_t), FFTW_BACKWARD, FFTW_ESTIMATE);

      //zc generation
      gr_complex d_chu0[62];
      gr_complex d_chu1[62];
      zc(d_chu0,0);
      zc(d_chu1,1);
      //Primero el 1
      memset(d_chu_f, 0, sizeof(gr_complex)*fftl);
      memcpy(d_chu_f, d_chu0 + 31, sizeof(gr_complex)*31);
      memcpy(d_chu_f + fftl-31, d_chu0, sizeof(gr_complex)*31);
      fftwf_execute(d_plan_r);
      memcpy(d_chu0_t, d_chu_t, sizeof(gr_complex)*fftl);
      memcpy(d_chu0_t + fftl, d_chu_t, sizeof(gr_complex)*fftl);


      //ahora el segundo
      memset(d_chu_f, 0, sizeof(gr_complex)*fftl);
      memcpy(d_chu_f, d_chu1 + 31, sizeof(gr_complex)*31);
      memcpy(d_chu_f + fftl-31, d_chu1, sizeof(gr_complex)*31);

      fftwf_execute(d_plan_r);
      memcpy(d_chu1_t, d_chu_t, sizeof(gr_complex)*fftl);
      memcpy(d_chu1_t + fftl, d_chu_t, sizeof(gr_complex)*fftl);

      //ahora se conjugan ambas secuencias
      volk_32fc_conjugate_32fc_a(d_chu1_t, d_chu1_t, 2*fftl);
      volk_32fc_conjugate_32fc_a(d_chu0_t, d_chu0_t, 2*fftl);

      // printf("Chu0\n");
      // for(int j = 0; j < d_fftl; j++){
      //   printf("pos %i value %f + %fi\n", j, d_chu0_t[j].real(), d_chu0_t[j].imag());
      // }
      //
      // printf("Chu1\n");
      // for(int j = 0; j < d_fftl; j++){
      //   printf("pos %i value %f + %fi\n", j, d_chu1_t[j].real(), d_chu1_t[j].imag());
      // }


      //se liberan los recursos
      fftwf_destroy_plan(d_plan_r);
      fftwf_free(d_chu_f);
      fftwf_free(d_chu_t);
    }


    /*
    * Our virtual destructor.
    */
    psss_time_sync_impl::~psss_time_sync_impl()
    {
      volk_free(d_corr_in);
      volk_free(d_chu0_t);
      volk_free(d_chu1_t);
    }

    // Define imaginary constant
    const gr_complex
    psss_time_sync_impl::d_C_I = gr_complex(0,1);

    // Define PI for use in this block
    const float
    psss_time_sync_impl::d_PI = float(M_PI);

    int
    psss_time_sync_impl::work(int noutput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items)
      {
        // This is a sink block. It does not compute any output.
        const gr_complex *in = (const gr_complex *) input_items[0];

        // variable used in multiple positions
        long nir = nitems_read(0);

        bool changed = false;
        for(int i = 0 ; i < noutput_items ; i++){
          //extract PSS from its carriers.
          memcpy(d_corr_in, in + 2*i, sizeof(gr_complex)*2*d_fftl);
          // for(int j = 0; j < d_fftl; j++){
          //   printf("pos %i value %f + %fi\n", j, d_corr_in[j].real(), d_corr_in[j].imag());
          // }

          // tracking does need less cross correlation calculations!
          if(d_is_locked){ changed = tracking(); }
          else{ changed = find_pss_symbol(); }

          int sync_frame_start = calculate_sync_frame_start(nir+2*i);
          printf("abs_pos: %i\n", sync_frame_start);
          //Do things if new max is found!
          if(changed){
            d_lock_count = 0; // reset lock count!
            int sync_frame_start = calculate_sync_frame_start(nir+2*i);
            if(d_sync_frame_start != sync_frame_start ){
              if(!d_is_locked){
                printf("\n%s NEW sync_frame_start = %i\tN_id_2 = %i\tcorr_val = %f\n\n",name().c_str(), sync_frame_start, d_N_id_2, d_corr_val );
                //~ (*d_tag).set_N_id_2(d_N_id_2); // only set a new Cell ID number if not yet locked!
                // for(int contador = 0; contador<124; contador++){
                //   printf("pos %i\t value= %f \n", contador, chuX[contador].real());
                // }
                message_port_pub(d_port_N_id_2, pmt::from_long((long)d_N_id_2));
                d_sync_frame_start = sync_frame_start;
              }
              else if( abs(d_sync_frame_start-sync_frame_start) < 2 ){ //only moves by one sample in tracking mode!
                if( d_sync_frame_start < sync_frame_start ){d_sync_frame_start++;}
                else{d_sync_frame_start--;}
              }
              set_sync_frame_start();
            }
          }else{
            d_lock_count++;
          }
        }

        // After a certain amount of unchanged N_id_2 calculations. calculation
        //is stopped and block has no further function.
        //Se utiliza 14.5*syncPeriod ya que deben pasar todos los simbolos de un
        //periodo y se le añade un poco de holgura.
        if( !d_is_locked && d_lock_count > (28*syncPeriod) && d_N_id_2 >=0 ){
          printf("\n%s is locked! sync_frame_start = %ld\tN_id_2 = %i\tcorr_val = %f\n\n",name().c_str(), d_sync_frame_start, d_N_id_2, d_corr_val );
          printf("Calculator duracion: %f\n", pc_work_time_avg 	() 	);
          d_is_locked = true;
          //~ (*d_tag).lock();
          message_port_pub( d_port_lock, pmt::PMT_T );
          //~ (*d_sel).lock();
        }



        // Tell runtime system how many output items we produced.
        return noutput_items;
      }

      void
      psss_time_sync_impl::zc(gr_complex *zc, int cell_id)
      {
        // calculate value of variable u according to cell id number
        float u=0;
        switch (cell_id){
          case 0: u=26.0; break;
          case 1: u=37.0; break;
        }

        // generate zadoff-chu sequences according to original algorithm
        for(int i = 0; i < 31; i++){
          float phase = -M_PI*u*i*(i+1.0)/63.0;
          zc[i] = std::polar(1.0f, phase);
          phase = -M_PI*u*(i + 32.0)*(i + 33.0)/63.0;
          zc[i + 31] = std::polar(1.0f, phase);
        }
      }


      void
      psss_time_sync_impl::mi_max_pos(float &max, gr_complex *x, int len)
      {
        gr_complex res;

        volk_32fc_x2_dot_prod_32fc(&res, x, d_corr_in, len);
        max = abs(res);
      }


      bool
      psss_time_sync_impl::find_pss_symbol()
      {
        int len = 2*d_fftl;
        float max0 = 0.0;
        mi_max_pos(max0, d_chu0_t, len);

        float max1 = 0.0;
        mi_max_pos(max1, d_chu1_t, len);

        int N_id_2 = (max1 > max0)? 1: 0;
        float maxc = (max1 > max0)? max1: max0;
        printf("Chu0: max = %f\n", max0);
        printf("Chu1: max = %f\n", max1);
        //Calculate return value
        bool has_changed = false;
        if(d_corr_val < maxc){
          has_changed = true;
          d_N_id_2 = N_id_2;
          d_corr_val = maxc;
        }
        return has_changed;
      }

      bool
      psss_time_sync_impl::tracking()
      {
        int len = 2*d_fftl;
        float max = 0.0;
        switch(d_N_id_2){
          case 0: mi_max_pos(max, d_chu0_t, len); break;
          case 1: mi_max_pos(max, d_chu1_t, len); break;
        }
        if(d_corr_val < max){
          d_corr_val = max;
          return true;
        }
        else{
          d_corr_val = d_corr_val*0.999;
        }

        return false;
      }

      // convenience function fpr better readability
      void
      psss_time_sync_impl::set_sync_frame_start()
      {
        pmt::pmt_t msg = pmt::from_long( d_sync_frame_start) ;
        message_port_pub( d_port_sync_frame_start, msg );

        //~ (*d_tag).set_sync_frame_start(d_sync_frame_start);
        //~ (*d_sel).set_sync_frame_start(d_sync_frame_start);
      }

      //convenience method for better readability
      //get tag from stream --> get tag.offset --> do some calculation --> return sync_frame_start
      int
      psss_time_sync_impl::calculate_sync_frame_start(long pos)
      {
        std::vector <gr::tag_t> v_off;
        get_tags_in_range(v_off, 0, pos, pos+1);
        long offset = pmt::to_long(v_off[0].value) - (d_fftl+d_cpl0);
        return int( offset%(syncPeriod*2*d_slotl) );
      }



      } /* namespace cv2x */
    } /* namespace gr */
