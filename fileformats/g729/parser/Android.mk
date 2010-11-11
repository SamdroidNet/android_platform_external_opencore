LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/g729fileparser.cpp



LOCAL_MODULE := libpvg729parser

LOCAL_CFLAGS :=   $(PV_CFLAGS)


LOCAL_C_INCLUDES := \
	$(PV_TOP)//fileformats/g729/parser/include \
	$(PV_TOP)//fileformats/g729/parser/src \
	$(PV_INCLUDES) 


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/g729fileparser.h

include $(BUILD_STATIC_LIBRARY)

