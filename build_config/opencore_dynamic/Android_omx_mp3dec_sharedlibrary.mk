LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(SAMSUNG_MP3_DECODER), true)
LOCAL_WHOLE_STATIC_LIBRARIES := \
	libomx_mp3_component_lib
else
LOCAL_WHOLE_STATIC_LIBRARIES := \
	libomx_mp3_component_lib \
	libpvmp3
endif

LOCAL_MODULE := libomx_mp3dec_sharedlibrary

LOCAL_PRELINK_MODULE := false

-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

ifeq ($(SAMSUNG_MP3_DECODER), true)
LOCAL_SHARED_LIBRARIES +=   libomx_sharedlibrary libopencore_common libsMP3Dec
else
LOCAL_SHARED_LIBRARIES +=   libomx_sharedlibrary libopencore_common 
endif


include $(BUILD_SHARED_LIBRARY)

ifeq ($(SAMSUNG_MP3_DECODER), true)
include   $(PV_TOP)/codecs_v2/omx/omx_mp3dec_ss/Android.mk
include   $(PV_TOP)/codecs_v2/audio/mp3dec_ss/Android.mk
else
include   $(PV_TOP)/codecs_v2/omx/omx_mp3/Android.mk
endif

include   $(PV_TOP)/codecs_v2/audio/mp3/dec/Android.mk