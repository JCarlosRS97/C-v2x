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
#include "subframe_to_symbol_vector_impl.h"

namespace gr {
  namespace cv2x {

    subframe_to_symbol_vector::sptr
    subframe_to_symbol_vector::make(int NSLRB, int NFFT)
    {
      return gnuradio::get_initial_sptr
        (new subframe_to_symbol_vector_impl(NSLRB, NFFT));
    }

    /*
     * The private constructor
     */
    subframe_to_symbol_vector_impl::subframe_to_symbol_vector_impl(int NSLRB, int NFFT)
      : gr::sync_interpolator("subframe_to_symbol_vector",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*NSLRB*NRBsc*NSLsymb*2),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*NFFT), 14),
              NSLsc(NSLRB*NRBsc),
              symbol(0),
              NFFT(NFFT)
    {
    }

    /*
     * Our virtual destructor.
     */
    subframe_to_symbol_vector_impl::~subframe_to_symbol_vector_impl()
    {
    }

    int
    subframe_to_symbol_vector_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      //Center the pointer
      out += NFFT/2 - NSLsc/2;
      // Do <+signal processing+>
      int consumed_items = 0;
     //printf("Se piden %i items.\n", noutput_items);
      for(int i = 0; i < noutput_items; i++){
         memcpy(out, in + symbol*NSLsc + consumed_items*NSLsc*NSLsymb*2, sizeof(gr_complex)*NSLsc);
         //printf("Simbolo: %f, %f\n", std::real(out[31]), std::imag(out[31]));
         out += NFFT;
         if(symbol == (NSLsymb*2 - 1)){
           //printf("Nuevo subframe\n");
            symbol = 0;
            consumed_items++;
         }else{
            symbol++;
         }
      }
     //printf("Se han requerido %i inputs\n", consumed_items);
      consume_each(consumed_items);
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace cv2x */
} /* namespace gr */
