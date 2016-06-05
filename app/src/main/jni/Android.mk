LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# OpenCV
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include /home/mayur/workspace/opencv-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_MODULE    := detectFace
LOCAL_SRC_FILES := detectFace.cpp

LOCAL_LDLIBS +=  -llog -ldl

include $(BUILD_SHARED_LIBRARY)