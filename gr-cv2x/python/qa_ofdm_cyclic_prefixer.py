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

class qa_ofdm_cyclic_prefixer (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        sqr = cv2x.slss_generator(301, 0, 0, 8,512);
        tr = blocks.throttle(gr.sizeof_gr_complex*512,500, True);
        dst = blocks.file_sink(gr.sizeof_gr_complex, "capture.dat");
        h = blocks.head(512*gr.sizeof_gr_complex, 25*14);
        cp = cv2x.ofdm_cyclic_prefixer(512, (20,20), 0, "")
        self.tb.connect (sqr,tr)
        self.tb.connect(tr, h);
        self.tb.connect(h, cp);
        self.tb.connect(cp, dst);
        self.tb.run ();


if __name__ == '__main__':
    gr_unittest.run(qa_ofdm_cyclic_prefixer, "qa_ofdm_cyclic_prefixer.xml")
