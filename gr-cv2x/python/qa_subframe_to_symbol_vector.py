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
from gnuradio import fft

class qa_subframe_to_symbol_vector (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        sqr = cv2x.slss_generator(301, 0, 0, 3);
        dst = blocks.file_sink(gr.sizeof_gr_complex*128, "capture.dat");
        test = blocks.file_sink(gr.sizeof_gr_complex*1008, "source.dat");
        divisor = cv2x.subframe_to_symbol_vector(6, 128);
        h = blocks.head(1008*gr.sizeof_gr_complex, 5);
        h2 = blocks.head(128*gr.sizeof_gr_complex, 14*5);
        fft1 = fft.fft_vcc(128, False, (), True, 1)
        self.tb.connect (sqr,h)
        self.tb.connect(h, divisor);
        self.tb.connect(divisor, fft1);
        self.tb.connect(fft1, h2);
        self.tb.connect(h2, dst);
        self.tb.connect (h, test)
        self.tb.run ();


if __name__ == '__main__':
    gr_unittest.run(qa_subframe_to_symbol_vector, "qa_subframe_to_symbol_vector.xml")
