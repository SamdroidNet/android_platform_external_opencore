LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/ac3_dec.cpp \
	src/omx_ac3_component.cpp \
	src/ac3_timestamp.cpp

LOCAL_MODULE := libomx_ac3_component_lib

LOCAL_CFLAGS :=   $(PV_CFLAGS)

LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES := \
	$(PV_TOP)/codecs_v2/omx/omx_ac3dec_ss/include \
	$(PV_TOP)/codecs_v2/omx/omx_ac3dec_ss/src \
	$(PV_TOP)/extern_libs_v2/khronos/openmax/include \
	$(PV_TOP)/codecs_v2/omx/omx_baseclass/include \
	$(PV_INCLUDES)


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/ac3_dec.h \
	include/omx_ac3_component.h \
	include/ac3_timestamp.h

include $(BUILD_STATIC_LIBRARY)

