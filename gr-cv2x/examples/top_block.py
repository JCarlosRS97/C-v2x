#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Tue Apr  2 22:17:03 2019
##################################################

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt5 import Qt, QtCore
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
import cv2x
import foo
import howto
import pmt
import sys
from gnuradio import qtgui


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
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

        if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
            self.restoreGeometry(self.settings.value("geometry").toByteArray())
        else:
            self.restoreGeometry(self.settings.value("geometry", type=QtCore.QByteArray))

        ##################################################
        # Variables
        ##################################################
        self.syncPeriod = syncPeriod = 5
        self.samp_rate = samp_rate = 30720000
        self.fft_len = fft_len = 512

        ##################################################
        # Blocks
        ##################################################
        self.howto_ofdm_cyclic_prefixer_0 = howto.ofdm_cyclic_prefixer(fft_len, (int(160.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len),int(144.0/2048*fft_len)), 0, '')
        self.foo_random_periodic_msg_source_0 = foo.random_periodic_msg_source(64, 100000000, 1, True, False, 1)
        self.fft_vxx_1 = fft.fft_vcc(fft_len, True, (), False, 1)
        self.fft_vxx_0 = fft.fft_vcc(fft_len, False, (), True, 1)
        self.cv2x_slss_generator_0 = cv2x.slss_generator(101, 0, 0, syncPeriod, fft_len)
        self.cv2x_rough_symbol_sync_cc_0 = cv2x.rough_symbol_sync_cc(fft_len, 1)
        self.cv2x_pss_symbol_selector_cvc_0 = cv2x.pss_symbol_selector_cvc(fft_len, syncPeriod, 0)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*fft_len, samp_rate/fft_len,True)
        self.blocks_head_0 = blocks.head(gr.sizeof_gr_complex*fft_len, 45)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_gr_complex*fft_len, '/home/jcrs/Escritorio/Bloques/gr-cv2x/build/python/trasselector.dat', False)
        self.blocks_file_sink_0.set_unbuffered(False)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.foo_random_periodic_msg_source_0, 'out'), (self.cv2x_pss_symbol_selector_cvc_0, 'half_frame'))
        self.msg_connect((self.foo_random_periodic_msg_source_0, 'out'), (self.cv2x_pss_symbol_selector_cvc_0, 'lock'))
        self.connect((self.blocks_head_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.fft_vxx_0, 0))
        self.connect((self.cv2x_pss_symbol_selector_cvc_0, 0), (self.fft_vxx_1, 0))
        self.connect((self.cv2x_rough_symbol_sync_cc_0, 0), (self.cv2x_pss_symbol_selector_cvc_0, 0))
        self.connect((self.cv2x_slss_generator_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.fft_vxx_0, 0), (self.howto_ofdm_cyclic_prefixer_0, 0))
        self.connect((self.fft_vxx_1, 0), (self.blocks_head_0, 0))
        self.connect((self.howto_ofdm_cyclic_prefixer_0, 0), (self.cv2x_rough_symbol_sync_cc_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_syncPeriod(self):
        return self.syncPeriod

    def set_syncPeriod(self, syncPeriod):
        self.syncPeriod = syncPeriod

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate/self.fft_len)

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len
        self.blocks_throttle_0.set_sample_rate(self.samp_rate/self.fft_len)


def main(top_block_cls=top_block, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
