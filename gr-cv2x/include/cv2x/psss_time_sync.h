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


#ifndef INCLUDED_CV2X_PSSS_TIME_SYNC_H
#define INCLUDED_CV2X_PSSS_TIME_SYNC_H

#include <cv2x/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace cv2x {

    /*!
     * \brief <+description of block+>
     * \ingroup cv2x
     *
     */
    class CV2X_API psss_time_sync : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<psss_time_sync> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of cv2x::psss_time_sync.
       *
       * To avoid accidental use of raw pointers, cv2x::psss_time_sync's
       * constructor is in a private implementation
       * class. cv2x::psss_time_sync::make is the public interface for
       * creating new instances.
       */
      static sptr make(int fftl, int syncPeriod);
    };

  } // namespace cv2x
} // namespace gr

#endif /* INCLUDED_CV2X_PSSS_TIME_SYNC_H */
