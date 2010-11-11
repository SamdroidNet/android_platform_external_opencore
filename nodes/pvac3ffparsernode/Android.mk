LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/pvmf_ac3ffparser_node.cpp \
	src/pvmf_ac3ffparser_port.cpp \
	src/pvmf_ac3ffparser_factory.cpp



LOCAL_MODULE := libpvac3ffparsernode

LOCAL_CFLAGS :=   $(PV_CFLAGS)


LOCAL_C_INCLUDES := \
	$(PV_TOP)//nodes/pvac3ffparsernode/include \
	$(PV_TOP)//nodes/pvac3ffparsernode/src \
	$(PV_TOP)//nodes/pvac3ffparsernode/../common/include \
	$(PV_INCLUDES) 


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/pvmf_ac3ffparser_factory.h \
	include/pvmf_ac3ffparser_registry_factory.h \
	include/pvmf_ac3ffparser_defs.h

include $(BUILD_STATIC_LIBRARY)

