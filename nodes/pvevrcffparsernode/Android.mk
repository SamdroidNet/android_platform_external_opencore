LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/pvmf_evrcffparser_node.cpp \
	src/pvmf_evrcffparser_port.cpp \
	src/pvmf_evrcffparser_factory.cpp



LOCAL_MODULE := libpvevrcffparsernode

LOCAL_CFLAGS :=   $(PV_CFLAGS)



LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := 

LOCAL_C_INCLUDES := \
	$(PV_TOP)/nodes/pvevrcffparsernode/src \
 	$(PV_TOP)/nodes/pvevrcffparsernode/include \
 	$(PV_INCLUDES)


LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := \
	include/pvmf_evrcffparser_factory.h \
	include/pvmf_evrcffparser_registry_factory.h \
 	include/pvmf_evrcffparser_events.h

include $(BUILD_STATIC_LIBRARY)

