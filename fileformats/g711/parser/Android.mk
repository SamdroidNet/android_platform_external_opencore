LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/g711fileparser.cpp



LOCAL_MODULE := libpvg711parser

LOCAL_CFLAGS :=   $(PV_CFLAGS)


LOCAL_C_INCLUDES := \
	$(PV_TOP)//fileformats/g711/parser/include \
	$(PV_TOP)//fileformats/g711/parser/src \
	$(PV_INCLUDES) 


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/g711fileparser.h

include $(BUILD_STATIC_LIBRARY)

