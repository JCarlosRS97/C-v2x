#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2019 <+YOU OR YOUR COMPANY+>.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import cv2x_swig as cv2x

class qa_pss_symbol_selector_cvc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        src = blocks.file_source(gr.sizeof_gr_complex, "capture.dat", True);
        simbolsync = lte.rough_symbol_sync_cc(512, 1);
        dst = blocks.file_sink(gr.sizeof_gr_complex, "lol.dat")
        self.tb.connect (src,simbolsync)
        self.tb.connect(simbolsync, dst);
        self.tb.run ()
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_pss_symbol_selector_cvc, "qa_pss_symbol_selector_cvc.xml")
