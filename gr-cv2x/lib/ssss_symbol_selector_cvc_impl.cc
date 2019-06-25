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
#include "ssss_symbol_selector_cvc_impl.h"

namespace gr {
  namespace cv2x {

    ssss_symbol_selector_cvc::sptr
    ssss_symbol_selector_cvc::make(int fftl, int syncPeriod)
    {
      return gnuradio::get_initial_sptr
      (new ssss_symbol_selector_cvc_impl(fftl, syncPeriod));
    }

    /*
    * The private constructor
    */
    ssss_symbol_selector_cvc_impl::ssss_symbol_selector_cvc_impl(int fftl, int syncPeriod)
    : gr::block("ssss_symbol_selector_cvc",
    gr::io_signature::make( 1, 1, sizeof(gr_complex)),
    gr::io_signature::make( 1, 1, sizeof(gr_complex) * 64)),
    d_fftl(fftl),
    d_cpl(144*fftl/2048),
    d_cpl0(160*fftl/2048),
    d_slotl(7*fftl+6*d_cpl+d_cpl0),
    d_slot_num(0),
    d_sym_num(0),
    d_abs_pos(0),
    d_offset(0),
    d_N_id_2(-1)
    {
      set_relative_rate(1.0/double(syncPeriod*d_slotl));
      set_output_multiple(2);
      //printf("rel_rate = %f\n",relative_rate());
      set_tag_propagation_policy(TPP_DONT);
      d_key = pmt::string_to_symbol("offset_marker");
      d_id_key = pmt::string_to_symbol("N_id_2");
      d_tag_id = pmt::string_to_symbol(this->name() );
      d_port_sync_end = pmt::string_to_symbol("sync_end");
      message_port_register_out(d_port_sync_end);
    }

    /*
    * Our virtual destructor.
    */
    ssss_symbol_selector_cvc_impl::~ssss_symbol_selector_cvc_impl()
    {
    }

    void
    ssss_symbol_selector_cvc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      for(int i = 0 ; i < ninput_items_required.size() ; i++){
        ninput_items_required[i] = noutput_items*(d_fftl + d_cpl/2);
      }
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    ssss_symbol_selector_cvc_impl::general_work (int noutput_items,
      gr_vector_int &ninput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items)
      {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        //printf("work! nitems_read+ninput_items = %ld\t", nitems_read(0) + ninput_items[0]);
        std::vector <gr::tag_t> v_id;
        get_tags_in_range(v_id,0,nitems_read(0),nitems_read(0)+ninput_items[0],d_id_key);
        if (v_id.size() > 0){
          d_N_id_2 = int(pmt::to_long(v_id[0].value));
          d_offset = v_id[0].offset;
          d_abs_pos = v_id[0].offset+11*d_fftl+10*d_cpl+2*d_cpl0; // points at the exact beginning of a SSSS symbol.
          // printf("%s found N_id_2 = %i\t id_off = %ld\n",name().c_str(), d_N_id_2, d_offset);
        }


        if(d_abs_pos >= nitems_read(0) && d_N_id_2 >= 0 ){//Si esta por pasar
          // printf("found:  d_abs_pos = %ld >= %ld = nitems_read\tnoutputs= %i\n", d_abs_pos, nitems_read(0), ninput_items[0]  );


          if(d_abs_pos+2*d_fftl + d_cpl < nitems_read(0)+ninput_items[0]){//Si va a pasar en esta iteracion
            // printf("\nPRODUCE real output!\tdiff = %ld\tpos = %ld\tninput = %i\n\n",d_abs_pos-nitems_read(0), d_abs_pos, ninput_items[0]);

            // memcpy(out,in+(d_abs_pos-nitems_read(0) ),sizeof(gr_complex)*d_fftl);
            // memcpy(out + d_fftl,in+(d_abs_pos-nitems_read(0) + d_fftl + d_cpl),sizeof(gr_complex)*d_fftl);

            int nfft = 64;
            for(int i = 0; i < nfft; i++){
              out[i] = in[i*(d_fftl/nfft) + (d_abs_pos-nitems_read(0))];
              //printf("pos %i value: %f\n", i, out[i]);
              out[i + nfft] = in[i*(d_fftl/nfft) + (d_abs_pos-nitems_read(0)) + d_fftl + d_cpl];
            }

            add_item_tag(0,nitems_written(0),d_key   , pmt::from_long( d_offset ),d_tag_id);
            add_item_tag(0,nitems_written(0),d_id_key, pmt::from_long( d_N_id_2 ),d_tag_id);
            pmt::pmt_t msg = pmt::from_long( d_abs_pos) ;
            message_port_pub( d_port_sync_end, msg );
            consume_each(ninput_items[0]);
            return 2;
          }
          else{
            int diff = int(d_abs_pos-nitems_read(0));
            //printf("nitems_read + diff = %ld\n",nitems_read(0)+diff );
            if(diff > ninput_items[0] ){//Esta lejos
              consume_each(ninput_items[0]);
            }
            else{//Si es que no caben los dos simbolos
              consume_each( diff );
            }
            return 0;
          }

        }
        else{//No hay objetivo marcado
          //printf("missed: d_abs_pos = %ld  < %ld = nitems_read\n", d_abs_pos, nitems_read(0) );
          consume_each(ninput_items[0]);
          return 0;
        }
      }

    } /* namespace lte */
  } /* namespace gr */
