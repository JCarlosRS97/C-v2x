#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/python
export PATH=/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/build/python:$PATH
export LD_LIBRARY_PATH=/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/build/swig:$PYTHONPATH
/usr/bin/python2 /home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/python/qa_subframe_to_symbol_vector.py 
