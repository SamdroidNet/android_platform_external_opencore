LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_WHOLE_STATIC_LIBRARIES := \
	libomx_amr_component_lib \
 	libpvamrwbdecoder

ifneq ($(SAMSUNG_AMR_DECODER), true)
LOCAL_WHOLE_STATIC_LIBRARIES += libpvdecoder_gsmamr
endif

LOCAL_MODULE := libomx_amrdec_sharedlibrary

LOCAL_PRELINK_MODULE := false

-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

LOCAL_SHARED_LIBRARIES +=   libomx_sharedlibrary libopencore_common

ifeq ($(SAMSUNG_AMR_DECODER), true)
LOCAL_SHARED_LIBRARIES +=  libsAMRNB libsAMRWB
endif



include $(BUILD_SHARED_LIBRARY)

ifeq ($(SAMSUNG_AMR_DECODER), true)
include   $(PV_TOP)/codecs_v2/omx/omx_amrdec_ss/Android.mk
else
include   $(PV_TOP)/codecs_v2/omx/omx_amr/Android.mk
endif

include   $(PV_TOP)/codecs_v2/audio/gsm_amr/amr_nb/dec/Android.mk
include   $(PV_TOP)/codecs_v2/audio/gsm_amr/amr_wb/dec/Android.mk

ifeq ($(SAMSUNG_AMR_DECODER), true)
include   $(PV_TOP)/codecs_v2/audio/amrdec_ss/Android.mk
include   $(PV_TOP)/codecs_v2/audio/amr_wbdec_ss/Android.mk
endif

