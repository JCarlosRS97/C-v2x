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

#ifndef INCLUDED_CV2X_SSSS_TAGGER_CC_IMPL_H
#define INCLUDED_CV2X_SSSS_TAGGER_CC_IMPL_H

#include <cv2x/ssss_tagger_cc.h>

namespace gr {
  namespace cv2x {

    class ssss_tagger_cc_impl : public ssss_tagger_cc
    {
    private:
       int d_fftl;
       int d_cpl;
       int d_cpl0;
       int d_slotl;
       int d_syncFramel;
       int d_slot_num;
       uint64_t d_offset_0;
       pmt::pmt_t d_key;
       pmt::pmt_t d_tag_id;
       long d_syncFrame_start;
       int syncPeriod;

       void set_frame_start(long sync_frame_start);
       void handle_msg_frame_start(pmt::pmt_t msg);

     public:
      ssss_tagger_cc_impl(int fftl, int syncPeriod);
      ~ssss_tagger_cc_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_SSSS_TAGGER_CC_IMPL_H */
