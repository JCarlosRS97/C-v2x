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

#ifndef INCLUDED_CV2X_PSS_TAGGER_CC_IMPL_H
#define INCLUDED_CV2X_PSS_TAGGER_CC_IMPL_H

#include <cv2x/pss_tagger_cc.h>

namespace gr {
  namespace cv2x {

    class pss_tagger_cc_impl : public pss_tagger_cc
    {
     private:
      int syncPeriod;
      int d_fftl;
      int d_cpl;
      int d_cpl0;
      int d_slotl;
      int d_sync_frame_start;
      int d_N_id_2;
      bool d_is_locked;
      pmt::pmt_t d_key;
      pmt::pmt_t d_id_key;
      pmt::pmt_t d_tag_id;
      int sync_framel;
      int cont;
      long next_sync;

      void handle_msg_lock(pmt::pmt_t msg);
      void handle_msg_sync_frame_start(pmt::pmt_t msg);
      void handle_msg_N_id_2(pmt::pmt_t msg);

     public:
      pss_tagger_cc_impl(int fftl, int syncPeriod);
      ~pss_tagger_cc_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

      void set_sync_frame_start(int start){d_sync_frame_start = start;}
      void set_N_id_2(int nid2){d_N_id_2 = nid2;}
      void lock(){d_is_locked = true;}
      void unlock(){d_is_locked = false;}
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_PSS_TAGGER_CC_IMPL_H */
