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
#include "pss_symbol_selector_cvc_impl.h"

namespace gr {
  namespace cv2x {

    pss_symbol_selector_cvc::sptr
    pss_symbol_selector_cvc::make(int fft, int syncPeriod, int syncOffsetIndicator)
    {
      return gnuradio::get_initial_sptr
        (new pss_symbol_selector_cvc_impl(fft, syncPeriod, syncOffsetIndicator));
    }

    /*
     * The private constructor
     */
    pss_symbol_selector_cvc_impl::pss_symbol_selector_cvc_impl(int fft, int syncPeriod, int syncOffsetIndicator)
      : gr::block("pss_symbol_selector_cvc",
          gr::io_signature::make( 1, 1, sizeof(gr_complex)),
          gr::io_signature::make( 1, 1, sizeof(gr_complex) * 128)),//2*nfft
            d_fftl(fft),
            d_cpl(144*fft/2048),
            d_cpl0(160*fft/2048),
            d_slotl(7*fft+6*d_cpl+d_cpl0),
            d_syml(fft+d_cpl),
            d_syml0(fft+d_cpl0),
            d_offset(0),
            d_ass_sync_frame_start(3*syncPeriod*d_slotl),
            d_is_locked(false),
            syncPeriod(syncPeriod),
            syncOffsetIndicator(syncOffsetIndicator),
            nfft(64)
    {
      //set_output_multiple(2);
      set_relative_rate(1/d_syml);
      set_tag_propagation_policy(TPP_DONT);
      d_key = pmt::string_to_symbol("offset_marker");
      d_sym_key = pmt::string_to_symbol("symbol");
      d_tag_id = pmt::string_to_symbol(this->name() );

      message_port_register_in(pmt::mp("lock"));
      set_msg_handler(pmt::mp("lock"), boost::bind(&pss_symbol_selector_cvc_impl::handle_msg_lock, this, _1));

      message_port_register_in(pmt::mp("sync_frame"));
      set_msg_handler(pmt::mp("sync_frame"), boost::bind(&pss_symbol_selector_cvc_impl::handle_msg_sync_frame_start, this, _1));

      for(int i = 0; i < nfft; i++){
        pss1_index[i] = i*(d_fftl/nfft) + d_cpl;
        pss2_index[i] = i*(d_fftl/nfft) + d_cpl*2 + d_fftl;
      }
    }

    void
    pss_symbol_selector_cvc_impl::handle_msg_sync_frame_start(pmt::pmt_t msg)
    {
        set_sync_frame_start(pmt::to_long(msg) );
    }

    void
    pss_symbol_selector_cvc_impl::handle_msg_lock(pmt::pmt_t msg)
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
    pss_symbol_selector_cvc_impl::~pss_symbol_selector_cvc_impl()
    {
    }

    void
    pss_symbol_selector_cvc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      unsigned ninputs = ninput_items_required.size ();
      for (unsigned i = 0; i < ninputs; i++){
          ninput_items_required[i] =  d_syml0*2 + d_syml * (noutput_items - 1) + history() - 1;
      }
    }

    int
    pss_symbol_selector_cvc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      int nin = ninput_items[0];
      long nir = nitems_read(0);
      bool is_locked = d_is_locked;

      // First thing to do --> get tag from preceding block
      std::vector <gr::tag_t> v_off;
      get_tags_in_range(v_off,0,nir,nir+nin,d_sym_key);
      if (v_off.size() > 0){
          long offset = pmt::to_long(v_off[0].value);
          if(offset != d_offset){
              d_offset = offset;
              // printf("Offset %ld\n", d_offset);
          }
      }
      int offset = d_offset;
      if(offset == 0){ // if offset is "0" then there is no symbol sync yet --> return!
          consume_each(nin);
          return 0;
      }

      // generate output
      int consumed_items = 0;
      int nout = 0;
      //d_ass_sync_frame_start para nosotros será donde comienza la subtrama de referencia
      long pss_pos = (d_ass_sync_frame_start+(d_fftl+d_cpl0)-6 )%(syncPeriod*2*d_slotl);
      long abs_pos;
      int mod_pss;
      int i;
      for(i = 0 ; (i+2*d_syml0) < nin ; i++){
          abs_pos = nir+long(i); // calculate new absolute sample position
          mod_pss = abs( (int(abs_pos-(pss_pos) ))%(syncPeriod*2*d_slotl) );
          if(d_ass_sync_frame_start < syncPeriod*2* d_slotl && mod_pss < 12 ){
              // If it is near to the maximun correlation value(tracking)
              produce_output(out, in+i, abs_pos, nout);
              //consumed_items = i+1;
              //printf("%s--> generate output sync_frame_start\tmod_pss = %i\tabs_pos = %ld\tpss_pos = %ld\tframe = %ld\t offset %i\n", name().c_str(), mod_pss, abs_pos,pss_pos,d_ass_sync_frame_start, offset );
          }
          else if(is_locked){//For now step over all samples
              //consumed_items = i+1;
          }
          else if(((abs(abs_pos-offset)%d_slotl)-d_syml0)%d_syml == 0){
              // At the first sample of a symbol when it isn't at tracking mode
              // yet
              produce_output(out, in+i, abs_pos, nout);
              i += (d_syml-10); // Reduce iterations
              //consumed_items = i+1; // +1 because i is initialized with 0
          }

          // break if maximum number of output vectors are produced!
          if(nout == noutput_items){break;}
      }
      consumed_items = i+1;
      // Tell runtime system how many input items we consumed on each input stream
      // printf("noutput_items %i\t nout = %i\n", noutput_items, nout);
      // printf("consumed items: %i\n", consumed_items);
      // printf("Input buffer %f\tOutput buffer %f\n", pc_input_buffers_full(0), pc_output_buffers_full(0));
      // printf("nin: %i\n", nin);
      consume_each (consumed_items);
      // Tell runtime system how many output items we produced.
      return nout;
    }

    // method contains steps to produce output with all necessary steps! Does not create any new variables etc.
    void
    pss_symbol_selector_cvc_impl::produce_output(gr_complex *&out, const gr_complex *in, long abs_pos, int &nout)
    {
        //extract psss symbols decimated
        for(int i = 0; i < nfft; i++){
          out[i] = in[pss1_index[i]];
          out[i + nfft] = in[pss2_index[i]];
        }

        // pss_calc needs the exact position of first sample in stream
        add_item_tag(0,nitems_written(0)+nout,d_key, pmt::from_long( abs_pos ),d_tag_id);

        out += 2*nfft; //move pointer to output buffer by the size of one vector
        nout += 1; // 1 output vector produced
    }
  } /* namespace cv2x */
} /* namespace gr */
