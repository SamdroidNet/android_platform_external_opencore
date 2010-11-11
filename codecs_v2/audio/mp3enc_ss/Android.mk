LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PREBUILT_LIBS:= libsMP3Enc.so

include $(BUILD_MULTI_PREBUILT)
