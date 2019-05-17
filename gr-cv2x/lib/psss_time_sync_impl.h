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
#include <fftw3.h>

namespace gr {
  namespace cv2x {

    class psss_time_sync_impl : public psss_time_sync
    {
      static const float d_PI;
      int d_fftl;
      int d_cpl;
      int d_cpl0;
      int d_slotl;
      int d_N_id_2;
      long d_sync_frame_start;
      float d_corr_val;  // Actual maximun correlator value
      int d_lock_count;  // Count the number of inputs in which d_corr_val don't change
      bool d_is_locked;
      int syncPeriod;
      int nfft;
      int d_offset;

      pmt::pmt_t d_port_lock;
      pmt::pmt_t d_port_sync_frame_start;
      pmt::pmt_t d_port_N_id_2;


      void zc(gr_complex *zc, int cell_id); // used to generate Zadoff-Chu sequences
      bool find_pss_symbol(); // prepares the calculation stuff etc.
      bool tracking();
      void max_pos(float &max, gr_complex *x, int len); //finds maximum of one correlation
      void generate_time_psss(gr_complex *seq, int freq_offset, int N_id_2);


      inline void set_sync_frame_start();
      inline int calculate_sync_frame_start(long pos);
      // attributes for correlation
      gr_complex *d_corr_in;
      gr_complex *d_chu0_f0_t;
      gr_complex *d_chu1_f0_t;
      gr_complex *d_chu0_f1_t;
      gr_complex *d_chu1_f1_t;
      gr_complex *d_chu0_fm1_t;
      gr_complex *d_chu1_fm1_t;
      gr_complex *d_chu_f;
      gr_complex *d_chu_t;
      fftwf_plan d_plan_r;
      boost::shared_ptr<gr::analog::sig_source_c> d_sig;

     public:
      psss_time_sync_impl(int fftl, int syncPeriod, boost::shared_ptr<gr::analog::sig_source_c> &sig);
      ~psss_time_sync_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_PSSS_TIME_SYNC_IMPL_H */
