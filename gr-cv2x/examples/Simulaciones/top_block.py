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
from gnuradio import fft
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from lte_ssss_sync import lte_ssss_sync  # grc-generated hier_block
from optparse import OptionParser
from pss_time_sync import pss_time_sync  # grc-generated hier_block
import cv2x
import time
import pmt


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
        if (pmt.to_long(msg) == 120):
            self.cont = self.cont + 1;
    def get_cont(self):
        return self.cont;

class top_block(gr.top_block, Qt.QWidget):

    def __init__(self, message_consumer0, dev):
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
        self.blocks_vector_sink_x_0 = blocks.vector_sink_c(1)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*fft_len, samp_rate/fft_len,True)
        self.blocks_multiply_xx_0_0 = blocks.multiply_vcc(1)
        self.blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        #self.blocks_message_debug_0 = blocks.message_debug()
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_gr_complex*256, '/home/carlos/Escritorio/matlab/data/random.dat', True)
        self.blocks_delay_0 = blocks.delay(gr.sizeof_gr_complex*1, 21)
        self.blocks_add_xx_1 = blocks.add_vcc(fft_len)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_sig_source_x_0_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, -1000, 1, 0)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, dev, 2500)


        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.lte_ssss_sync_0, 'SLSSID'), (message_consumer0, 'in_port'))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.analog_sig_source_x_0_0, 0), (self.blocks_multiply_xx_0_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_xx_0_0, 1))
        self.connect((self.blocks_add_xx_1, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_delay_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_file_source_0, 0), (self.blocks_add_xx_1, 1))
        self.connect((self.blocks_multiply_xx_0, 0), (self.pss_time_sync_0, 0))
        self.connect((self.blocks_multiply_xx_0_0, 0), (self.cv2x_rough_symbol_sync_cc_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.fft_vxx_0, 0))
        self.connect((self.cv2x_lte_cyclic_prefixer_vcc_0, 0), (self.blocks_delay_0, 0))
        self.connect((self.cv2x_rough_symbol_sync_cc_0, 0), (self.blocks_multiply_xx_0, 0))
        self.connect((self.cv2x_slss_generator_0, 0), (self.blocks_add_xx_1, 0))
        self.connect((self.fft_vxx_0, 0), (self.cv2x_lte_cyclic_prefixer_vcc_0, 0))
        self.connect((self.lte_ssss_sync_0, 0), (self.blocks_vector_sink_x_0, 0))
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
        self.pss_time_sync_0.set_syncPeriod(self.syncPeriod)
        self.lte_ssss_sync_0.set_syncPeriod(self.syncPeriod)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.sig.set_sampling_freq(self.samp_rate)
        self.pss_time_sync_0.set_samp_rate(self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate/self.fft_len)
        self.analog_sig_source_x_0_0.set_sampling_freq(self.samp_rate)


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)
    message_consumer0 = message_consumer()
    timer = QtCore.QTimer()
    QtCore.QTimer.connect(timer, QtCore.SIGNAL("timeout()"), qapp, Qt.SLOT('quit()'))
    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    dev = float(sys.argv[1])

    for i in range(10):
        tb = top_block_cls(message_consumer0, dev/2)
        tb.start()
        #tb.show()
        timer.start(350)


        qapp.exec_()


    res = message_consumer0.get_cont()
    f=open("resultados"+ sys.argv[1] + ".txt", "r")
    lista = f.read().split()
    acumulado = int(lista[0]) + res
    print acumulado
    f.close()
    f=open("resultados"+sys.argv[1]+".txt", "w")
    f.write("%i\n" % acumulado)

if __name__ == '__main__':
    main()
