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
#include "pss_time_sim_impl.h"
#include <volk/volk.h>
#include <algorithm>

namespace gr {
  namespace cv2x {

    pss_time_sim::sptr
    pss_time_sim::make(int fftl, int syncPeriod, boost::shared_ptr<gr::analog::sig_source_c> &sig, float umbralSim, float umbralCorr)
    {
      return gnuradio::get_initial_sptr
        (new pss_time_sim_impl(fftl, syncPeriod, sig, umbralSim, umbralCorr));
    }

    /*
     * The private constructor
     */
    pss_time_sim_impl::pss_time_sim_impl(int fftl, int syncPeriod, boost::shared_ptr<gr::analog::sig_source_c> &sig, float umbralSim, float umbralCorr)
      : gr::sync_block("pss_time_sim",
      gr::io_signature::make(1, 1, sizeof(gr_complex)*128),
      gr::io_signature::make(0, 0, 0)),
      syncPeriod(syncPeriod),
      d_fftl(fftl),
      d_cpl(144*fftl/2048),
      d_cpl0(160*fftl/2048),
      d_slotl(7*fftl+6*d_cpl+d_cpl0),
      d_N_id_2(-1),
      d_sync_frame_start(0),
      d_corr_val(0.0),
      d_lock_count(0),
      nfft(64),
      d_sig(sig),
      umbralSim(umbralSim),
      umbralCorr(umbralCorr),
      d_is_locked(false)
      {
        d_port_lock = pmt::string_to_symbol("lock");
        message_port_register_out(d_port_lock);
        d_port_sync_frame_start = pmt::string_to_symbol("sync_frame");
        message_port_register_out(d_port_sync_frame_start);
        d_port_N_id_2 = pmt::string_to_symbol("N_id_2");
        message_port_register_out(d_port_N_id_2);

        d_chu_f   = (gr_complex*) fftwf_malloc(sizeof(gr_complex)*nfft);
        d_chu_t    =  (gr_complex*) fftwf_malloc(sizeof(gr_complex)*nfft);
        d_chu0_fm1_t   = (gr_complex*) volk_malloc(sizeof(gr_complex)*nfft, volk_get_alignment());
        d_chu1_fm1_t   = (gr_complex*) volk_malloc(sizeof(gr_complex)*nfft, volk_get_alignment());
        d_chu0_f0_t   = (gr_complex*) volk_malloc(sizeof(gr_complex)*nfft, volk_get_alignment());
        d_chu1_f0_t   = (gr_complex*) volk_malloc(sizeof(gr_complex)*nfft, volk_get_alignment());
        d_chu0_f1_t   = (gr_complex*) volk_malloc(sizeof(gr_complex)*nfft, volk_get_alignment());
        d_chu1_f1_t   = (gr_complex*) volk_malloc(sizeof(gr_complex)*nfft, volk_get_alignment());
        d_half_shift   = (gr_complex*) volk_malloc(sizeof(gr_complex)*nfft, volk_get_alignment());
        d_corr_in   = (gr_complex*) volk_malloc(sizeof(gr_complex)*nfft, volk_get_alignment());
        d_energia   = (gr_complex*) volk_malloc(sizeof(gr_complex)*nfft, volk_get_alignment());
        d_chu_sim_f1_t = (gr_complex*) volk_malloc(sizeof(gr_complex)*(nfft/2-1), volk_get_alignment());
        d_chu_sim_fm1_t = (gr_complex*) volk_malloc(sizeof(gr_complex)*(nfft/2-1), volk_get_alignment());

        d_plan_r = fftwf_plan_dft_1d(nfft, reinterpret_cast<fftwf_complex*>(d_chu_f), reinterpret_cast<fftwf_complex*>(d_chu_t), FFTW_BACKWARD, FFTW_ESTIMATE);

        //zc generation

        generate_time_psss(d_chu0_f0_t, 0, 0);
        generate_time_psss(d_chu1_f0_t, 0, 1);
        generate_time_psss(d_chu0_f1_t, 1, 0);
        generate_time_psss(d_chu1_f1_t, 1, 1);
        generate_time_psss(d_chu0_fm1_t, -1, 0);
        generate_time_psss(d_chu1_fm1_t, -1, 1);
        //shift
        for(int i = 0; i < nfft; i++){
          d_half_shift[i]= std::polar(1.0f, float(M_PI*i/float(nfft)));
        }

        volk_32fc_x2_multiply_32fc(d_chu0_f0_t, d_chu0_f0_t, d_half_shift, nfft);
        volk_32fc_x2_multiply_32fc(d_chu1_f0_t, d_chu1_f0_t, d_half_shift, nfft);
        volk_32fc_x2_multiply_32fc(d_chu0_f1_t, d_chu0_f1_t, d_half_shift, nfft);
        volk_32fc_x2_multiply_32fc(d_chu1_f1_t, d_chu1_f1_t, d_half_shift, nfft);
        volk_32fc_x2_multiply_32fc(d_chu0_fm1_t, d_chu0_fm1_t, d_half_shift, nfft);
        volk_32fc_x2_multiply_32fc(d_chu1_fm1_t, d_chu1_fm1_t, d_half_shift, nfft);

        // printf("Chu0\n");
        // for(int j = 0; j < nfft; j++){
        //   printf("pos %i value %f + %fi\n", j, d_chu0_f0_t[j].real(), d_chu0_f0_t[j].imag());
        // }
        //
        // printf("Chu1\n");
        // for(int j = 0; j < nfft; j++){
        //   printf("pos %i value %f + %fi\n", j, d_chu1_f0_t[j].real(), d_chu1_f0_t[j].imag());
        // }

        //Se calcula el desfase
        int j = 0;
        for(int j = 0; j < nfft/2-1;j++){
          d_chu_sim_f1_t[j] = d_chu0_f1_t[j+1]*conj(d_chu0_f1_t[nfft-(j+1)]);
          d_chu_sim_f1_t[j] = conj(d_chu_sim_f1_t[j])/abs(d_chu_sim_f1_t[j]);
          d_chu_sim_fm1_t[j] = d_chu0_fm1_t[j+1]*conj(d_chu0_fm1_t[nfft-(j+1)]);
          d_chu_sim_fm1_t[j] = conj(d_chu_sim_fm1_t[j])/ abs(d_chu_sim_fm1_t[j]);
          // printf("pos %i value %f + %fi\n", j, d_chu_sim_f1_t[j].real(), d_chu_sim_f1_t[j].imag());
          // printf("pos %i value %f + %fi\n", j, d_chu_sim_fm1_t[j].real(), d_chu_sim_fm1_t[j].imag());
        }


        //ahora se conjugan ambas secuencias
        volk_32fc_conjugate_32fc_a(d_chu0_f0_t, d_chu0_f0_t, nfft);
        volk_32fc_conjugate_32fc_a(d_chu1_f0_t, d_chu1_f0_t, nfft);
        volk_32fc_conjugate_32fc_a(d_chu0_f1_t, d_chu0_f1_t, nfft);
        volk_32fc_conjugate_32fc_a(d_chu1_f1_t, d_chu1_f1_t, nfft);
        volk_32fc_conjugate_32fc_a(d_chu0_fm1_t, d_chu0_fm1_t, nfft);
        volk_32fc_conjugate_32fc_a(d_chu1_fm1_t, d_chu1_fm1_t, nfft);



        //se liberan los recursos
        fftwf_destroy_plan(d_plan_r);
        fftwf_free(d_chu_f);
        fftwf_free(d_chu_t);
      }


