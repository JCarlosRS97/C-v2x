# Install script for directory: /home/jcrs/Escritorio/Bloques/gr-cv2x/grc

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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks" TYPE FILE FILES
    "/home/jcrs/Escritorio/Bloques/gr-cv2x/grc/cv2x_subframe_to_symbol_vector.xml"
    "/home/jcrs/Escritorio/Bloques/gr-cv2x/grc/cv2x_slss_generator.xml"
    "/home/jcrs/Escritorio/Bloques/gr-cv2x/grc/cv2x_pss_symbol_selector_cvc.xml"
    "/home/jcrs/Escritorio/Bloques/gr-cv2x/grc/cv2x_extract_subcarriers_vcvc.xml"
    "/home/jcrs/Escritorio/Bloques/gr-cv2x/grc/cv2x_rough_symbol_sync_cc.xml"
    "/home/jcrs/Escritorio/Bloques/gr-cv2x/grc/cv2x_pss_calculator_vcm.xml"
    "/home/jcrs/Escritorio/Bloques/gr-cv2x/grc/cv2x_ofdm_cyclic_prefixer.xml"
    )
endif()

