LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_COPY_HEADERS_TO :=$(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
		inc/sthmb.h	\
		inc/scmn_audb.h	\
		inc/scmn_base.h	\
		inc/scmn_bitb.h	\
		inc/scmn_imgb.h	\
		inc/scmn_mfal.h	\
		inc/scmn_mtype.h \
		inc/scmn_mtag.h \
		inc/simgp.h \

include $(BUILD_COPY_HEADERS)

include $(PV_TOP)/extern_libs_v2/sec/lib/Android.mk
include $(PV_TOP)/codecs_v2/video/s3c_mfc/dec/Android.mk
include $(PV_TOP)/codecs_v2/video/s3c_mfc/enc/Android.mk

