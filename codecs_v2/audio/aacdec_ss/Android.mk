LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PREBUILT_LIBS:= libsAACPlusDec.so

include $(BUILD_MULTI_PREBUILT)
