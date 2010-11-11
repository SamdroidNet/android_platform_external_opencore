# Get the current local path as the first operation
LOCAL_PATH := $(call get_makefile_dir)

# Clear out the variables used in the local makefiles
include $(MK)/clear.mk

TARGET := omx_ac3_component_lib


XINCDIRS += \
        ../../../../../extern_libs_v2/khronos/openmax/include \
        ../../../../omx/omx_baseclass/include \
        ../../../../audio/ac3/dec/src \
        ../../../../audio/ac3/dec/include

XCXXFLAGS += $(FLAG_COMPILE_WARNINGS_AS_ERRORS)

OPTIMIZE_FOR_PERFORMANCE_OVER_SIZE := true

SRCDIR := ../../src
INCSRCDIR := ../../include

SRCS := ac3_dec.cpp \
	omx_ac3_component.cpp \
	ac3_timestamp.cpp


HDRS := ac3_dec.h \
	omx_ac3_component.h \
	ac3_timestamp.h


include $(MK)/library.mk

