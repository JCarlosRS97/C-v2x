# Copyright 2011,2012 Free Software Foundation, Inc.
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

########################################################################
# Install public header files
########################################################################
install(FILES
    api.h
    subframe_to_symbol_vector.h
    slss_generator.h
    pss_symbol_selector_cvc.h
    extract_subcarriers_vcvc.h
    rough_symbol_sync_cc.h
    ofdm_cyclic_prefixer.h
    pss_calculator_vcm.h
    pss_tagger_cc.h
    sync_frequency_c.h
    ssss_symbol_selector_cvc.h
    ssss_calculator_vcm.h
    ssss_tagger_cc.h
    decimator_cc.h
    psss_time_sync.h
    lte_cyclic_prefixer_vcc.h
    pss_time_sim.h DESTINATION include/cv2x
)
