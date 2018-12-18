INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_CV2X cv2x)

FIND_PATH(
    CV2X_INCLUDE_DIRS
    NAMES cv2x/api.h
    HINTS $ENV{CV2X_DIR}/include
        ${PC_CV2X_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    CV2X_LIBRARIES
    NAMES gnuradio-cv2x
    HINTS $ENV{CV2X_DIR}/lib
        ${PC_CV2X_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CV2X DEFAULT_MSG CV2X_LIBRARIES CV2X_INCLUDE_DIRS)
MARK_AS_ADVANCED(CV2X_LIBRARIES CV2X_INCLUDE_DIRS)

