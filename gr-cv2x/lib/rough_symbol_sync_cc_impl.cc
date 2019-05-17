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
#include "rough_symbol_sync_cc_impl.h"

#include <fftw3.h>
#include <volk/volk.h>

#include <cstdio>

namespace gr {
  namespace cv2x {

    rough_symbol_sync_cc::sptr
    rough_symbol_sync_cc::make(int fftl, int vlen, int subcarrierBW, boost::shared_ptr<gr::analog::sig_source_c> &sig)
    {
      return gnuradio::get_initial_sptr
      (new rough_symbol_sync_cc_impl(fftl, vlen, subcarrierBW, sig));
    }

    /*
    * The private constructor
    */
    rough_symbol_sync_cc_impl::rough_symbol_sync_cc_impl(int fftl, int vlen, int subcarrierBW, boost::shared_ptr<gr::analog::sig_source_c> &sig)
    : gr::sync_block("rough_symbol_sync_cc",
    gr::io_signature::make( 1, 1, sizeof(gr_complex)*vlen),
    gr::io_signature::make( 1, 1, sizeof(gr_complex)*vlen)),
    d_fftl(fftl),
    d_sig(sig),
    d_cpl(144*fftl/2048),
    d_cpl0(160*fftl/2048),
    d_syml(fftl + d_cpl),
    d_syml0(fftl + d_cpl0),
    d_slotl(7*fftl+6*d_cpl+d_cpl0),
    d_sym_pos(0),
    d_is_locked(false),
    d_corr_val(0.0),
    d_work_call(0),
    stp(d_cpl0/4),
    d_f_av(0.0),
    subcarrierBW(subcarrierBW),
    d_vlen(vlen)
    {
      d_key=pmt::string_to_symbol("symbol");
      d_tag_id=pmt::string_to_symbol(this->name() );

      d_cp0 = (gr_complex*)fftwf_malloc(sizeof(gr_complex)*d_cpl0*d_vlen);
      d_cp1 = (gr_complex*)fftwf_malloc(sizeof(gr_complex)*d_cpl0*d_vlen);
      d_conj = (gr_complex*)fftwf_malloc(sizeof(gr_complex)*d_cpl0*d_vlen);
    }

    /*
    * Our virtual destructor.
    */
    rough_symbol_sync_cc_impl::~rough_symbol_sync_cc_impl()
    {
      fftwf_free(d_cp0);
      fftwf_free(d_cp1);
      fftwf_free(d_conj);
    }

    void rough_symbol_sync_cc_impl::forecast(int noutput_items,
      gr_vector_int &ninput_items_required)
      {
        //more than fftl+stp input items are required for correlation (standart too small)
        //maybe block before cp-sync requires bigger output buffer


        //~16*4 correlations in work function
        if(noutput_items< d_fftl+d_cpl*16){
          ninput_items_required[0]=d_fftl+d_cpl*16;
        } else	{
          ninput_items_required[0]=noutput_items;
        }
      }

