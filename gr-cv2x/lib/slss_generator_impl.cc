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
              gr::io_signature::make(1, 1, sizeof(gr_complex)*NSLRB*NRBsc*NSLsymb*2)),
        slssId(slssId),
        syncOffsetIndicator1(syncOffsetIndicator1),
        syncOffsetIndicator2(syncOffsetIndicator2),
        syncPeriod(syncPeriod)
    {
      create_pss();
      create_sss();
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

      // Do <+signal processing+>
      for(int i = 0; i < noutput_items; i++) {

            }
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

    void slss_generator_impl::create_pss(){
      //6.11.1.1 of TS36.211
      int u = (slssId > 167)? ZC_roots[1] : ZC_roots[0];
      for(int i = 0; i <= 30; i++){
         psss_symbols[i] = std::polar(1.0f, float(-M_PI*u*(i+1.0)/63.0));
      }
      for(int i = 31; i <= 61; i++){
         psss_symbols[i] = std::polar(1.0f, float(-M_PI*u*(i + 1.0)*(i + 2.0)/63.0));
      }
   }

   void slss_generator_impl::create_sss(){

   }

  } /* namespace cv2x */
} /* namespace gr */
