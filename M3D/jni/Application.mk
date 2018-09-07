# The ARMv7 is significanly faster due to the use of the hardware FPU
APP_ABI := armeabi-v7a
#APP_ABI := all
APP_OPTIM := release
APP_PLATFORM := android-8
APP_CPPFLAGS += -frtti 
APP_CPPFLAGS += -fexceptions
#APP_STL := gnustl_shared
#APP_STL := gnustl_static
APP_STL := gnustl_static
APP_CPPFLAGS += -D_GLIBCXX_USE_WCHAR_T -Wno-deprecated -fpermissive
#APP_CPPFLAGS += -fshort-wchar #强制将wchar转为2字节
#APP_CPPFLAGS += -Dsvl2
APP_MODULES := M3D
APP_CFLAGS += -Wno-error=format-security