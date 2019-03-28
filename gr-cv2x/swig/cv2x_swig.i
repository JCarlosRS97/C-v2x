/* -*- c++ -*- */

#define CV2X_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "cv2x_swig_doc.i"

%{
#include "cv2x/slss_generator.h"
#include "cv2x/subframe_to_symbol_vector.h"
%}


%include "cv2x/slss_generator.h"
GR_SWIG_BLOCK_MAGIC2(cv2x, slss_generator);

%include "cv2x/subframe_to_symbol_vector.h"
GR_SWIG_BLOCK_MAGIC2(cv2x, subframe_to_symbol_vector);
