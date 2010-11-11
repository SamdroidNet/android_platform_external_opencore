# Get the current local path as the first operation
LOCAL_PATH := $(call get_makefile_dir)

# Clear out the variables used in the local makefiles
include $(MK)/clear.mk

TARGET := libomx_wma_ss_component_lib


XINCDIRS += \
        ../../../../../extern_libs_v2/khronos/openmax/include \
        ../../../../omx/omx_baseclass/include

XCXXFLAGS += $(FLAG_COMPILE_WARNINGS_AS_ERRORS)

OPTIMIZE_FOR_PERFORMANCE_OVER_SIZE := true

SRCDIR := ../../src
INCSRCDIR := ../../include

SRCS := pxm_dec.cpp \
	omx_pxm_component.cpp \
	pxm_timestamp.cpp


HDRS := pxm_dec.h \
	omx_pxm_component.h \
	pxm_timestamp.h


include $(MK)/library.mk

