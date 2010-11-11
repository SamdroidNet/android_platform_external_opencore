LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/evrc_dec.cpp \
	src/omx_evrc_component.cpp \
	src/evrc_timestamp.cpp



LOCAL_MODULE := libomx_evrc_component_lib

LOCAL_CFLAGS :=   $(PV_CFLAGS)

LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES := \
	$(PV_TOP)/codecs_v2/omx/omx_evrcdec_ss/include \
	$(PV_TOP)/codecs_v2/omx/omx_evrcdec_ss/src \
	$(PV_TOP)/extern_libs_v2/khronos/openmax/include \
	$(PV_TOP)/codecs_v2/omx/omx_baseclass/include \
	$(PV_INCLUDES)


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/evrc_dec.h \
	include/omx_evrc_component.h \
	include/evrc_timestamp.h
include $(BUILD_STATIC_LIBRARY)

