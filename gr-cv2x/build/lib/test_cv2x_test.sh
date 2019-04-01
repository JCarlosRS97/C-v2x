#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/jcrs/Escritorio/Bloques/gr-cv2x/lib
export PATH=/home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib:$PATH
export LD_LIBRARY_PATH=/home/jcrs/Escritorio/Bloques/gr-cv2x/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-cv2x 
