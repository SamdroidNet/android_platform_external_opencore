LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/ac3fileparser.cpp



LOCAL_MODULE := libpvac3parser

LOCAL_CFLAGS :=   $(PV_CFLAGS)


LOCAL_C_INCLUDES := \
	$(PV_TOP)//fileformats/ac3/parser/include \
	$(PV_TOP)//fileformats/ac3/parser/src \
	$(PV_INCLUDES) 


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/ac3fileparser.h

include $(BUILD_STATIC_LIBRARY)

