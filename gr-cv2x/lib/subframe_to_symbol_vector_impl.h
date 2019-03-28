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

#ifndef INCLUDED_CV2X_SUBFRAME_TO_SYMBOL_VECTOR_IMPL_H
#define INCLUDED_CV2X_SUBFRAME_TO_SYMBOL_VECTOR_IMPL_H

#include <cv2x/subframe_to_symbol_vector.h>

namespace gr {
  namespace cv2x {

    class subframe_to_symbol_vector_impl : public subframe_to_symbol_vector
    {
     private:
        //System values
       static const int NSLsymb = 7;
       static const int NRBsc = 12;
       const int NSLsc;
       int  symbol;
       int NFFT;

     public:
      subframe_to_symbol_vector_impl(int NSLRB, int NFFT);
      ~subframe_to_symbol_vector_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_SUBFRAME_TO_SYMBOL_VECTOR_IMPL_H */
