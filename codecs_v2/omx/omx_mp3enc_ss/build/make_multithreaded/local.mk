# Get the current local path as the first operation
LOCAL_PATH := $(call get_makefile_dir)

# Clear out the variables used in the local makefiles
include $(MK)/clear.mk

TARGET := omx_mp3enc_component_lib


XINCDIRS += \
        ../../../../../extern_libs_v2/khronos/openmax/include \
        ../../../../omx/omx_baseclass/include 


XCXXFLAGS += $(FLAG_COMPILE_WARNINGS_AS_ERRORS)

OPTIMIZE_FOR_PERFORMANCE_OVER_SIZE := true

SRCDIR := ../../src
INCSRCDIR := ../../include

SRCS := mp3_enc.cpp \
	omx_mp3enc_component.cpp 
	

HDRS := mp3_enc.h \
	omx_mp3enc_component.h \
	sMP3Enc.h


include $(MK)/library.mk

