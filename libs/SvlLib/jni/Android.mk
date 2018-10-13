LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := svlLib

 ifeq ($(APP_OPTIM),debug)
  APP_CFLAGS := -O0 -g $(APP_CFLAGS)
  LOCAL_CPPFLAGS:=-O0 -g $(LOCAL_CPPFLAGS)
else
  APP_CFLAGS := -O2 -DNDEBUG -g $(APP_CFLAGS)
  LOCAL_CPPFLAGS := -O2 -DNDEBUG -g $(LOCAL_CPPFLAGS)
endif
LOCAL_CFLAGS += -D__MOBILE__
LOCAL_CPPFLAGS += -D__MOBILE__
LOCAL_CFLAGS += -D__ANDROID__
LOCAL_CPPFLAGS += -D__ANDROID__
#IOSTODO 为IOS没有完全移植暂时定义
LOCAL_CFLAGS += -D__IOSTODO__  -D_7ZIP_ST
LOCAL_CPPFLAGS += -D__IOSTODO__ -D_7ZIP_ST
LOCAL_LDLIBS    := -llog -ldl

LOCAL_C_INCLUDES += $(LOCAL_PATH)/svl2/3rdParty/7Zip
LOCAL_C_INCLUDES += $(LOCAL_PATH)/svl2/3rdParty/LZ4
LOCAL_C_INCLUDES += $(LOCAL_PATH)/svl2/3rdParty/AES

#FILE_LIST += $(wildcard $(LOCAL_PATH)/svl2/3rdParty/7Zip/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/svl2/3rdParty/LZ4/*.c)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=svl2/3rdParty/7Zip/7zAlloc.c \
	svl2/3rdParty/7Zip/7zBuf.c \
	svl2/3rdParty/7Zip/7zBuf2.c \
	svl2/3rdParty/7Zip/7zCrc.c \
	svl2/3rdParty/7Zip/7zCrcOpt.c \
	svl2/3rdParty/7Zip/7zDec.c \
	svl2/3rdParty/7Zip/7zIn.c \
	svl2/3rdParty/7Zip/CpuArch.c \
	svl2/3rdParty/7Zip/LzmaDec.c \
	svl2/3rdParty/7Zip/Lzma2Dec.c \
	svl2/3rdParty/7Zip/Bra.c \
	svl2/3rdParty/7Zip/Bra86.c \
	svl2/3rdParty/7Zip/Bcj2.c \
	svl2/3rdParty/7Zip/Ppmd7.c \
	svl2/3rdParty/7Zip/Ppmd7Dec.c \
	svl2/3rdParty/7Zip/7zFile.c \
	svl2/3rdParty/7Zip/7zStream.c \
	svl2/3rdParty/7Zip/Alloc.c \
	svl2/3rdParty/7Zip/LzFind.c \
	svl2/3rdParty/7Zip/LzmaEnc.c \
	svl2/3rdParty/7Zip/LzmaLib.c \
	svl2/3rdParty/7Zip/LzFindMt.c \
	svl2/3rdParty/AES/AES.cpp \
	
LOCAL_SRC_FILES += 	svl2/DataManager.cpp\
	svl2/StdAfx.cpp\
	svl2/Stk_AssemblyFeature.cpp\
	svl2/Stk_Camera.cpp\
	svl2/Stk_ComponentFeature.cpp\
	svl2/Stk_CompositeText.cpp\
	svl2/Stk_CurveEntity.cpp\
	svl2/Stk_DocumentManager.cpp\
	svl2/Stk_Entity.cpp\
	svl2/Stk_Feat.cpp\
	svl2/Stk_Feat_Hole.cpp\
	svl2/Stk_Feat_Pipe.cpp\
	svl2/Stk_File.cpp\
	svl2/Stk_GeoAttribute.cpp\
	svl2/Stk_GroupFeature.cpp\
	svl2/Stk_Information.cpp\
	svl2/Stk_Instance.cpp\
	svl2/Stk_LayerInfosSet.cpp\
	svl2/Stk_LineSetEntity.cpp\
	svl2/Stk_Manager.cpp\
	svl2/Stk_MeshEntity.cpp\
	svl2/Stk_MetaDataEntity.cpp\
	svl2/Stk_Node.cpp\
	svl2/Stk_NodeConnector.cpp\
	svl2/Stk_Object.cpp\
	svl2/Stk_OutFrame.cpp\
	svl2/Stk_PatternFeature.cpp\
	svl2/Stk_Platform.cpp\
	svl2/Stk_PMIEntity.cpp\
	svl2/Stk_Preview.cpp\
	svl2/Stk_ProtoType.cpp\
	svl2/Stk_SpacialPoint.cpp\
	svl2/Stk_SurfaceEntity.cpp\
	svl2/Stk_TopoEntity.cpp\
	svl2/Stk_UserData.cpp\
	svl2/Stk_Utility.cpp\
	svl2/Stk_View.cpp \
	svl2/Stk_InsAttribute.cpp \
	svl2/Stk_Feat_MangeticLine.cpp \
	svl2/Stk_Picture.cpp \
	svl2/Stk_AESHelper.cpp \
	svl2/Stk_Feat_Arrow.cpp \
	svl2/Stk_Render.cpp \
	svl2/Stk_Texture.cpp \
	svl2/Stk_Feat_Mark.cpp \
	svl2/Stk_Listener.cpp \
	svl2/Stk_VirtualBomItemInfo.cpp \
	svl2/Stk_Listener.cpp \
 
#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
