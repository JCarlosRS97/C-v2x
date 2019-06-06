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
import numpy as np

class tag_sink(gr.sync_block):
    def __init__(self):
        gr.sync_block.__init__(
            self,
            name = "tag sink",
            in_sig = [np.complex64],
            out_sig = None,
        )
        self.key = None
        self.bien = 0
        self.total = 0

    def work(self, input_items, output_items):
        num_input_items = len(input_items[0])

        #put code here to process the input items...

        #print all the tags received in this work call
        nread = self.nitems_read(0)
        tags = self.get_tags_in_range(0, nread, nread+num_input_items)
        for tag in tags:
            self.total = self.total + 1
            if (abs((pmt.to_long(tag.value)%(256+18))-25)<5):
                self.bien = self.bien + 1

        return num_input_items

    def getBien(self):
        return self.bien

    def getTotal(self):
        return self.total

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
        self.syncPeriod = syncPeriod = 2
        self.samp_rate = samp_rate = SubcarrierBW*fft_len

        ##################################################
        # Blocks
        ##################################################
        sig = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, 0, 1, 0)

        fft_vxx_0 = fft.fft_vcc(fft_len, False, (), True, 1)
        cv2x_slss_generator_0 = cv2x.slss_generator(120, 0, 0, syncPeriod, fft_len)
        cv2x_rough_symbol_sync_cc_0 = cv2x.rough_symbol_sync_cc(fft_len, SubcarrierBW, sig, umbral)
        cv2x_lte_cyclic_prefixer_vcc_0 = cv2x.lte_cyclic_prefixer_vcc(fft_len)
        blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*fft_len, samp_rate/fft_len,True)
        blocks_multiply_xx_0_0 = blocks.multiply_vcc(1)
        blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        blocks_file_source_0 = blocks.file_source(gr.sizeof_gr_complex*256, '../random.dat', True)
        blocks_delay_0 = blocks.delay(gr.sizeof_gr_complex*1, 21)
        blocks_add_xx_1 = blocks.add_vcc(fft_len)
        blocks_add_xx_0 = blocks.add_vcc(1)
        analog_sig_source_x_0_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, -1000, 1, 0)
        analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 5, 0)



        ##################################################
        # Connections
        ##################################################
        self.connect((analog_noise_source_x_0, 0), (blocks_add_xx_0, 0))
        self.connect((analog_sig_source_x_0_0, 0), (blocks_multiply_xx_0_0, 0))
        self.connect((blocks_add_xx_0, 0), (blocks_multiply_xx_0_0, 1))
        self.connect((blocks_add_xx_1, 0), (blocks_throttle_0, 0))
        self.connect((blocks_delay_0, 0), (blocks_add_xx_0, 1))
        self.connect((blocks_file_source_0, 0), (blocks_add_xx_1, 1))
        self.connect((blocks_multiply_xx_0_0, 0), (cv2x_rough_symbol_sync_cc_0, 0))
        self.connect((blocks_throttle_0, 0), (fft_vxx_0, 0))
        self.connect((cv2x_lte_cyclic_prefixer_vcc_0, 0), (blocks_delay_0, 0))
        self.connect((cv2x_rough_symbol_sync_cc_0, 0), (blocks_multiply_xx_0, 0))
        self.connect((cv2x_slss_generator_0, 0), (blocks_add_xx_1, 0))
        self.connect((fft_vxx_0, 0), (cv2x_lte_cyclic_prefixer_vcc_0, 0))
        self.connect((sig, 0), (blocks_multiply_xx_0, 1))
        self.connect((blocks_multiply_xx_0, 0),(vector, 0) )

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len
        self.set_samp_rate(self.SubcarrierBW*self.fft_len)
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

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.sig.set_sampling_freq(self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate/self.fft_len)
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
    umbral = float(sys.argv[1])/20


    tb = top_block_cls(vector,umbral)
    tb.start()
    #tb.show()
    timer.start(300)


    qapp.exec_()

    # for i in range(100):
    #     tb = top_block_cls(vector,umbral)
    tb.start()
    #tb.show()
    timer.start(400)


    qapp.exec_()


    print vector.getBien()

    print vector.getTotal()

    res = vector.getBien()
    f=open("resultados"+ sys.argv[1] + ".txt", "r")
    lista = f.read().split()
    acumulado = int(lista[0]) + res
    print acumulado
    f.close()
    f=open("resultados"+sys.argv[1]+".txt", "w")
    f.write("%i\n" % acumulado)
    f.close()

    res = vector.getTotal()
    f=open("errores"+ sys.argv[1] + ".txt", "r")
    lista = f.read().split()
    acumulado = int(lista[0]) + res
    f.close()
    f=open("errores"+sys.argv[1]+".txt", "w")
    f.write("%i\n" % acumulado)


if __name__ == '__main__':
    main()
