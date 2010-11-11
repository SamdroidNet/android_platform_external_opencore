LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/evrcfileparser.cpp



LOCAL_MODULE := libpvevrcparser

LOCAL_CFLAGS :=   $(PV_CFLAGS)


LOCAL_C_INCLUDES := \
	$(PV_TOP)//fileformats/evrc/parser/include \
	$(PV_TOP)//fileformats/evrc/parser/src \
	$(PV_INCLUDES) 


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/evrcfileparser.h

include $(BUILD_STATIC_LIBRARY)

