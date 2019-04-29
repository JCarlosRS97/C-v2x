#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Error
# Author: Juan Carlos Ruiz Sicilia
# Generated: Fri Apr 26 13:55:58 2019
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

from PyQt4 import Qt
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
import cv2x
import sys


class un_error(gr.top_block, Qt.QWidget):

    def __init__(self, fft_len=512, syncPeriod=160):
        gr.top_block.__init__(self, "Error")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Error")
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

        self.settings = Qt.QSettings("GNU Radio", "un_error")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Parameters
        ##################################################
        self.fft_len = fft_len
        self.syncPeriod = syncPeriod

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 32000

        ##################################################
        # Blocks
        ##################################################
        self.fft_vxx_0 = fft.fft_vcc(fft_len, True, (), False, 1)
        self.cv2x_ssss_tagger_cc_0 = cv2x.ssss_tagger_cc(fft_len, syncPeriod)
        self.cv2x_ssss_symbol_selector_cvc_0 = cv2x.ssss_symbol_selector_cvc(fft_len, syncPeriod)
        self.cv2x_ssss_calculator_vcm_0 = cv2x.ssss_calculator_vcm(fft_len, "N_id_2", "offset_marker", syncPeriod)
        self.cv2x_extract_subcarriers_vcvc_0 = cv2x.extract_subcarriers_vcvc(6, fft_len)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.cv2x_ssss_calculator_vcm_0, 'frame_start'), (self.cv2x_ssss_tagger_cc_0, 'frame_start'))    
        self.msg_connect((self.cv2x_ssss_calculator_vcm_0, 'cell_id'), (self, 'SLSSID'))    
        self.connect((self.cv2x_extract_subcarriers_vcvc_0, 0), (self.cv2x_ssss_calculator_vcm_0, 0))    
        self.connect((self.cv2x_ssss_symbol_selector_cvc_0, 0), (self.fft_vxx_0, 0))    
        self.connect((self.cv2x_ssss_tagger_cc_0, 0), (self, 0))    
        self.connect((self.fft_vxx_0, 0), (self.cv2x_extract_subcarriers_vcvc_0, 0))    
        self.connect((self, 0), (self.cv2x_ssss_symbol_selector_cvc_0, 0))    
        self.connect((self, 0), (self.cv2x_ssss_tagger_cc_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "un_error")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len

    def get_syncPeriod(self):
        return self.syncPeriod

    def set_syncPeriod(self, syncPeriod):
        self.syncPeriod = syncPeriod

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate


def argument_parser():
    parser = OptionParser(usage="%prog: [options]", option_class=eng_option)
    parser.add_option(
        "", "--fft-len", dest="fft_len", type="intx", default=512,
        help="Set FFT length [default=%default]")
    parser.add_option(
        "", "--syncPeriod", dest="syncPeriod", type="intx", default=160,
        help="Set Sync Period [default=%default]")
    return parser


def main(top_block_cls=un_error, options=None):
    if options is None:
        options, _ = argument_parser().parse_args()

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls(fft_len=options.fft_len, syncPeriod=options.syncPeriod)
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
