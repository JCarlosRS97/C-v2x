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
#include "slss_generator_impl.h"
#include <cmath>
#include <cstdlib>
#include <complex>

namespace gr {
  namespace cv2x {

    slss_generator::sptr
    slss_generator::make(int slssId, int syncOffsetIndicator1, int syncOffsetIndicator2, int syncPeriod)
    {
      return gnuradio::get_initial_sptr
        (new slss_generator_impl(slssId, syncOffsetIndicator1, syncOffsetIndicator2, syncPeriod));
    }

    /*
     * The private constructor
     */
    slss_generator_impl::slss_generator_impl(int slssId, int syncOffsetIndicator1, int syncOffsetIndicator2, int syncPeriod)
      : gr::sync_block("slss_generator",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*TAM_VECTOR)),
        slssId(slssId),
        syncOffsetIndicator1(syncOffsetIndicator1),
        syncOffsetIndicator2(syncOffsetIndicator2),
        syncPeriod(syncPeriod),
        subframeCounter(0)
    {
      create_psss();
      create_ssss();
   }

    /*
     * Our virtual destructor.
     */
    slss_generator_impl::~slss_generator_impl()
    {
    }

    int
    slss_generator_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *out = (gr_complex *) output_items[0];
      memset(out, 0, sizeof(gr_complex) * TAM_VECTOR);
      // Do <+signal processing+>
      for(int i = 0; i < noutput_items; i++) {
         if((subframeCounter % syncPeriod) == syncOffsetIndicator1 ){
            //It's a reference subframe
            CreateSubframe(out);
           //printf("SLSS Gen: nuevo subframe de referencia en %i\n", subframeCounter);
         }
         //Update subframeCounter
         subframeCounter++;
         // if(subframeCounter == syncPeriod){
         //    subframeCounter = 0;
         // }
         //update out pointer
         out += TAM_VECTOR;
      }
     //printf("SLSS Gen: Se han producido %i valores\n", noutput_items);
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

    void slss_generator_impl::create_psss(){
      //6.11.1.1 of TS36.211
      int u = (slssId > 167)? ZC_roots[1] : ZC_roots[0];
      for(int i = 0; i < 31; i++){
         float phase = -M_PI*u*(i+1.0)/63.0;
         psss_symbols[i] = std::polar(amplitude, phase);
         phase = -M_PI*u*(i + 1.0)*(i + 2.0)/63.0;
         psss_symbols[i + 31] = std::polar(amplitude, phase);
      }
   }

   void slss_generator_impl::create_ssss(){
      //TS 36.211 6.11.2.1 with suftrame 5
      int x[31];
      int s[31];
      int c[31];
      int z[31];

      int n_id_1 = slssId % 168;
      int n_id_2 = slssId / 168;
      int q_prime = n_id_1 / 30;
      int q = (n_id_1 + float(q_prime*(q_prime + 1))/2)/30;
      int m_prime = n_id_1 + q*(q + 1)/2.0f;
      int m0 = m_prime % 31;
      int m1 = (m0 + m_prime/31 + 1) % 31;

      //s_m1 sequence
      //x and s sequences of s_m1
      x[0] = 0; x[1] = 0; x[2] = 0; x[3] = 0; x[4] = 1;
      s[0] = 1; s[1] = 1; s[2] = 1; s[3] = 1; s[4] = -1;
      for (int  i = 0; i <= 25; i++){
         x[i + 5] = (x[i + 2] + x[i]) % 2;
         s[i + 5] = 1 - 2*x[i + 5];
      }

      int s_m0[31];
      int s_m1[31];
      for(int i = 0; i < 31; i++){
         s_m0[i] = s[(i + m0) % 31];
         s_m1[i] = s[(i + m1) % 31];
      }

      // c0 y c1 sequences
      //x and c sequences of c0 and c1
      x[0] = 0; x[1] = 0; x[2] = 0; x[3] = 0; x[4] = 1;
      c[0] = 1; c[1] = 1; c[2] = 1; c[3] = 1; c[4] = -1;
      for (int  i = 0; i <= 25; i++){
         x[i + 5] = (x[i + 3] + x[i]) % 2;
         c[i + 5] = 1 - 2*x[i + 5];
      }

      int c0[31];
      int c1[31];
      for(int i = 0; i < 31; i++){
         c0[i] = c[(i + n_id_2) % 31];
         c1[i] = c[(i + n_id_2 + 3) % 31];
      }

      //z1_m1 sequence
      //x and c sequences of s_m1
      x[0] = 0; x[1] = 0; x[2] = 0; x[3] = 0; x[4] = 1;
      z[0] = 1; z[1] = 1; z[2] = 1; z[3] = 1; z[4] = -1;
      for (int  i = 0; i <= 25; i++){
         x[i + 5] = (x[i + 4] + x[i + 2] + x[i + 1] + x[i]) % 2;
         z[i + 5] = 1 - 2*x[i + 5];
      }

      int z1_m1[31];
      for(int i = 0; i < 31; i ++){
         z1_m1[i] = z[(i + (m1 % 8)) % 31];
      }
      //final sequence
      for(int i = 0; i < 31; i++){
         ssss_symbols[2 * i] = s_m1[i] * c0[i] * sqrt(72/62.0);
         ssss_symbols[2 * i  + 1] = s_m0[i] * c1[i] * z1_m1[i] * sqrt(72/62.0);
      }

   }

   void slss_generator_impl::CreateSubframe(gr_complex subframe[NSLRB*NRBsc*NSLsymb*2]){
      //map PSSS
      int frecPosNegative = NSLsc/2 - 31;
      //PSSS
      memcpy(subframe + 1*NSLsc + frecPosNegative, psss_symbols, sizeof(gr_complex)*62);
      memcpy(subframe + 2*NSLsc + frecPosNegative, psss_symbols, sizeof(gr_complex)*62);
      //SSSS
      memcpy(subframe + 11*NSLsc + frecPosNegative, ssss_symbols, sizeof(gr_complex)*62);
      memcpy(subframe + 12*NSLsc + frecPosNegative, ssss_symbols, sizeof(gr_complex)*62);

   }



  } /* namespace cv2x */
} /* namespace gr */
