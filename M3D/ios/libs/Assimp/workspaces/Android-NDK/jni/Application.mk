ifeq ($(CC),clang)
    NDK_TOOLCHAIN_VERSION := $(CC)
    $(info "Use llvm Compiler")
endif

APP_ABI := armeabi-v7a
APP_PLATFORM := android-9
APP_STL := gnustl_static #stlport_static
APP_CPPFLAGS += -frtti 
APP_CPPFLAGS += -fexceptions