LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE	:= libSOIL

LOCAL_CFLAGS := -D__ANDROID__ 
LOCAL_CPPFLAGS := -D__ANDROID__ 
 
LOCAL_SRC_FILES :=  image_DXT.c\
                    image_helper.c\
                    SOIL.c\
                    stb_image_write.c\
                    stb_image.c\
                    stb_vorbis.c\
                    
LOCAL_LDLIBS    := -llog -lGLESv1_CM -lGLESv2 

include $(BUILD_STATIC_LIBRARY)