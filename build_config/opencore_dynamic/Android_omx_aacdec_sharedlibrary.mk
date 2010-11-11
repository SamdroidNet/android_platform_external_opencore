LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(SAMSUNG_AAC_DECODER), true)
    LOCAL_WHOLE_STATIC_LIBRARIES := libomx_aac_component_lib
else
LOCAL_WHOLE_STATIC_LIBRARIES := \
	libomx_aac_component_lib \
 	libpv_aac_dec
endif

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libomx_aacdec_sharedlibrary

-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

ifeq ($(SAMSUNG_AAC_DECODER), true)
LOCAL_SHARED_LIBRARIES +=   libomx_sharedlibrary libopencore_common libsAACPlusDec
else
LOCAL_SHARED_LIBRARIES +=   libomx_sharedlibrary libopencore_common
endif

include $(BUILD_SHARED_LIBRARY)

ifeq ($(SAMSUNG_AAC_DECODER), true)
include   $(PV_TOP)/codecs_v2/omx/omx_aacdec_ss/Android.mk
else
include   $(PV_TOP)/codecs_v2/omx/omx_aac/Android.mk
endif

ifeq ($(SAMSUNG_AAC_DECODER), true)
include   $(PV_TOP)/codecs_v2/audio/aacdec_ss/Android.mk
else
include   $(PV_TOP)/codecs_v2/audio/aac/dec/Android.mk
endif

