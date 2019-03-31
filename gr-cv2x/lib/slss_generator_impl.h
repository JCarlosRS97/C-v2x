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

#ifndef INCLUDED_CV2X_SLSS_GENERATOR_IMPL_H
#define INCLUDED_CV2X_SLSS_GENERATOR_IMPL_H

#include <cv2x/slss_generator.h>

namespace gr {
  namespace cv2x {

    class slss_generator_impl : public slss_generator
    {
     private:
       //System values
      static const int NSLsymb = 7;
      static const int Subcarriers = 62;
      int ZC_roots[2];
      //Block configuration
      int slssId;
      int syncOffsetIndicator1;
      int syncOffsetIndicator2;
      int syncPeriod;
      //Internal variables
      gr_complex psss_symbols[Subcarriers]; //dos simbolos iguales y 62 subportadoras
      gr_complex ssss_symbols[Subcarriers]; //dos simbolos iguales y 62 subportadoras
      gr_complex subframe[NSLsymb*2][Subcarriers];
      int subframeCounter;
      int symbolCounter;
      int NFFT;
      float amplitude;

      void create_psss();
      void create_ssss();
      void CreateSubframe();

     public:
      slss_generator_impl(int slssId, int syncOffsetIndicator1, int syncOffsetIndicator2, int syncPeriod, int NFFT);
      ~slss_generator_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_SLSS_GENERATOR_IMPL_H */
