LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_WHOLE_STATIC_LIBRARIES := \
	libomx_wma_ss_component_lib

LOCAL_MODULE := libomx_wma_ssdec_sharedlibrary

LOCAL_PRELINK_MODULE := false
-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

LOCAL_SHARED_LIBRARIES += libomx_sharedlibrary libopencore_common libsWMA9Dec

include $(BUILD_SHARED_LIBRARY)
include   $(PV_TOP)/codecs_v2/omx/omx_wmadec_ss/Android.mk
include   $(PV_TOP)/codecs_v2/audio/wmadec_ss/Android.mk
