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

#ifndef INCLUDED_CV2X_SSSS_SYMBOL_SELECTOR_CVC_IMPL_H
#define INCLUDED_CV2X_SSSS_SYMBOL_SELECTOR_CVC_IMPL_H

#include <cv2x/ssss_symbol_selector_cvc.h>

namespace gr {
  namespace cv2x {

    class ssss_symbol_selector_cvc_impl : public ssss_symbol_selector_cvc
    {
      private:
         int d_fftl;
         int d_cpl;
         int d_cpl0;
         int d_slotl;
         int d_slot_num;
         int d_sym_num;
         int d_N_id_2;
         uint64_t d_abs_pos;
         uint64_t d_offset;
         pmt::pmt_t d_key;
         pmt::pmt_t d_id_key;
         pmt::pmt_t d_tag_id;
         pmt::pmt_t d_port_sync_end;

     public:
      ssss_symbol_selector_cvc_impl(int fftl, int syncPeriod);
      ~ssss_symbol_selector_cvc_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_SSSS_SYMBOL_SELECTOR_CVC_IMPL_H */
