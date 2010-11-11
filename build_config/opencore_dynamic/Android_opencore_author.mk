LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_WHOLE_STATIC_LIBRARIES := \
 	libpvauthorengine \
#	libpvmp4ffcomposer \
# 	libpvmp4ffcomposernode \


LOCAL_WHOLE_STATIC_LIBRARIES +=  libandroidpvauthor

LOCAL_MODULE := libopencore_author

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	../../extern_libs_v2/sec/inc/smp4fm_oc_factory.h \
	../../extern_libs_v2/sec/inc/smp4fm_oc_clipcfg.h 

-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

LOCAL_SHARED_LIBRARIES += libopencore_common libsmp4fmocn
#LOCAL_SHARED_LIBRARIES += libopencore_common

include $(BUILD_SHARED_LIBRARY)
#include   $(PV_TOP)/fileformats/mp4/composer/Android.mk
#include   $(PV_TOP)/nodes/pvmp4ffcomposernode/Android.mk
include   $(PV_TOP)/engines/author/Android.mk
include $(PV_TOP)/android/author/Android.mk

