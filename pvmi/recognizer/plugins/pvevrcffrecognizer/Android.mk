LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/pvevrcffrec_factory.cpp \
	src/pvevrcffrec_plugin.cpp



LOCAL_MODULE := libpvevrcffrecognizer

LOCAL_CFLAGS :=   $(PV_CFLAGS)


LOCAL_C_INCLUDES := \
	$(PV_TOP)//pvmi/recognizer/plugins/pvevrcffrecognizer/include \
	$(PV_TOP)//pvmi/recognizer/plugins/pvevrcffrecognizer/src \
	$(PV_TOP)/pvmi/recognizer/include \
	$(PV_INCLUDES) 


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/pvevrcffrec_factory.h

include $(BUILD_STATIC_LIBRARY)

