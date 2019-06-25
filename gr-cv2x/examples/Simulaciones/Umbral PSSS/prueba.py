#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Sat May 18 18:06:16 2019
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))



from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from ltev_rx_sync import ltev_rx_sync  # grc-generated hier_block
from optparse import OptionParser
from tx_v2x import tx_v2x  # grc-generated hier_block
import sip
import cv2x
import time
import pmt
from ltev_rx_sync import ltev_rx_sync  # grc-generated hier_block
from tx_v2x import tx_v2x  # grc-generated hier_block
import numpy as np

class tag_sink(gr.sync_block):
    def __init__(self):
        gr.sync_block.__init__(
            self,
            name = "tag sink",
            in_sig = [np.complex64],
            out_sig = None,
        )

        self.pos = -1
        self.N_id = -1
        self.sync_end = -1
        self.key = pmt.intern("sync_frame")
        self.message_port_register_in(pmt.intern('n_id'))
        self.set_msg_handler(pmt.intern('n_id'),
                             self.handle_N)
        self.message_port_register_in(pmt.intern('pos'))
        self.set_msg_handler(pmt.intern('pos'),
                             self.handle_pos)
        self.message_port_register_in(pmt.intern('sync_end'))
        self.set_msg_handler(pmt.intern('sync_end'),
                             self.handle_end)
    def work(self, input_items, output_items):
        num_input_items = len(input_items[0])
        return num_input_items
    def handle_N(self, msg):
        # Create a new PMT from long value and put in list
        self.N_id = pmt.to_long(msg)

    def handle_pos(self, msg):
        # Create a new PMT from long value and put in list
        self.pos = pmt.to_long(msg)

    def handle_end(self, msg):
        # Create a new PMT from long value and put in list
        if(self.sync_end == -1):
            self.sync_end = pmt.to_long(msg)

    def reset(self):
        self.N_id = -1
        self.pos = -1
        self.sync_end = -1

    def get_N_id(self):
        return self.N_id

    def get_end(self):
        return self.sync_end


    def get_pos(self):
        return self.pos

class top_block(gr.top_block):

    def __init__(self, vector,umbral):
        gr.top_block.__init__(self, "Top Block")

         ##################################################
        # Variables
        ##################################################
        self.fft_len = fft_len = 256
        self.SubcarrierBW = SubcarrierBW = 15000
        self.syncPeriod = syncPeriod = 160
        self.samp_rate = samp_rate = SubcarrierBW*fft_len

        ##################################################
        # Blocks
        ##################################################
        self.tx_v2x_0 = tx_v2x(
            SubcarrierBW=SubcarrierBW,
            fft_len=fft_len,
            slss_id=120,
            syncPeriod=syncPeriod,
        )

        self.ltev_rx_sync_0 = ltev_rx_sync(
            SubcarrierBW=15000,
            fft_len=256,
            syncPeriod=syncPeriod,
            umbralPSSS=umbral,
        )
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_multiply_xx_0_0 = blocks.multiply_vcc(1)
        self.blocks_message_debug_0 = blocks.message_debug()
        self.blocks_delay_0 = blocks.delay(gr.sizeof_gr_complex*1, 21)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_sig_source_x_0_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, 0, 1, 0)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 7.18, 0)
        self.parador = blocks.head(gr.sizeof_gr_complex, 3840000)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.ltev_rx_sync_0, 'N_id'), (vector, 'n_id'))
        self.msg_connect((self.ltev_rx_sync_0, 'sync_frame_start'), (vector, 'pos'))
        self.msg_connect((self.ltev_rx_sync_0, 'sync_end'), (vector, 'sync_end'))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.analog_sig_source_x_0_0, 0), (self.blocks_multiply_xx_0_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_xx_0_0, 1))
        self.connect((self.blocks_delay_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_multiply_xx_0_0, 0), (self.ltev_rx_sync_0, 0))
        self.connect((self.ltev_rx_sync_0, 0), (vector, 0))
        self.connect((self.tx_v2x_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.parador, 0))
        self.connect((self.parador, 0), (self.blocks_delay_0, 0))


    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len
        self.set_samp_rate(self.SubcarrierBW*self.fft_len)
        self.tx_v2x_0.set_fft_len(self.fft_len)

    def get_SubcarrierBW(self):
        return self.SubcarrierBW

    def set_SubcarrierBW(self, SubcarrierBW):
        self.SubcarrierBW = SubcarrierBW
        self.set_samp_rate(self.SubcarrierBW*self.fft_len)
        self.tx_v2x_0.set_SubcarrierBW(self.SubcarrierBW)

    def get_syncPeriod(self):
        return self.syncPeriod

    def set_syncPeriod(self, syncPeriod):
        self.syncPeriod = syncPeriod
        self.tx_v2x_0.set_syncPeriod(self.syncPeriod)
        self.ltev_rx_sync_0.set_syncPeriod(self.syncPeriod)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.analog_sig_source_x_0_0.set_sampling_freq(self.samp_rate)



def main(top_block_cls=top_block, options=None):

    vector = tag_sink()
    # QtCore.QTimer.connect(timer, QtCore.SIGNAL("timeout()"), qapp, Qt.SLOT('quit()'))

    def quitting():
        tb.stop()

    umbral = float(sys.argv[1])
    print("El umbral es " + sys.argv[1])
    n_id = []
    pos = []
    fin =[]
    for i in range(100):
        tb = top_block_cls(vector, umbral)
        n_id.append(vector.get_N_id())
        pos.append(vector.get_pos())
        fin.append(vector.get_end())
        vector.reset()
        tb.start()
        time.sleep(1)
        tb.stop()
        tb.wait()





    f=open("nid"+sys.argv[1] +"-"+sys.argv[2]+ ".txt", "w")
    for nid in n_id:
        f.write("%i\n" % nid)

    f.close()

    f=open("end"+sys.argv[1] +"-"+sys.argv[2]+ ".txt", "w")
    for i in fin:
        f.write("%i\n" % i)

    f.close()

    f=open("pos"+sys.argv[1]+"-"+sys.argv[2]+".txt", "w")
    for i in pos:
        f.write("%i\n" % i)

    f.close()


if __name__ == '__main__':
    main()
