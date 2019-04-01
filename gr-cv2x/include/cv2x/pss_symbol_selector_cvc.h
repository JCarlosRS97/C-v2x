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


#ifndef INCLUDED_CV2X_PSS_SYMBOL_SELECTOR_CVC_H
#define INCLUDED_CV2X_PSS_SYMBOL_SELECTOR_CVC_H

#include <cv2x/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace cv2x {

    /*!
     * \brief <+description of block+>
     * \ingroup cv2x
     *
     */
    class CV2X_API pss_symbol_selector_cvc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<pss_symbol_selector_cvc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of cv2x::pss_symbol_selector_cvc.
       *
       * To avoid accidental use of raw pointers, cv2x::pss_symbol_selector_cvc's
       * constructor is in a private implementation
       * class. cv2x::pss_symbol_selector_cvc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int fft, int syncPeriod, int syncOffsetIndicator);
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_PSS_SYMBOL_SELECTOR_CVC_H */