      int
      rough_symbol_sync_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
        {
          const gr_complex *in = (const gr_complex *) input_items[0];
          gr_complex *out = (gr_complex *) output_items[0];
        //   if(nitems_read(0) < 25000){
        //   memcpy(out, in, sizeof(gr_complex)*noutput_items*d_vlen );
        //   return noutput_items;
        // }

          //printf("%s.work\tnoutput_items = %i\tnitems_read = %ld\n", name().c_str(), noutput_items, nitems_read(0) );
          int nout = 0;
          int mod;

          int fine_pos = 0;
          gr_complex it_val;
          float peso;
          float it_peso = 0;

          //d_ass_sync_frame_start para nosotros será donde comienza la subtrama de referencia
          long abs_pos, max_pos;
          long nir = nitems_read(0);
          int i;
          for(i = 0 ; (i + d_fftl + d_cpl) < noutput_items; i++){
            abs_pos = nir+long(i); // calculate new absolute sample position
            mod = abs((abs(abs_pos-d_sym_pos+int((d_cpl0-d_cpl)/2))%d_slotl)-d_syml0)%d_syml;
            if(d_is_locked && mod < stp){ // tracking mode
              memcpy(d_cp0,in+i*d_vlen         ,sizeof(gr_complex)*d_cpl*d_vlen);
              memcpy(d_cp1,in+(i+d_fftl)*d_vlen,sizeof(gr_complex)*d_cpl*d_vlen);
              gr_complex val = corr(peso, d_cp0,d_cp1,d_cpl*d_vlen);

              if(it_peso <= peso ){
                it_val = val;
                it_peso = peso;
                max_pos = abs_pos;
                if(d_corr_val < peso ){
                  d_corr_val = peso;
                  d_sym_pos = abs_pos%d_slotl;
                  // printf("corr_val = %f\tsym_pos = %ld\tabs_pos = %ld\n", d_corr_val, d_sym_pos, abs_pos);
                }
              }
              //si es la ultima iteracion
              // printf("Pos %ld\tmod = %i\tsym_pos = %ld\td_corr_val = %f\n", abs_pos,mod, d_sym_pos, d_corr_val);
              if(mod + 1 == stp){
                // printf("tracking abs_pos: %ld\tmax_pos = %i\tcorr= %f\n", max_pos, int(max_pos%d_slotl), it_peso);
                i += d_fftl;
                it_peso = 0;
                max_pos = 0;
              }

            } else if(!d_is_locked && abs_pos%stp==0){ //Find mode
              memcpy(d_cp0,in+i*d_vlen          ,sizeof(gr_complex)*d_cpl*d_vlen);
              memcpy(d_cp1,in+(i+d_fftl)*d_vlen ,sizeof(gr_complex)*d_cpl*d_vlen);
              gr_complex val = corr(peso, d_cp0,d_cp1,d_cpl*d_vlen);

              int find_pos;
              if(d_corr_val < peso){
                if(i < stp){
                  fine_pos = 0;
                }else{
                  fine_pos = i - stp;
                }
                for(int j = fine_pos ; j < i+stp; j++){
                  memcpy(d_cp0,in+j*d_vlen         ,sizeof(gr_complex)*d_cpl*d_vlen);
                  memcpy(d_cp1,in+(j+d_fftl)*d_vlen,sizeof(gr_complex)*d_cpl*d_vlen);
                  gr_complex val = corr(peso, d_cp0,d_cp1,d_cpl*d_vlen);
                  if(peso > 0.75){
                    d_corr_val = peso;
                    find_pos = j;
                    d_sym_pos = (nir + j)%d_slotl;
                    d_is_locked = true;
                    //if(d_corr_val)
                    //printf("%s\tfine corr sym_pos = %ld\n",name().c_str(), d_sym_pos );
                  }
                }
              }
              // Update counter in function of the mode
              if(!d_is_locked){
                i +=stp-1;
              } else {
                i = find_pos + d_fftl;
              }
              // printf("Find Pos %ld\tsym_pos = %ld\td_corr_val = %f\n", abs_pos, d_sym_pos, d_corr_val);
            }

          }

          d_corr_val *= 0.99;
          // If d_corr_val loss the peak, we change to find mode
          if(d_corr_val<0.75){
            d_is_locked = false;
          }
          nout = i;

          if(d_is_locked /*&& it_peso >= d_corr_val*/){ // Only is corrected cfo if
            // Only if correlation value is modified, cfo estimation is corrected
            float coef = nitems_read(0)<2000? 0.5 : 0.8;
            float f_off = arg(it_val)/(2*M_PI)*15000.0;
            d_f_av=d_f_av*coef - ((1-coef) * f_off);
            (*d_sig).set_frequency((-1)*double(d_f_av) );
            // printf("offset: %f\n", d_f_av);
            // printf("abs_pos = %ld\t offset= %f\n", nitems_read(0) + fine_pos, d_f_av);
          }

          // actually the next block doesn't care about the exact tag position. Only the value and key are important.
          memcpy(out, in, sizeof(gr_complex)*nout*d_vlen );
          add_item_tag(0,nitems_read(0)+5,d_key, pmt::from_long(d_sym_pos),d_tag_id);
          // printf("noutput_items %i\t nout = %i\n", noutput_items, nout);
          // printf("consumed items: %i\n", nout);
          // printf("Input buffer %f\tOutput buffer %f\n", pc_input_buffers_full(0), pc_output_buffers_full(0));
          // Tell runtime system how many output items we produced.
          return nout;
        }

        gr_complex
        rough_symbol_sync_cc_impl::corr(float &max, gr_complex *x, gr_complex *y, int len)
        {
          gr_complex resultado;
          gr_complex energia;
          volk_32fc_conjugate_32fc_a(d_conj, y, len);
          volk_32fc_x2_dot_prod_32fc(&resultado, x, d_conj, len);
          volk_32fc_x2_dot_prod_32fc(&energia, y, d_conj, len);
          if(abs(energia) < 0.01){
            max = -1;
          }else{
            max = abs(resultado/energia);
          }
          return resultado;
        }

      } /* namespace cv2x */
    } /* namespace gr */
