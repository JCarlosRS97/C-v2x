#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/lib
export PATH=/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/build/lib:$PATH
export LD_LIBRARY_PATH=/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-cv2x 
