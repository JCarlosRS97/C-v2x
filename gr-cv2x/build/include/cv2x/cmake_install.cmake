# Install script for directory: /home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cv2x" TYPE FILE FILES
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/api.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/subframe_to_symbol_vector.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/slss_generator.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/pss_symbol_selector_cvc.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/extract_subcarriers_vcvc.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/rough_symbol_sync_cc.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/ofdm_cyclic_prefixer.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/pss_calculator_vcm.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/pss_tagger_cc.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/sync_frequency_c.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/ssss_symbol_selector_cvc.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/ssss_calculator_vcm.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/ssss_tagger_cc.h"
    "/home/carlos/Escritorio/bloques/C-v2x/gr-cv2x/include/cv2x/decimator_cc.h"
    )
endif()

