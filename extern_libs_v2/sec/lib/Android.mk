LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS) 

LOCAL_PRELINK_MODULE := false

LOCAL_PREBUILT_LIBS := \
	libsmp4vdomxoc.so \
	libs263domxoc.so \
	libs264domxoc.so \
	libsvc1domxoc.so \
	libsmp4veomxoc.so \
	libs263eomxoc.so \
	libs264eomxoc.so \
	libsavidreg.so \
	libsavidocn.so \
	libswmfdreg.so \
	libswmfdocn.so \
	libsvidmiooc.so \
	libsmp4fmocn.so \
	libsthmb.so \
	libsimgp.so \

include $(BUILD_MULTI_PREBUILT) 

