LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/pvmf_g711ffparser_node.cpp \
	src/pvmf_g711ffparser_port.cpp \
	src/pvmf_g711ffparser_factory.cpp



LOCAL_MODULE := libpvg711ffparsernode

LOCAL_CFLAGS :=   $(PV_CFLAGS)



LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := 

LOCAL_C_INCLUDES := \
	$(PV_TOP)/nodes/pvg711ffparsernode/src \
 	$(PV_TOP)/nodes/pvg711ffparsernode/include \
 	$(PV_INCLUDES)


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/pvmf_g711ffparser_factory.h \
	include/pvmf_g711ffparser_registry_factory.h \
 	include/pvmf_g711ffparser_events.h

include $(BUILD_STATIC_LIBRARY)

