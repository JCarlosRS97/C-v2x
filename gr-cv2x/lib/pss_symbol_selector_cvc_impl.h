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

#ifndef INCLUDED_CV2X_PSS_SYMBOL_SELECTOR_CVC_IMPL_H
#define INCLUDED_CV2X_PSS_SYMBOL_SELECTOR_CVC_IMPL_H

#include <cv2x/pss_symbol_selector_cvc.h>

namespace gr {
  namespace cv2x {

    class pss_symbol_selector_cvc_impl : public pss_symbol_selector_cvc
    {
     private:
       int d_fftl;              // fft length
       int d_cpl;               // cp length
       int d_cpl0;              // first cp length
       int d_slotl;             // slot length
       int d_syml;              // symbol length
       int d_syml0;             // first symbol length

       long d_offset;           // symbol offset calculated before
       long d_ass_sync_frame_start; //start pos of sync subframe
       pmt::pmt_t d_key;        // key tag to send abs_pos of output
       pmt::pmt_t d_sym_key;    // key tag to receive d_offset
       pmt::pmt_t d_tag_id;     // id of block needed to send tags
       bool d_is_locked;        // indicator of sync end
       int syncPeriod;          // distance in subframes of two sync subframes
                                // standart value = 160;
       int syncOffsetIndicator; // offset in subframes respect SFN
       int pss1_index[64];      // index of the first decimated psss
       int pss2_index[64];      // index of the second decimated psss
       int nfft;                // length of decimated psss. Constant.


       void produce_output(gr_complex *&out, const gr_complex *in, long abs_pos, int &nout);
       void handle_msg_lock(pmt::pmt_t msg);
       void handle_msg_sync_frame_start(pmt::pmt_t msg);

     public:
      pss_symbol_selector_cvc_impl(int fft, int syncPeriod, int syncOffsetIndicator);
      ~pss_symbol_selector_cvc_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
     void set_sync_frame_start(long start){d_ass_sync_frame_start = start;}
     void lock(){d_is_locked = true;
      //  printf("Selector duracion: %f\n", pc_work_time_avg 	() 	);
}
     void unlock(){d_is_locked = false;}
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_PSS_SYMBOL_SELECTOR_CVC_IMPL_H */
