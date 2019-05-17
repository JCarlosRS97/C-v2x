#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Fri May 17 13:36:03 2019
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

from PyQt4 import Qt
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from lte_ssss_sync import lte_ssss_sync  # grc-generated hier_block
from optparse import OptionParser
from pss_time_sync import pss_time_sync  # grc-generated hier_block
import cv2x
import sip


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
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
        self.syncPeriod = syncPeriod = 10
        self.samp_rate = samp_rate = SubcarrierBW*fft_len

        ##################################################
        # Blocks
        ##################################################
        self.sig = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, 0, 1, 0)
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
        
        
          
        self.pss_time_sync_0 = pss_time_sync(
            fft_len=fft_len,
            samp_rate=samp_rate,
            syncPeriod=syncPeriod,
        )
        self.lte_ssss_sync_0 = lte_ssss_sync(
            fft_len=fft_len,
            syncPeriod=syncPeriod,
        )
        self.cv2x_rough_symbol_sync_cc_0 = cv2x.rough_symbol_sync_cc(fft_len, 1, SubcarrierBW, self.sig)
        self.blocks_throttle_2 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        self.blocks_message_debug_0 = blocks.message_debug()
        self.blocks_file_source_0_0 = blocks.file_source(gr.sizeof_gr_complex*1, '/home/carlos/Escritorio/matlab/data/receptorauto.dat', False)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.lte_ssss_sync_0, 'SLSSID'), (self.blocks_message_debug_0, 'print'))    
        self.connect((self.blocks_file_source_0_0, 0), (self.blocks_throttle_2, 0))    
        self.connect((self.blocks_multiply_xx_0, 0), (self.pss_time_sync_0, 0))    
        self.connect((self.blocks_throttle_2, 0), (self.cv2x_rough_symbol_sync_cc_0, 0))    
        self.connect((self.cv2x_rough_symbol_sync_cc_0, 0), (self.blocks_multiply_xx_0, 0))    
        self.connect((self.lte_ssss_sync_0, 0), (self.qtgui_sink_x_0, 0))    
        self.connect((self.pss_time_sync_0, 0), (self.lte_ssss_sync_0, 0))    
        self.connect((self.sig, 0), (self.blocks_multiply_xx_0, 1))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len
        self.set_samp_rate(self.SubcarrierBW*self.fft_len)
        self.pss_time_sync_0.set_fft_len(self.fft_len)
        self.lte_ssss_sync_0.set_fft_len(self.fft_len)

    def get_SubcarrierBW(self):
        return self.SubcarrierBW

    def set_SubcarrierBW(self, SubcarrierBW):
        self.SubcarrierBW = SubcarrierBW
        self.set_samp_rate(self.SubcarrierBW*self.fft_len)

    def get_syncPeriod(self):
        return self.syncPeriod

    def set_syncPeriod(self, syncPeriod):
        self.syncPeriod = syncPeriod
        self.pss_time_sync_0.set_syncPeriod(self.syncPeriod)
        self.lte_ssss_sync_0.set_syncPeriod(self.syncPeriod)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.sig.set_sampling_freq(self.samp_rate)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)
        self.pss_time_sync_0.set_samp_rate(self.samp_rate)
        self.blocks_throttle_2.set_sample_rate(self.samp_rate)


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
