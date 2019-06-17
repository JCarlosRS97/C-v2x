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
#include "ssss_calculator_vcm_impl.h"

#include <volk/volk.h>

namespace gr {
  namespace cv2x {

    ssss_calculator_vcm::sptr
    ssss_calculator_vcm::make(int fftl, std::string key_id, std::string key_offset, int syncPeriod)
    {
      return gnuradio::get_initial_sptr
      (new ssss_calculator_vcm_impl(fftl, key_id, key_offset, syncPeriod));
    }

    /*
    * The private constructor
    */
    ssss_calculator_vcm_impl::ssss_calculator_vcm_impl(int fftl, std::string key_id, std::string key_offset, int syncPeriod)
    : gr::sync_decimator("ssss_calculator_vcm",
    gr::io_signature::make( 1, 1, sizeof(gr_complex) * 64),
    gr::io_signature::make(0, 0, 0), 2),//Dos simbolos
    d_fftl(fftl),
    syncPeriod(syncPeriod),
    d_slotl(7*fftl+6*(144*fftl/2048)+(160*fftl/2048) ),
    d_cell_id(-1),
    d_max_val_new(0.0),
    d_max_val_old(0.0),
    d_N_id_2(-1),
    d_frame_start(0),
    d_is_locked(false),
    d_unchanged_id(0)
    {
      d_key_id = pmt::string_to_symbol(key_id);
      d_key_offset = pmt::string_to_symbol(key_offset);
      // Get needed message ports!
      d_port_cell_id = pmt::string_to_symbol("cell_id");
      message_port_register_out(d_port_cell_id);
      d_port_frame_start = pmt::string_to_symbol("frame_start");
      message_port_register_out(d_port_frame_start);

      //initialize d_cX
      char cX_x[31] = {0};
      cX_x[4] = 1;
      for(int i = 0; i < 26 ; i++ ){
        cX_x[i+5] = (cX_x[i+3] + cX_x[i])%2;
      }
      //Do NRZ coding (necessary here?)
      for(int i = 0 ; i < 31 ; i++){
        d_cX[i] = 1-2*cX_x[i];
      }

      //initialize d_sref
      int m0_ref=0;
      char sX_x[31]={0};
      sX_x[4]=1;
      for(int i = 0; i < 26 ; i++){
        sX_x[i+5] = (sX_x[i+2] + sX_x[i])%2;
      }
      gr_complex sX[31]={0};
      for(int i = 0 ; i < 31 ; i++){
        sX[i] = 1-2*sX_x[i];
      }
      int m0 = 0;
      gr_complex d_sref[62];
      for(int i = 0 ; i < 31 ; i++){
        d_sref[i   ]=sX[ (i+m0)%31 ];
        d_sref[i+31]=sX[ (i+m0)%31 ];
      }

      seq = (gr_complex*) volk_malloc(sizeof(gr_complex)*31, volk_get_alignment());
      d_s0ref = (gr_complex*) volk_malloc(sizeof(gr_complex)*62, volk_get_alignment());
      memcpy(d_s0ref, d_sref, sizeof(gr_complex)*62);
      // For correlation. Only real values
      // volk_32fc_conjugate_32fc_a(d_s0ref, d_s0ref, 62);



      //initialize d_zX
      char zX_x[31] = {0};
      zX_x[4] = 1;
      for (int i = 0 ; i < 26 ; i++){
        zX_x[i+5] = (zX_x[i+4] + zX_x[i+2] + zX_x[i+1] + zX_x[i+0])%2;
      }
      for (int i = 0; i < 31 ; i++){
        d_zX[i] = 1 - (2*zX_x[i]);
      }

      //initialize m0/m1 pair lookup table
      for(int i = 0 ; i < 168 ; i++){
        int N = i;
        int q_prime = floor(N/30);
        int q = floor( ( N + (q_prime*(q_prime+1)/2) )/30 );
        int m_prime = N + q * (q+1)/2;
        int m0g = m_prime%31;
        int m1g = (m0g + int(floor(m_prime/31))+1 )%31;
        d_v_m0[i] = m0g;
        d_v_m1[i] = m1g;
      }
    }

    /*
    * Our virtual destructor.
    */
    ssss_calculator_vcm_impl::~ssss_calculator_vcm_impl()
    {
    }

    int
    ssss_calculator_vcm_impl::work(int noutput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items)
      {
        // This is a sink block! It does not produce any items thus it doesn't have an output port!
        const gr_complex *in = (const gr_complex *) input_items[0];

        if(d_is_locked){
          return noutput_items;
        }

        gr_complex ssss_symbols[124];
        extract_ssss(ssss_symbols, in);

        std::vector <gr::tag_t> v_id;
        get_tags_in_range(v_id, 0, nitems_read(0), nitems_read(0)+1, d_key_id);
        if (v_id.size() > 0){
          d_N_id_2 = int(pmt::to_long(v_id[0].value));
        }
        if(d_N_id_2 < 0){return 1;}
        // extract the 2 half ssss symbols which are interleaved differently by their position within a frame.
        gr_complex even[31]={0};
        gr_complex odd [31]={0};
        for(int i = 0; i < 31 ; i++){
          even[i] = ssss_symbols[2 * i + 0] + ssss_symbols[2 * i + 0 + 62];
          odd[i]  = ssss_symbols[2 * i + 1] + ssss_symbols[2 * i + 1 + 62];
        }

        int N_id_1 = get_ssss_info(even, odd, d_N_id_2);

        if(N_id_1 >= 0){
          // if(d_max_val_new > d_max_val_old*0.6){
            long offset = 0;
            std::vector <gr::tag_t> v_off;
            get_tags_in_range(v_off,0,nitems_read(0),nitems_read(0)+1,d_key_offset);
            if (v_off.size() > 0){
              offset = pmt::to_long(v_off[0].value);
            }

            //if(d_ssss_pos == 5){offset += (syncPeriod*2*d_slotl);}
            d_frame_start = offset%(syncPeriod*2*d_slotl);

            d_cell_id = N_id_1 + 168*d_N_id_2;

            // printf("actual = %i\n", d_cell_id);

            d_unchanged_id++;
            if(d_unchanged_id >= 1){
              printf("\n%s locked to frame_start = %ld\tabs_pos = %ld\tcell_id = %i\n\n", name().c_str(), d_frame_start, offset, d_cell_id );
              publish_frame_start(d_frame_start);
              publish_cell_id(d_cell_id);
              d_is_locked = true;
            }
          // }

          d_max_val_old = d_max_val_new;
          d_max_val_new = 0;
        }
        // Tell runtime system how many output items we produced.
        return 1;
      }

