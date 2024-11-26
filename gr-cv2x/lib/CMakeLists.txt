# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR} ${FFTW3F_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS} ${FFTW3F_LIBRARY_DIRS})
list(APPEND cv2x_sources
    subframe_to_symbol_vector_impl.cc
    slss_generator_impl.cc
    correlator.cc
    pss_symbol_selector_cvc_impl.cc
    extract_subcarriers_vcvc_impl.cc
    rough_symbol_sync_cc_impl.cc
    ofdm_cyclic_prefixer_impl.cc
    pss_calculator_vcm_impl.cc
    pss_tagger_cc_impl.cc
    sync_frequency_c_impl.cc
    ssss_symbol_selector_cvc_impl.cc
    ssss_calculator_vcm_impl.cc
    ssss_tagger_cc_impl.cc
    decimator_cc_impl.cc
    psss_time_sync_impl.cc
    lte_cyclic_prefixer_vcc_impl.cc
    pss_time_sim_impl.cc )

set(cv2x_sources "${cv2x_sources}" PARENT_SCOPE)
if(NOT cv2x_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT cv2x_sources)

add_library(gnuradio-cv2x SHARED ${cv2x_sources})
target_link_libraries(gnuradio-cv2x ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES}   ${FFTW3F_LIBRARIES}
 )
set_target_properties(gnuradio-cv2x PROPERTIES DEFINE_SYMBOL "gnuradio_cv2x_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-cv2x PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-cv2x RUNTIME_COMPONENT "cv2x_runtime" DEVEL_COMPONENT "cv2x_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})
list(APPEND test_cv2x_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_cv2x.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_cv2x.cc
    )

add_executable(test-cv2x ${test_cv2x_sources})

target_link_libraries(
  test-cv2x
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  ${FFTW3F_LIBRARIES}
  gnuradio-cv2x
)

GR_ADD_TEST(test_cv2x test-cv2x)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")
