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
#include "pss_tagger_cc_impl.h"

namespace gr {
  namespace cv2x {

    pss_tagger_cc::sptr
    pss_tagger_cc::make(int fftl, int syncPeriod)
    {
      return gnuradio::get_initial_sptr
      (new pss_tagger_cc_impl(fftl, syncPeriod));
    }

    /*
    * The private constructor
    */
    pss_tagger_cc_impl::pss_tagger_cc_impl(int fftl, int syncPeriod)
    : gr::sync_block("pss_tagger_cc",
    gr::io_signature::make( 1, 1, sizeof(gr_complex)),
    gr::io_signature::make( 1, 1, sizeof(gr_complex))),
    syncPeriod(syncPeriod),
    d_fftl(fftl),
    d_cpl(144*fftl/2048),
    d_cpl0(160*fftl/2048),
    d_slotl(7*fftl+6*d_cpl+d_cpl0),
    d_sync_frame_start(0),
    d_N_id_2(-1),
    sync_framel(syncPeriod*d_slotl*2),
    d_is_locked(false),
    cont(0),
    next_sync(0)
    {
      set_tag_propagation_policy(TPP_DONT);
      d_key=pmt::string_to_symbol("slot");
      d_tag_id=pmt::string_to_symbol(this->name() );
      d_id_key = pmt::string_to_symbol("N_id_2");

      message_port_register_in(pmt::mp("lock"));
      set_msg_handler(pmt::mp("lock"), boost::bind(&pss_tagger_cc_impl::handle_msg_lock, this, _1));

      message_port_register_in(pmt::mp("sync_frame"));
      set_msg_handler(pmt::mp("sync_frame"), boost::bind(&pss_tagger_cc_impl::handle_msg_sync_frame_start, this, _1));

      message_port_register_in(pmt::mp("N_id_2"));
      set_msg_handler(pmt::mp("N_id_2"), boost::bind(&pss_tagger_cc_impl::handle_msg_N_id_2, this, _1));
    }

    void
    pss_tagger_cc_impl::handle_msg_N_id_2(pmt::pmt_t msg)
    {
      set_N_id_2( (int) pmt::to_long(msg) );
    }

    void
    pss_tagger_cc_impl::handle_msg_sync_frame_start(pmt::pmt_t msg)
    {
      set_sync_frame_start(pmt::to_long(msg) );
    }

    void
    pss_tagger_cc_impl::handle_msg_lock(pmt::pmt_t msg)
    {
      if(msg == pmt::PMT_T)
      {
        lock();
      }
      else{
        unlock();
      }
    }

    /*
    * Our virtual destructor.
    */
    pss_tagger_cc_impl::~pss_tagger_cc_impl()
    {
    }

    int
    pss_tagger_cc_impl::work(int noutput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items)
      {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];
        long nir = nitems_read(0);

        memcpy(out,in,sizeof(gr_complex)*noutput_items);

        if(d_is_locked){
          if(cont == 0){ // first iteration after locked
            while((cont*sync_framel + d_sync_frame_start) < nir){
              cont++;
            }
            next_sync = cont*sync_framel + d_sync_frame_start;
          }
          // printf("proximo ssss= %ld\t pos=%ld\n", next_sync, nitems_read(0) + noutput_items);
          while(nitems_read(0) + noutput_items > next_sync){
            add_item_tag(0,next_sync,d_id_key, pmt::from_long(d_N_id_2),d_tag_id);
            cont++;
            next_sync = cont*sync_framel + d_sync_frame_start;
          }
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
      }

    } /* namespace lte */
  } /* namespace gr */