      /*
      * Our virtual destructor.
      */
      pss_time_sim_impl::~pss_time_sim_impl()
      {
        volk_free(d_half_shift);
        volk_free(d_energia);
        volk_free(d_corr_in);
        volk_free(d_chu0_f0_t);
        volk_free(d_chu1_f0_t);
        volk_free(d_chu0_f1_t);
        volk_free(d_chu1_f1_t);
        volk_free(d_chu0_fm1_t);
        volk_free(d_chu1_fm1_t);
        volk_free(d_chu_sim_f1_t);
        volk_free(d_chu_sim_fm1_t);
      }

      // Define PI for use in this block
      const float
      pss_time_sim_impl::d_PI = float(M_PI);

      int
      pss_time_sim_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
        {
          // This is a sink block. It does not compute any output.
          const gr_complex *in = (const gr_complex *) input_items[0];

          // variable used in multiple positions
          long nir = nitems_read(0);

          bool changed;
          for(int i = 0 ; i < noutput_items ; i++){
            changed= false;
            //extract PSS from its carriers.
            memcpy(d_energia, in, sizeof(gr_complex)*nfft);
            //memcpy(d_corr_in2, in + nfft, sizeof(gr_complex)*nfft);
            for(int j = 0; j< nfft; j++){
              d_corr_in[j] = in[j] - in[j+nfft];
            }
            in += 2*nfft;
            // tracking does need less cross correlation calculations!
            float metrica;
            int ifo;
            find_sim(metrica, ifo);

            if(metrica > umbralSim){
              if(d_is_locked){ changed = tracking(); }
              else{ changed = find_pss_symbol(ifo); }
              int sync_frame_start = calculate_sync_frame_start(nir + i);
              printf("pos %i\n", sync_frame_start);
              printf("metrica %f\n", metrica);
            }
            //Do things if new max is found!
            if(changed){
              d_lock_count = 0; // reset lock count!
              int sync_frame_start = calculate_sync_frame_start(nir + i);
              (*d_sig).set_frequency((-1)*double(d_offset*15000.0) - 7500.0);
              if(d_sync_frame_start != sync_frame_start ){
                if(!d_is_locked){
                  printf("%s NEW sync_frame_start = %i\tN_id_2 = %i\td_offset = %i\tcorr_val = %f\n\n",name().c_str(), sync_frame_start, d_N_id_2,d_offset, d_corr_val );
                  //~ (*d_tag).set_N_id_2(d_N_id_2); // only set a new Cell ID number if not yet locked!
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
          if( !d_is_locked && d_lock_count > (14.1*syncPeriod*2) && d_N_id_2 >=0 ){
            printf("\n%s is locked! sync_frame_start = %ld\tN_id_2 = %i\tcorr_val = %f\n\n",name().c_str(), d_sync_frame_start, d_N_id_2, d_corr_val );
            printf("IFO= %i\n", d_offset);
            printf("Calculator -> %f\n", pc_work_time_avg());
            d_is_locked = true;
            message_port_pub( d_port_lock, pmt::PMT_T );
          }

          // Tell runtime system how many output items we produced.
          return noutput_items;
        }

        void
        pss_time_sim_impl::zc(gr_complex *zc, int cell_id)
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
        pss_time_sim_impl::max_pos(float &max, gr_complex *x, int len)
        {
          gr_complex res;

          volk_32fc_x2_dot_prod_32fc(&res, x, d_corr_in, len);
          max = abs(res);
        }


        bool
        pss_time_sim_impl::find_pss_symbol(int ifo)
        {
          int len = nfft;
          int N_id_2 = 0;
          float max, maxc;
          gr_complex energia;
          //Primero se calcula la energia para normalizar la métrica
          volk_32fc_x2_conjugate_dot_prod_32fc(&energia, d_corr_in, d_corr_in, len);
          float abs_energia = sqrt(abs(energia));
          if(ifo == -1){
            max_pos(maxc, d_chu0_fm1_t, len);
            max_pos(max, d_chu1_fm1_t, len);
          }else if(ifo == 0){
            max_pos(maxc, d_chu0_f0_t, len);
            max_pos(max, d_chu1_f0_t, len);
          }else{
            max_pos(maxc, d_chu0_f1_t, len);
            max_pos(max, d_chu1_f1_t, len);
          }

          if(max > maxc){
            N_id_2 = 1;
            maxc = max;
          }
          maxc /= abs_energia;
          //Calculate return value
          bool has_changed = false;
          if(d_corr_val < maxc && maxc > umbralCorr){
            has_changed = true;
            d_N_id_2 = N_id_2;
            d_corr_val = maxc;
            d_offset = ifo;
            // printf("IFO= %i\n", d_offset);
          }
          return has_changed;
        }

        bool
        pss_time_sim_impl::tracking()
        {
          int len = nfft;
          float max = 0.0;
          switch(d_N_id_2 + (d_offset + 1)*2){
            case 0: max_pos(max, d_chu0_fm1_t, len); break;
            case 1: max_pos(max, d_chu1_fm1_t, len); break;
            case 2: max_pos(max, d_chu0_f0_t, len); break;
            case 3: max_pos(max, d_chu1_f0_t, len); break;
            case 4: max_pos(max, d_chu0_f1_t, len); break;
            case 5: max_pos(max, d_chu1_f1_t, len); break;
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
        pss_time_sim_impl::set_sync_frame_start()
        {
          pmt::pmt_t msg = pmt::from_long( d_sync_frame_start) ;
          message_port_pub( d_port_sync_frame_start, msg );

          //~ (*d_tag).set_sync_frame_start(d_sync_frame_start);
          //~ (*d_sel).set_sync_frame_start(d_sync_frame_start);
        }

        //convenience method for better readability
        //get tag from stream --> get tag.offset --> do some calculation --> return sync_frame_start
        int
        pss_time_sim_impl::calculate_sync_frame_start(long pos)
        {
          std::vector <gr::tag_t> v_off;
          get_tags_in_range(v_off, 0, pos, pos+1);
          long offset = pmt::to_long(v_off[0].value) - (d_fftl+d_cpl0);
          // printf("abs_pos: %ld\n", pmt::to_long(v_off[0].value));
          return int( offset%(syncPeriod*2*d_slotl) );
        }

        void
        pss_time_sim_impl::generate_time_psss(gr_complex *seq, int freq_offset, int N_id_2){
          gr_complex d_chu[62];
          zc(d_chu, N_id_2);
          memset(d_chu_f, 0, sizeof(gr_complex)*nfft);
          memcpy(d_chu_f, d_chu + 31 - freq_offset, sizeof(gr_complex)*(31 + freq_offset));
          memcpy(d_chu_f + 33 + freq_offset, d_chu, sizeof(gr_complex)*(31 - freq_offset));
          fftwf_execute(d_plan_r);
          memcpy(seq, d_chu_t, sizeof(gr_complex)*nfft);
        }

        float
        pss_time_sim_impl::find_sim(float &metrica, int &ifo){
          gr_complex res0, res1, resm1, energia;
          res0 = 0;
          res1 = 0;
          resm1 = 0;
          for(int i = 1; i < nfft/2;i++){
            gr_complex val = d_corr_in[i]*conj(d_corr_in[nfft-i]);
            res0 += val;
            res1 += val*d_chu_sim_f1_t[i-1];
            resm1 += val*d_chu_sim_fm1_t[i-1];
            // printf("pos %i value %f + %fi\n", i-1, d_chu_sim_f1_t[i].real(), d_chu_sim_f1_t[i].imag());
            // printf("pos %i value %f + %fi\n", i-1, d_chu_sim_fm1_t[i].real(), d_chu_sim_fm1_t[i].imag());
          }
          ifo = 0;
          float max = abs(res0);
          float peso1 = abs(res1);
          float pesom1 = abs(resm1);
          /*Para debug
          volk_32fc_x2_conjugate_dot_prod_32fc(&energia, d_energia+1, d_energia +1, nfft/2-1);

          printf("IFO = 0 -> %f\n", max/abs(energia));
          printf("IFO = -1 -> %f\n", peso1/abs(energia));
          printf("IFO = 1 -> %f\n", pesom1/abs(energia));
          Para debug*/

          if(max < peso1){
            max = peso1;
            ifo = 1;
          }
          if(max < pesom1){
            max = pesom1;
            ifo = -1;
          }
          // printf("elegido: %i\n", ifo);
          volk_32fc_x2_conjugate_dot_prod_32fc(&energia, d_energia+1, d_energia +1, nfft/2-1);
          if(abs(energia)>0.001){
            metrica = max/abs(energia);
          }else{
            metrica= 0;
          }
        }

        } /* namespace cv2x */
      } /* namespace gr */
