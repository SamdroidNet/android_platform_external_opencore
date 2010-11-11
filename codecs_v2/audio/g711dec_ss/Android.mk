LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PREBUILT_LIBS:= libsG711.so

include $(BUILD_MULTI_PREBUILT)
