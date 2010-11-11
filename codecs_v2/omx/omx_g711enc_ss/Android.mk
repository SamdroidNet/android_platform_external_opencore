LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/g711_enc.cpp \
 	src/omx_g711enc_component.cpp


LOCAL_MODULE := libomx_g711enc_component_lib

LOCAL_CFLAGS :=  $(PV_CFLAGS)

LOCAL_ARM_MODE := arm

LOCAL_STATIC_LIBRARIES :=

LOCAL_SHARED_LIBRARIES :=

LOCAL_C_INCLUDES := \
	$(PV_TOP)/codecs_v2/omx/omx_amrenc/src \
 	$(PV_TOP)/codecs_v2/omx/omx_amrenc/include \
 	$(PV_TOP)/extern_libs_v2/khronos/openmax/include \
 	$(PV_INCLUDES)

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
	include/g711_enc.h \
 	include/omx_g711enc_component.h \
 	include/sG711.h

include $(BUILD_STATIC_LIBRARY)
