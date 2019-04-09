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


#ifndef INCLUDED_CV2X_SSSS_CALCULATOR_VCM_H
#define INCLUDED_CV2X_SSSS_CALCULATOR_VCM_H

#include <cv2x/api.h>
#include <gnuradio/sync_decimator.h>

#include <cstdio>

namespace gr {
  namespace cv2x {

    /*!
     * \brief <+description of block+>
     * \ingroup cv2x
     *
     */
    class CV2X_API ssss_calculator_vcm : virtual public gr::sync_decimator
    {
     public:
      typedef boost::shared_ptr<ssss_calculator_vcm> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of cv2x::ssss_calculator_vcm.
       *
       * To avoid accidental use of raw pointers, cv2x::ssss_calculator_vcm's
       * constructor is in a private implementation
       * class. cv2x::ssss_calculator_vcm::make is the public interface for
       * creating new instances.
       */
      static sptr make(int fftl, std::string key_id, std::string key_offset, int syncPeriod);
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_SSSS_CALCULATOR_VCM_H */
