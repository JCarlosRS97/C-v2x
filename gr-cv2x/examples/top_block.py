#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Fri May  3 20:14:14 2019
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
from gnuradio import fft
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from lte_ssss_sync import lte_ssss_sync  # grc-generated hier_block
from ltev_psss_sync import ltev_psss_sync  # grc-generated hier_block
from optparse import OptionParser
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
        
        
          
        self.ltev_psss_sync_0 = ltev_psss_sync(
            fft_len=fft_len,
            syncPeriod=syncPeriod,
        )
        self.lte_ssss_sync_0 = lte_ssss_sync(
            fft_len=fft_len,
            syncPeriod=syncPeriod,
        )
        self.fft_vxx_0 = fft.fft_vcc(fft_len, False, (), True, 1)
        self.cv2x_slss_generator_0 = cv2x.slss_generator(120, 0, 0, syncPeriod, fft_len)
        self.cv2x_rough_symbol_sync_cc_0 = cv2x.rough_symbol_sync_cc(fft_len, 1, SubcarrierBW, self.sig)
        self.cv2x_ofdm_cyclic_prefixer_0 = cv2x.ofdm_cyclic_prefixer(fft_len, (int(160.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len), int(144.0/2048*fft_len)), 0, '')
        self.blocks_vector_sink_x_0 = blocks.vector_sink_c(1)
        self.blocks_throttle_1 = blocks.throttle(gr.sizeof_gr_complex*1, 100,True)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*fft_len, samp_rate/fft_len,True)
        self.blocks_stream_to_vector_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, fft_len)
        self.blocks_message_debug_0 = blocks.message_debug()
        self.blocks_head_0_0_0 = blocks.head(gr.sizeof_gr_complex*1, 3840*100)
        self.blocks_head_0_0 = blocks.head(gr.sizeof_gr_complex*1, 3840*100)
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_gr_complex*1, '/home/carlos/Escritorio/random.dat', True)
        self.blocks_file_sink_0_0_0 = blocks.file_sink(gr.sizeof_gr_complex*1, '/home/carlos/Escritorio/salida2.dat', False)
        self.blocks_file_sink_0_0_0.set_unbuffered(False)
        self.blocks_file_sink_0_0 = blocks.file_sink(gr.sizeof_gr_complex*1, '/home/carlos/Escritorio/salida.dat', False)
        self.blocks_file_sink_0_0.set_unbuffered(False)
        self.blocks_delay_0 = blocks.delay(gr.sizeof_gr_complex*1, 21)
        self.blocks_add_xx_1 = blocks.add_vcc(fft_len)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_const_source_x_0 = analog.sig_source_c(0, analog.GR_CONST_WAVE, 0, 0, 0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.lte_ssss_sync_0, 'SLSSID'), (self.blocks_message_debug_0, 'print'))    
        self.connect((self.analog_const_source_x_0, 0), (self.blocks_add_xx_0, 0))    
        self.connect((self.blocks_add_xx_0, 0), (self.cv2x_rough_symbol_sync_cc_0, 0))    
        self.connect((self.blocks_add_xx_1, 0), (self.blocks_throttle_0, 0))    
        self.connect((self.blocks_delay_0, 0), (self.blocks_add_xx_0, 1))    
        self.connect((self.blocks_file_source_0, 0), (self.blocks_stream_to_vector_0, 0))    
        self.connect((self.blocks_head_0_0, 0), (self.blocks_file_sink_0_0, 0))    
        self.connect((self.blocks_head_0_0_0, 0), (self.blocks_file_sink_0_0_0, 0))    
        self.connect((self.blocks_stream_to_vector_0, 0), (self.blocks_add_xx_1, 1))    
        self.connect((self.blocks_throttle_0, 0), (self.fft_vxx_0, 0))    
        self.connect((self.blocks_throttle_1, 0), (self.blocks_vector_sink_x_0, 0))    
        self.connect((self.cv2x_ofdm_cyclic_prefixer_0, 0), (self.blocks_delay_0, 0))    
        self.connect((self.cv2x_rough_symbol_sync_cc_0, 0), (self.blocks_head_0_0_0, 0))    
        self.connect((self.cv2x_rough_symbol_sync_cc_0, 0), (self.ltev_psss_sync_0, 0))    
        self.connect((self.cv2x_slss_generator_0, 0), (self.blocks_add_xx_1, 0))    
        self.connect((self.fft_vxx_0, 0), (self.cv2x_ofdm_cyclic_prefixer_0, 0))    
        self.connect((self.lte_ssss_sync_0, 0), (self.qtgui_sink_x_0, 0))    
        self.connect((self.ltev_psss_sync_0, 0), (self.blocks_head_0_0, 0))    
        self.connect((self.ltev_psss_sync_0, 0), (self.lte_ssss_sync_0, 0))    
        self.connect((self.sig, 0), (self.blocks_throttle_1, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len
        self.set_samp_rate(self.SubcarrierBW*self.fft_len)
        self.ltev_psss_sync_0.set_fft_len(self.fft_len)
        self.lte_ssss_sync_0.set_fft_len(self.fft_len)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate/self.fft_len)

    def get_SubcarrierBW(self):
        return self.SubcarrierBW

    def set_SubcarrierBW(self, SubcarrierBW):
        self.SubcarrierBW = SubcarrierBW
        self.set_samp_rate(self.SubcarrierBW*self.fft_len)

    def get_syncPeriod(self):
        return self.syncPeriod

    def set_syncPeriod(self, syncPeriod):
        self.syncPeriod = syncPeriod
        self.ltev_psss_sync_0.set_syncPeriod(self.syncPeriod)
        self.lte_ssss_sync_0.set_syncPeriod(self.syncPeriod)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.sig.set_sampling_freq(self.samp_rate)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate/self.fft_len)


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
