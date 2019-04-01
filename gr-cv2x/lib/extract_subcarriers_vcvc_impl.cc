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
#include "extract_subcarriers_vcvc_impl.h"

namespace gr {
  namespace cv2x {

    extract_subcarriers_vcvc::sptr
    extract_subcarriers_vcvc::make(int N_rb, int fftl)
    {
      return gnuradio::get_initial_sptr
        (new extract_subcarriers_vcvc_impl(N_rb, fftl));
    }

    /*
     * The private constructor
     */
    extract_subcarriers_vcvc_impl::extract_subcarriers_vcvc_impl(int N_rb, int fftl)
      : gr::sync_block("extract_subcarriers_vcvc",
            gr::io_signature::make( 1, 1, sizeof(gr_complex) * fftl),
            gr::io_signature::make( 1, 1, sizeof(gr_complex) * 12 * N_rb)),
        d_N_rb_dl(N_rb),
        d_fftl(fftl)
    {}

    /*
     * Our virtual destructor.
     */
    extract_subcarriers_vcvc_impl::~extract_subcarriers_vcvc_impl()
    {
    }

    int
    extract_subcarriers_vcvc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      //~ int N_rb_dl = d_N_rb_dl;
      //~ int fftl = d_fftl;
      int h_byte_tones_num = sizeof(gr_complex) * 6 * d_N_rb_dl;

      for (int i = 0; i < noutput_items; i ++){
        memcpy(out, in+(d_fftl-(6*d_N_rb_dl)), h_byte_tones_num );
        //memcpy(out+(6*d_N_rb_dl), in+1, h_byte_tones_num);
        //el sidelink no tiene portadora en dc
        memcpy(out+(6*d_N_rb_dl), in, h_byte_tones_num);


        in  += d_fftl;
        out += 12 * d_N_rb_dl;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace cv2x */
} /* namespace gr */
