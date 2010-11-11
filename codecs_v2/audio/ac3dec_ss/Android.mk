LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PREBUILT_LIBS:= libsAC3Dec.so

include $(BUILD_MULTI_PREBUILT)
