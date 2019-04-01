# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
install(FILES
    cv2x_subframe_to_symbol_vector.xml
    cv2x_slss_generator.xml
    cv2x_pss_symbol_selector_cvc.xml
    cv2x_extract_subcarriers_vcvc.xml
    cv2x_rough_symbol_sync_cc.xml
    cv2x_pss_calculator_vcm.xml DESTINATION share/gnuradio/grc/blocks
)
