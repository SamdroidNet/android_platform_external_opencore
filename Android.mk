ifneq ($(BUILD_WITHOUT_PV),true)
LOCAL_PATH := $(call my-dir)
PV_TOP := $(LOCAL_PATH)
include $(CLEAR_VARS)

#PV_CFLAGS := -Wno-non-virtual-dtor -DENABLE_MEMORY_PLAYBACK -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -DUSE_CML2_CONFIG
PV_CFLAGS := -Wno-non-virtual-dtor -DENABLE_SHAREDFD_PLAYBACK -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -DUSE_CML2_CONFIG

ifeq ($(ENABLE_PV_LOGGING),1)
 PV_CFLAGS += -DPVLOGGER_INST_LEVEL=5
endif

ifeq ($(TARGET_ARCH),arm)
  PV_CFLAGS += -DPV_ARM_GCC_V5
endif

include $(CLEAR_VARS)

FORMAT := android

PV_COPY_HEADERS_TO := libpv

PV_INCLUDES := \
	$(PV_TOP)/android \
	$(PV_TOP)/extern_libs_v2/khronos/openmax/include \
	$(PV_TOP)/engines/common/include \
	$(PV_TOP)/engines/player/config/android \
	$(PV_TOP)/engines/player/include \
	$(PV_TOP)/nodes/pvmediaoutputnode/include \
	$(PV_TOP)/nodes/pvdownloadmanagernode/config/opencore \
	$(PV_TOP)/pvmi/pvmf/include \
	$(PV_TOP)/fileformats/mp4/parser/config/opencore \
	$(PV_TOP)/oscl/oscl/config/android \
	$(PV_TOP)/oscl/oscl/config/shared \
	$(PV_TOP)/engines/author/include \
	$(PV_TOP)/android/drm/oma1/src \
	$(PV_TOP)/build_config/opencore_dynamic \
	$(PV_TOP)/codecs_v2/audio/gsm_amr/amr_nb/enc/include \
	$(TARGET_OUT_HEADERS)/$(PV_COPY_HEADERS_TO)

$(call add-prebuilt-files, ETC, pvplayer.cfg)

include $(PV_TOP)/extern_libs_v2/sec/Android.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_opencore_common.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_opencore_author.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_opencore_player.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_opencore_2way.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_sharedlibrary.mk
#include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_avcdec_sharedlibrary.mk
#include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_m4vdec_sharedlibrary.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_aacdec_sharedlibrary.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_amrdec_sharedlibrary.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_mp3dec_sharedlibrary.mk
ifeq ($(SAMSUNG_WMA_DECODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_wmadec_sharedlibrary.mk
endif
ifeq ($(SAMSUNG_AC3_DECODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_ac3dec_sharedlibrary.mk
endif
ifeq ($(SAMSUNG_G711_DECODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_g711dec_sharedlibrary.mk
endif
ifeq ($(SAMSUNG_EVRC_DECODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_evrcdec_sharedlibrary.mk
endif
ifeq ($(SAMSUNG_QCELP_DECODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_qcelpdec_sharedlibrary.mk
endif
ifeq ($(SAMSUNG_G729_DECODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_g729dec_sharedlibrary.mk
endif
#include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_avcenc_sharedlibrary.mk
#include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_m4venc_sharedlibrary.mk
ifeq ($(SAMSUNG_AMR_NB_ENCODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_amrenc_sharedlibrary.mk
endif
ifeq ($(SAMSUNG_G711_ENCODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_g711enc_sharedlibrary.mk
endif
ifeq ($(SAMSUNG_EVRC_ENCODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_evrcenc_sharedlibrary.mk
endif
ifeq ($(SAMSUNG_G729_ENCODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_g729enc_sharedlibrary.mk
endif
ifeq ($(SAMSUNG_MP3_ENCODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_mp3enc_sharedlibrary.mk
endif
ifeq ($(SAMSUNG_QCELP8_ENCODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_qcelpenc_sharedlibrary.mk
endif
ifeq ($(SAMSUNG_QCELP13_ENCODER), true)
include $(PV_TOP)/build_config/opencore_dynamic/Android_omx_qcelpenc_sharedlibrary.mk
endif
include $(PV_TOP)/build_config/opencore_dynamic/Android_opencore_net_support.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_opencore_downloadreg.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_opencore_download.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_opencore_rtspreg.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_opencore_rtsp.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_opencore_mp4localreg.mk
include $(PV_TOP)/build_config/opencore_dynamic/Android_opencore_mp4local.mk
include $(PV_TOP)/oscl/unit_test/Android.mk
include $(PV_TOP)/engines/player/test/Android.mk
include $(PV_TOP)/engines/author/test/Android.mk
include $(PV_TOP)/engines/2way/test/Android.mk
endif
