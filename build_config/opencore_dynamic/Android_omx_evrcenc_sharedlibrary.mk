LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_WHOLE_STATIC_LIBRARIES := \
	libomx_evrcenc_component_lib

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libomx_evrcenc_sharedlibrary

-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

LOCAL_SHARED_LIBRARIES +=   libomx_sharedlibrary libopencore_common libsEVRC

include $(BUILD_SHARED_LIBRARY)
include   $(PV_TOP)/codecs_v2/omx/omx_evrcenc_ss/Android.mk
#include   $(PV_TOP)/codecs_v2/audio/evrc_ss/Android.mk
