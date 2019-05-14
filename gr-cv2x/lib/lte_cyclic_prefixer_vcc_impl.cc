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
#include "lte_cyclic_prefixer_vcc_impl.h"

namespace gr {
  namespace cv2x {

    lte_cyclic_prefixer_vcc::sptr
    lte_cyclic_prefixer_vcc::make(int fftl)
    {
      return gnuradio::get_initial_sptr
        (new lte_cyclic_prefixer_vcc_impl(fftl));
    }

    /*
     * The private constructor
     */
    lte_cyclic_prefixer_vcc_impl::lte_cyclic_prefixer_vcc_impl(int fftl)
      : gr::block("lte_cyclic_prefixer_vcc",
              gr::io_signature::make(1, 1, fftl*sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
        d_fftl(fftl),
        d_cpl(144*fftl/2048),
        d_cpl0(160*fftl/2048),
        d_pos(0)
    {
      set_output_multiple(d_fftl + d_cpl0);
      set_relative_rate(d_cpl0 + d_fftl);
    }

    /*
     * Our virtual destructor.
     */
    lte_cyclic_prefixer_vcc_impl::~lte_cyclic_prefixer_vcc_impl()
    {
    }

    void
    lte_cyclic_prefixer_vcc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = int(noutput_items/(d_fftl + d_cpl0));
    }

    int
    lte_cyclic_prefixer_vcc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
      //The minimum between these values is the number of nin to consume
      int nin = ninput_items[0];
      int max_input = int(noutput_items/(d_fftl + d_cpl0));
      int consumed_items = nin > max_input? max_input : nin;
      int nout = 0;

      for(int i = 0; i < consumed_items; i++){
        if(d_pos == 0){
          //If it is the fist symbol of slot
          memcpy(out + d_cpl0, in, d_fftl * sizeof(gr_complex));
          memcpy(out, in + d_fftl - d_cpl0, d_cpl0 * sizeof(gr_complex));
          in += d_fftl;
          out += d_fftl + d_cpl0;
          nout += d_fftl + d_cpl0;
        }else{
          // The rest of the symbol
          memcpy(out + d_cpl, in, d_fftl * sizeof(gr_complex));
          memcpy(out, in + d_fftl - d_cpl, d_cpl * sizeof(gr_complex));
          in += d_fftl;
          out += d_fftl + d_cpl;
          nout += d_fftl + d_cpl;
        }
        // Update counter of symbols
        d_pos = d_pos == 6? 0 : d_pos + 1;
      }

      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (consumed_items);

      // Tell runtime system how many output items we produced.
      return nout;
    }

  } /* namespace cv2x */
} /* namespace gr */
