LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/pvg729ffrec_factory.cpp \
	src/pvg729ffrec_plugin.cpp



LOCAL_MODULE := libpvg729ffrecognizer

LOCAL_CFLAGS :=   $(PV_CFLAGS)


LOCAL_C_INCLUDES := \
	$(PV_TOP)//pvmi/recognizer/plugins/pvg729ffrecognizer/include \
	$(PV_TOP)//pvmi/recognizer/plugins/pvg729ffrecognizer/src \
	$(PV_TOP)/pvmi/recognizer/include \
	$(PV_INCLUDES) 


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/pvg729ffrec_factory.h

include $(BUILD_STATIC_LIBRARY)

