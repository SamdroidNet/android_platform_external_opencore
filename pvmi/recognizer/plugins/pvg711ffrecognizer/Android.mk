LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/pvg711ffrec_factory.cpp \
	src/pvg711ffrec_plugin.cpp



LOCAL_MODULE := libpvg711ffrecognizer

LOCAL_CFLAGS :=   $(PV_CFLAGS)


LOCAL_C_INCLUDES := \
	$(PV_TOP)//pvmi/recognizer/plugins/pvg711ffrecognizer/include \
	$(PV_TOP)//pvmi/recognizer/plugins/pvg711ffrecognizer/src \
	$(PV_TOP)/pvmi/recognizer/include \
	$(PV_INCLUDES) 


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/pvg711ffrec_factory.h

include $(BUILD_STATIC_LIBRARY)

