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
            if (abs((pmt.to_long(tag.value)%(256+18))-21)<5):
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
            umbralPSSS=30,
        )
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_multiply_xx_0_0 = blocks.multiply_vcc(1)
        self.blocks_message_debug_0 = blocks.message_debug()
        self.blocks_delay_0 = blocks.delay(gr.sizeof_gr_complex*1, 21)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_sig_source_x_0_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, 0, 1, 0)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 0.125, 0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.ltev_rx_sync_0, 'out'), (self.blocks_message_debug_0, 'print'))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.analog_sig_source_x_0_0, 0), (self.blocks_multiply_xx_0_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_xx_0_0, 1))
        self.connect((self.blocks_delay_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_multiply_xx_0_0, 0), (self.ltev_rx_sync_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_delay_0, 0))
        self.connect((self.ltev_rx_sync_0, 0), (self.qtgui_sink_x_0, 0))
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

    for i in range(100):
        tb = top_block_cls(vector, umbral)
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
