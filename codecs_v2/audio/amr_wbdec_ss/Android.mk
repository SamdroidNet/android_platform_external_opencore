LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PREBUILT_LIBS:= libsAMRWB.so

include $(BUILD_MULTI_PREBUILT)
