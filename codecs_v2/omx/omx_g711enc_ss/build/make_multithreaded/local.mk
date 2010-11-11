# Get the current local path as the first operation
LOCAL_PATH := $(call get_makefile_dir)

# Clear out the variables used in the local makefiles
include $(MK)/clear.mk

TARGET := omx_g711enc_component_lib

XCXXFLAGS += $(FLAG_COMPILE_WARNINGS_AS_ERRORS)

OPTIMIZE_FOR_PERFORMANCE_OVER_SIZE := true

XINCDIRS += \
  ../../../../../extern_libs_v2/khronos/openmax/include


SRCDIR := ../../src
INCSRCDIR := ../../include

SRCS := g711_enc.cpp \
	omx_g711enc_component.cpp


HDRS := g711_enc.h \
	omx_g711enc_component.h




include $(MK)/library.mk

