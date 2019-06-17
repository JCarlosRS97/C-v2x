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

from PyQt4 import Qt, QtCore
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
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
        self.key = pmt.intern("sync_frame")
        self.message_port_register_in(pmt.intern('in_port'))
        self.set_msg_handler(pmt.intern('in_port'),
                             self.handle_msg)

    def handle_msg(self, msg):
        # Create a new PMT from long value and put in list
        self.N_id = pmt.to_long(msg)

    def work(self, input_items, output_items):
        num_input_items = len(input_items[0])

        #put code here to process the input items...

        #print all the tags received in this work call
        nread = self.nitems_read(0)
        tags = self.get_tags_in_range(0, nread, nread+num_input_items, self.key)
        for tag in tags:
            self.pos = pmt.to_long(tag.value)

        return num_input_items

    def reset(self):
        self.N_id = -1
        self.pos = -1

    def get_N_id(self):
        return self.N_id

    def get_pos(self):
        return self.pos

class top_block(gr.top_block, Qt.QWidget):

    def __init__(self, vector,umbral):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

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
        self.qtgui_sink_x_0 = qtgui.sink_c(
        	1024, #fftsize
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate, #bw
        	"", #name
        	True, #plotfreq
        	True, #plotwaterfall
        	True, #plottime
        	True, #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_sink_x_0_win)

        self.qtgui_sink_x_0.enable_rf_freq(False)



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

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.ltev_rx_sync_0, 'out'), (vector, 'in_port'))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.analog_sig_source_x_0_0, 0), (self.blocks_multiply_xx_0_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_xx_0_0, 1))
        self.connect((self.blocks_delay_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_multiply_xx_0_0, 0), (self.ltev_rx_sync_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_delay_0, 0))
        self.connect((self.ltev_rx_sync_0, 0), (vector, 0))
        self.connect((self.tx_v2x_0, 0), (self.blocks_throttle_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

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
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.analog_sig_source_x_0_0.set_sampling_freq(self.samp_rate)



def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    #vector = blocks.tag_debug(gr.sizeof_gr_complex, "symbol")
    #vector.set_display(False)
    vector = tag_sink()
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)
    timer = QtCore.QTimer()
    QtCore.QTimer.connect(timer, QtCore.SIGNAL("timeout()"), qapp, Qt.SLOT('quit()'))
    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    umbral = float(sys.argv[1])
    print("El umbral es " + sys.argv[1])
    n_id = []
    pos = []

    for i in range(100):
        tb = top_block_cls(vector, umbral)
        n_id.append(vector.get_N_id())
        pos.append(vector.get_pos())
        vector.reset()
        tb.start()
        #tb.show()
        timer.start(600)
        qapp.exec_()


    f=open("nid"+sys.argv[1] +"-"+sys.argv[2]+ ".txt", "w")
    for nid in n_id:
        f.write("%i\n" % nid)

    f.close()

    f=open("pos"+sys.argv[1]+"-"+sys.argv[2]+".txt", "w")
    for i in pos:
        f.write("%i\n" % i)

    f.close()


if __name__ == '__main__':
    main()
