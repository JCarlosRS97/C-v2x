#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/jcrs/Escritorio/Bloques/gr-cv2x/python
export PATH=/home/jcrs/Escritorio/Bloques/gr-cv2x/build/python:$PATH
export LD_LIBRARY_PATH=/home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/jcrs/Escritorio/Bloques/gr-cv2x/build/swig:$PYTHONPATH
/usr/bin/python2 /home/jcrs/Escritorio/Bloques/gr-cv2x/python/qa_slss_generator.py 
