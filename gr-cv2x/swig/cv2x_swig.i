/* -*- c++ -*- */

#define CV2X_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "cv2x_swig_doc.i"

%{
#include "cv2x/subframe_to_symbol_vector.h"
#include "cv2x/slss_generator.h"
#include "cv2x/pss_symbol_selector_cvc.h"
#include "cv2x/extract_subcarriers_vcvc.h"
#include "cv2x/rough_symbol_sync_cc.h"
#include "cv2x/pss_calculator_vcm.h"
%}


%include "cv2x/subframe_to_symbol_vector.h"
GR_SWIG_BLOCK_MAGIC2(cv2x, subframe_to_symbol_vector);
%include "cv2x/slss_generator.h"
GR_SWIG_BLOCK_MAGIC2(cv2x, slss_generator);
%include "cv2x/pss_symbol_selector_cvc.h"
GR_SWIG_BLOCK_MAGIC2(cv2x, pss_symbol_selector_cvc);
%include "cv2x/extract_subcarriers_vcvc.h"
GR_SWIG_BLOCK_MAGIC2(cv2x, extract_subcarriers_vcvc);
%include "cv2x/rough_symbol_sync_cc.h"
GR_SWIG_BLOCK_MAGIC2(cv2x, rough_symbol_sync_cc);
%include "cv2x/pss_calculator_vcm.h"
GR_SWIG_BLOCK_MAGIC2(cv2x, pss_calculator_vcm);
