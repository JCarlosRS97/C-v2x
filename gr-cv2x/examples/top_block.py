#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Wed Apr  3 23:58:54 2019
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
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
import cv2x
import sip
import sys


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
        self.syncPeriod = syncPeriod = 4
        self.samp_rate = samp_rate = 30720000
        self.fft_len = fft_len = 512

        ##################################################
        # Blocks
        ##################################################
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
        
        
          
        self.fft_vxx_1 = fft.fft_vcc(fft_len, True, (), False, 1)
        self.fft_vxx_0 = fft.fft_vcc(fft_len, False, (), True, 1)
        self.cv2x_slss_generator_0 = cv2x.slss_generator(101, 0, 0, syncPeriod, fft_len)
        self.cv2x_rough_symbol_sync_cc_0 = cv2x.rough_symbol_sync_cc(fft_len, 1)
        self.cv2x_pss_symbol_selector_cvc_0 = cv2x.pss_symbol_selector_cvc(fft_len, syncPeriod, 0)
        self.cv2x_pss_calculator_vcm_0 = cv2x.pss_calculator_vcm(fft_len)
        self.cv2x_ofdm_cyclic_prefixer_0 = cv2x.ofdm_cyclic_prefixer(fft_len, (int(160.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len)), 0, '')
        self.cv2x_extract_subcarriers_vcvc_0 = cv2x.extract_subcarriers_vcvc(6, fft_len)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*fft_len, 100000,True)
        self.blocks_message_debug_1 = blocks.message_debug()
        self.blocks_head_0 = blocks.head(gr.sizeof_gr_complex*512, 20)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_gr_complex*512, '/home/carlos/Escritorio/memuero.dat', False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.blocks_delay_0 = blocks.delay(gr.sizeof_gr_complex*1, 21)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_const_source_x_0 = analog.sig_source_c(0, analog.GR_CONST_WAVE, 0, 0, 0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.cv2x_pss_calculator_vcm_0, 'N_id_2'), (self.blocks_message_debug_1, 'print'))    
        self.msg_connect((self.cv2x_pss_calculator_vcm_0, 'half_frame'), (self.cv2x_pss_symbol_selector_cvc_0, 'half_frame'))    
        self.msg_connect((self.cv2x_pss_calculator_vcm_0, 'lock'), (self.cv2x_pss_symbol_selector_cvc_0, 'lock'))    
        self.connect((self.analog_const_source_x_0, 0), (self.blocks_add_xx_0, 0))    
        self.connect((self.blocks_add_xx_0, 0), (self.cv2x_rough_symbol_sync_cc_0, 0))    
        self.connect((self.blocks_delay_0, 0), (self.blocks_add_xx_0, 1))    
        self.connect((self.blocks_head_0, 0), (self.blocks_file_sink_0, 0))    
        self.connect((self.blocks_throttle_0, 0), (self.fft_vxx_0, 0))    
        self.connect((self.cv2x_extract_subcarriers_vcvc_0, 0), (self.cv2x_pss_calculator_vcm_0, 0))    
        self.connect((self.cv2x_ofdm_cyclic_prefixer_0, 0), (self.blocks_delay_0, 0))    
        self.connect((self.cv2x_pss_symbol_selector_cvc_0, 0), (self.fft_vxx_1, 0))    
        self.connect((self.cv2x_rough_symbol_sync_cc_0, 0), (self.cv2x_pss_symbol_selector_cvc_0, 0))    
        self.connect((self.cv2x_rough_symbol_sync_cc_0, 0), (self.qtgui_sink_x_0, 0))    
        self.connect((self.cv2x_slss_generator_0, 0), (self.blocks_head_0, 0))    
        self.connect((self.cv2x_slss_generator_0, 0), (self.blocks_throttle_0, 0))    
        self.connect((self.fft_vxx_0, 0), (self.cv2x_ofdm_cyclic_prefixer_0, 0))    
        self.connect((self.fft_vxx_1, 0), (self.cv2x_extract_subcarriers_vcvc_0, 0))    

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
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len


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
