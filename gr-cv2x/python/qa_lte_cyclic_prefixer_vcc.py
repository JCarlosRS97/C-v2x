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

class qa_lte_cyclic_prefixer_vcc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        ori = blocks.vector_source_c(range(256), True, 256);
        cp = cv2x.lte_cyclic_prefixer_vcc(256);
        h = blocks.head(gr.sizeof_gr_complex, 3840*100);
        dst = blocks.file_sink(gr.sizeof_gr_complex, "capture.dat");
        self.tb.connect (ori, cp);
        self.tb.connect(cp, h);
        self.tb.connect(h, dst);
        # set up fg
        self.tb.run ()
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_lte_cyclic_prefixer_vcc, "qa_lte_cyclic_prefixer_vcc.xml")
