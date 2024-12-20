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

#ifndef INCLUDED_CV2X_ROUGH_SYMBOL_SYNC_CC_IMPL_H
#define INCLUDED_CV2X_ROUGH_SYMBOL_SYNC_CC_IMPL_H

#include <cv2x/rough_symbol_sync_cc.h>

namespace gr {
  namespace cv2x {

    class rough_symbol_sync_cc_impl : public rough_symbol_sync_cc
    {
     private:
       int d_fftl;
       int d_cpl;
       int d_cpl0;
       int d_syml;
       int d_syml0;
       int d_slotl;
       bool simetria;
       long d_find_pos;
       long d_sym_pos;
       float d_corr_val;
       int d_work_call;
       float d_f_av;
       float subcarrierBW;
       bool d_is_locked;
       int stp;
       float umbral;
       float it_peso;
       boost::shared_ptr<gr::analog::sig_source_c> d_sig;
       gr_complex* d_cp0;
       gr_complex* d_cp1;
       gr_complex* d_conj;

       pmt::pmt_t d_key;
       pmt::pmt_t d_tag_id;

       gr_complex corr(gr_complex *x, gr_complex *y, int len);

     public:
      rough_symbol_sync_cc_impl(int fftl, int subcarrierBW, boost::shared_ptr<gr::analog::sig_source_c> &sig, float umbral, bool simetria);
      ~rough_symbol_sync_cc_impl();

      void forecast(int noutput_items,
	       gr_vector_int &ninput_items_required);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_ROUGH_SYMBOL_SYNC_CC_IMPL_H */
