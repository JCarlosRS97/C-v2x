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

#ifndef INCLUDED_CV2X_PSSS_TIME_SYNC_IMPL_H
#define INCLUDED_CV2X_PSSS_TIME_SYNC_IMPL_H

#include <cv2x/psss_time_sync.h>

namespace gr {
  namespace cv2x {

    class psss_time_sync_impl : public psss_time_sync
    {
      static const gr_complex d_C_I;
      static const float d_PI;
      int d_fftl;
      int d_cpl;
      int d_cpl0;
      int d_slotl;
      int d_N_id_2;
      long d_sync_frame_start;
      float d_corr_val;
      int d_lock_count;
      bool d_is_locked;
      int syncPeriod;
      int nfft;

      pmt::pmt_t d_port_lock;
      pmt::pmt_t d_port_sync_frame_start;
      pmt::pmt_t d_port_N_id_2;

      std::vector<float> d_corr_vec;

      void zc(gr_complex *zc, int cell_id); // used to generate Zadoff-Chu sequences
      bool find_pss_symbol(); // prepares the calculation stuff etc.
      bool tracking();
      void max_pos(float &max, gr_complex *x, int len); //finds maximum of one correlation
      void mi_max_pos(float &max, gr_complex *x, int len); //finds maximum of one correlation


      inline void set_sync_frame_start();
      inline int calculate_sync_frame_start(long pos);
      // attributes for correlation
      gr_complex *d_corr_in;
      gr_complex *d_chu0_t;
      gr_complex *d_chu1_t;

     public:
      psss_time_sync_impl(int fftl, int syncPeriod);
      ~psss_time_sync_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_PSSS_TIME_SYNC_IMPL_H */