      int
      ssss_calculator_vcm_impl::get_ssss_info(gr_complex* even, gr_complex* odd, int N_id_2)
      {
        // next 2 sequences depend on N_id_2
        gr_complex c0[31] = {0};
        gr_complex c1[31] = {0};
        for(int i = 0; i < 31 ; i++){
          c0[i] = d_cX[ (i+d_N_id_2  )%31 ];
          c1[i] = d_cX[ (i+d_N_id_2+3)%31 ];
        }

        gr_complex s1m1[31]={0};
        for (int i = 0 ; i < 31 ; i++){
          s1m1[i]=even[i]/gr_complex(c0[i]);
        }
        int m1 = calc_m(s1m1, 31);

        char z1m1[31] = {0};
        for (int i = 0 ; i < 31 ; i++) {
          z1m1[i] = d_zX[ ( i+(m1%8) )%31 ];
        }
        gr_complex s0m0[31] = {0};
        for (int i = 0 ; i < 31 ; i++){
          s0m0[i] = odd[i] / (c1[i] * gr_complex(z1m1[i]) );
          // printf("pos = %i\t value= %f\n", i, s0m0[i].real());
        }

        int m0 = calc_m(s0m0, m1); //m0 is always under m1
        //printf("m1 = %i\n",m1);
        // printf("m0 = %i, m1= %i\n", m0, m1);
        // printf("N_id_1: %i, d_max_val_new= %f\n", get_N_id_1(m0, m1), d_max_val_new);
        return get_N_id_1(m0, m1);
      }

      int
      ssss_calculator_vcm_impl::get_N_id_1(int m0, int m1)
      {
        int N_id_1 = -1;
        for(int i = 0 ; i < 168 ; i++ ){
          if(d_v_m0[i] == m0 && d_v_m1[i] == m1){
            N_id_1 = i;
            break;
          }
        }
        return N_id_1;
      }

      int
      ssss_calculator_vcm_impl::calc_m(gr_complex *s0m0, int max_m)
      {
        int mX = -1;

        memcpy(seq, s0m0, sizeof(gr_complex) * 31);

        std::vector<float> x_vec;
        xcorr(x_vec, seq, d_s0ref, max_m, 31);

        float max = 0;
        int pos = -1;
        for (int i = 0 ; i < max_m ; i++){
          float mag = x_vec[i];
          if (max < mag){
            max = mag;
            pos = i;
          }
        }

        mX = pos;

        d_max_val_new = (d_max_val_new + max)/2;//Esto puede traer problemas
        return mX;
      }

      // simple correlation between 2 arrays. returns complex value.
      float
      ssss_calculator_vcm_impl::corr(gr_complex *x,gr_complex *y, int len)
      {
        gr_complex res;

        volk_32fc_x2_dot_prod_32fc(&res, x, y, len);
        return abs(res);
      }

      // be careful! input arrays must have the same size!
      void
      ssss_calculator_vcm_impl::xcorr(std::vector<float> &v, gr_complex *x,gr_complex *y, int N, int len)
      {

        for (int i = 0 ; i < N ; i++){
            v.push_back( corr(x,y + i,len) );
          // printf("pos = %i\t value= %f\n", i, v[i]);
        }
      }

      void
      ssss_calculator_vcm_impl::publish_cell_id(int cell_id)
      {
        // printf("%s\t\tpublish_cell_id %i\n", name().c_str(), cell_id );
        pmt::pmt_t msg = pmt::from_long((long)cell_id) ;
        message_port_pub( d_port_cell_id, msg );
      }

      void
      ssss_calculator_vcm_impl::publish_frame_start(long frame_start)
      {
        // printf("%s\t\tpublish_frame_start %ld\n", name().c_str(), frame_start );
        pmt::pmt_t msg = pmt::from_long(frame_start) ;
        message_port_pub( d_port_frame_start, msg );
      }

      void
      ssss_calculator_vcm_impl::extract_ssss(gr_complex *ssss_symbols, const gr_complex *in)
      {
        memcpy(ssss_symbols, in+ 33, sizeof(gr_complex)*31);
        memcpy(ssss_symbols + 31, in, sizeof(gr_complex)*31);
        memcpy(ssss_symbols + 62, in + 64 + 33, sizeof(gr_complex)*31);
        memcpy(ssss_symbols + 31 + 62, in + 64, sizeof(gr_complex)*31);
      }

    } /* namespace lte */
  } /* namespace gr */
