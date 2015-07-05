CV_PATH = $$PWD/../3rdLibs/opencv/opencv-3.0.0/build
BOOST_PATH = $$PWD/../3rdLibs/boost/boost_1_58_0

INCLUDEPATH += $${CV_PATH}/include
INCLUDEPATH += $${BOOST_PATH}

LIB_SUFFIX = a

win32-msvc*{

  LIB_SUFFIX = lib

  LIBS += -L$${BOOST_PATH}/lib32-msvc-12.0

  CONFIG(debug, debug|release) {
    LIBS += $${CV_PATH}/x86/vc12/lib/opencv_world300d.$${LIB_SUFFIX}
  } else {    
    LIBS += $${CV_PATH}/x86/vc12/lib/opencv_world300.$${LIB_SUFFIX}
  } #config end

} #win32 end
