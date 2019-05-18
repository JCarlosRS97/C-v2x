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

from gnuradio import gr_unittest
import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))

from PyQt4 import Qt
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import gr
import gnuradio.gr.hier_block2
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from lte_ssss_sync import lte_ssss_sync  # grc-generated hier_block
from optparse import OptionParser
from pss_time_sync import pss_time_sync  # grc-generated hier_block
import cv2x
import sip
import pmt



sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))
import cv2x_swig as cv2x
from pss_time_sync import pss_time_sync  # grc-generated hier_block
from lte_ssss_sync import lte_ssss_sync  # grc-generated hier_block

class message_consumer(gr.sync_block):
    def __init__(self):
        gr.sync_block.__init__(
            self,
            name = "message consumer",
            in_sig = None,
            out_sig = None
        )
        self.cont = 0
        self.message_port_register_in(pmt.intern('in_port'))
        self.set_msg_handler(pmt.intern('in_port'),
                             self.handle_msg)

    def handle_msg(self, msg):
        # Create a new PMT from long value and put in list
        print(pmt.to_long(msg))

class qa_psss_time_sync (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        ##################################################
        # Variables
        ##################################################
        self.fft_len = fft_len = 256
        self.SubcarrierBW = SubcarrierBW = 15000
        self.syncPeriod = syncPeriod = 10
        self.samp_rate = samp_rate = SubcarrierBW*fft_len
        self.cont = 0;
        ##################################################
        # Blocks
        ##################################################
        self.sig = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, 0, 1, 0)


        self.pss_time_sync_0 = pss_time_sync(
            fft_len=fft_len,
            samp_rate=samp_rate,
            syncPeriod=syncPeriod,
        )
        self.lte_ssss_sync_0 = lte_ssss_sync(
            fft_len=fft_len,
            syncPeriod=syncPeriod,
        )
        self.fft_vxx_0 = fft.fft_vcc(fft_len, False, (), True, 1)
        self.cv2x_slss_generator_0 = cv2x.slss_generator(120, 0, 0, syncPeriod, fft_len)
        self.cv2x_rough_symbol_sync_cc_0 = cv2x.rough_symbol_sync_cc(fft_len, 1, SubcarrierBW, self.sig)
        self.cv2x_lte_cyclic_prefixer_vcc_0 = cv2x.lte_cyclic_prefixer_vcc(fft_len)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*fft_len, samp_rate/fft_len,True)
        self.blocks_multiply_xx_0_0 = blocks.multiply_vcc(1)
        self.blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        self.blocks_message_debug_0 = blocks.message_debug()
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_gr_complex*256, '/home/carlos/Escritorio/matlab/data/random.dat', True)
        self.blocks_delay_0 = blocks.delay(gr.sizeof_gr_complex*1, 21)
        self.blocks_add_xx_1 = blocks.add_vcc(fft_len)
        self.analog_sig_source_x_0_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, -1000, 1, 0)
        self.vector_sink_c0 = blocks.vector_sink_c()
        self.message_consumer0 = message_consumer()

        ##################################################
        # Connections
        ##################################################
        self.tb.msg_connect((self.lte_ssss_sync_0, 'SLSSID'), (self.message_consumer0, 'in_port'))
        self.tb.connect((self.analog_sig_source_x_0_0, 0), (self.blocks_multiply_xx_0_0, 0))
        self.tb.connect((self.blocks_add_xx_1, 0), (self.blocks_throttle_0, 0))
        self.tb.connect((self.blocks_delay_0, 0), (self.blocks_multiply_xx_0_0, 1))
        self.tb.connect((self.blocks_file_source_0, 0), (self.blocks_add_xx_1, 1))
        self.tb.connect((self.blocks_multiply_xx_0, 0), (self.pss_time_sync_0, 0))
        self.tb.connect((self.blocks_multiply_xx_0_0, 0), (self.cv2x_rough_symbol_sync_cc_0, 0))
        self.tb.connect((self.blocks_throttle_0, 0), (self.fft_vxx_0, 0))
        self.tb.connect((self.cv2x_lte_cyclic_prefixer_vcc_0, 0), (self.blocks_delay_0, 0))
        self.tb.connect((self.cv2x_rough_symbol_sync_cc_0, 0), (self.blocks_multiply_xx_0, 0))
        self.tb.connect((self.cv2x_slss_generator_0, 0), (self.blocks_add_xx_1, 0))
        self.tb.connect((self.fft_vxx_0, 0), (self.cv2x_lte_cyclic_prefixer_vcc_0, 0))
        self.tb.connect((self.lte_ssss_sync_0, 0), (self.vector_sink_c0, 0))
        self.tb.connect((self.pss_time_sync_0, 0), (self.lte_ssss_sync_0, 0))
        self.tb.connect((self.sig, 0), (self.blocks_multiply_xx_0, 1))








        # set up fg
        self.tb.run ()
        self.time.sleep(1)
        self.tb.stop()
        self.tb.wait()
        # check data



if __name__ == '__main__':
    #gr_unittest.run(qa_psss_time_sync, "qa_psss_time_sync.xml")
    qa_psss_time_sync().test_001_t()
