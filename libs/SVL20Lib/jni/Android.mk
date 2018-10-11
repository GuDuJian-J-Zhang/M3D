LOCAL_PATH := $(call my-dir)
LOCAL_PATH += /../../SVLLib_CPP/
include $(CLEAR_VARS)

LOCAL_MODULE    := SVLLib2

ifeq ($(APP_OPTIM),debug)
  APP_CFLAGS := -O0 -g $(APP_CFLAGS)
  LOCAL_CPPFLAGS:=-O0 -g $(LOCAL_CPPFLAGS)
else
  APP_CFLAGS := -O2 -DNDEBUG -g $(APP_CFLAGS)
  LOCAL_CPPFLAGS := -O2 -DNDEBUG -g $(LOCAL_CPPFLAGS)
endif

#NDK_TOOLCHAIN_VERSION := 4.9
#LOCAL_CPPFLAGS += -frtti -std=c++11
#NDK_TOOLCHAIN := x86-4.8

LOCAL_CFLAGS += -D__MOBILE__
LOCAL_CPPFLAGS += -D__MOBILE__
LOCAL_CFLAGS += -D__ANDROID__
LOCAL_CPPFLAGS += -D__ANDROID__
LOCAL_CFLAGS += -DSVLLIB_EXPORT
LOCAL_CPPFLAGS += -DSVLLIB_EXPORT
LOCAL_CFLAGS += -DRAPIDJSON_HAS_STDSTRING
LOCAL_CPPFLAGS += -DRAPIDJSON_HAS_STDSTRING
LOCAL_CFLAGS += -D_UNICODE
LOCAL_CPPFLAGS += -D_UNICODE
LOCAL_CFLAGS += -DUNICODE
LOCAL_CPPFLAGS += -DUNICODE

#IOSTODO ΪIOSû����ȫ��ֲ��ʱ����
LOCAL_CFLAGS += -D__IOSTODO__  -D_7ZIP_ST
LOCAL_CPPFLAGS += -D__IOSTODO__ -D_7ZIP_ST
LOCAL_LDLIBS    := -llog -ldl

LOCAL_C_INCLUDES += $(LOCAL_PATH)/SVLLib
LOCAL_C_INCLUDES += $(LOCAL_PATH)/SVLLib/3rdParty
LOCAL_C_INCLUDES += $(LOCAL_PATH)/SVLLib/3rdParty/7Zip
LOCAL_C_INCLUDES += $(LOCAL_PATH)/SVLLib/3rdParty/LZ4
LOCAL_C_INCLUDES += $(LOCAL_PATH)/SVLLib/3rdParty/AES
LOCAL_C_INCLUDES += $(LOCAL_PATH)/SVLLib/3rdParty/MD5
LOCAL_C_INCLUDES += $(LOCAL_PATH)/SVLLib/ExtractCompress
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Include/SVLLib
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Include/MD5
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Include/rapidjson


#FILE_LIST += $(wildcard $(LOCAL_PATH)/SVLLib/3rdParty/7Zip/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/SVLLib/3rdParty/LZ4/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/SVLLib/3rdParty/MD5/*.cpp)
#FILE_LIST += $(wildcard SVLLib/ExtractCompress/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES += SVLLib.cpp
LOCAL_SRC_FILES += Utility.cpp

LOCAL_SRC_FILES +=SVLLib/3rdParty/7Zip/7zAlloc.c \
	SVLLib/3rdParty/7Zip/7zBuf.c \
	SVLLib/3rdParty/7Zip/7zBuf2.c \
	SVLLib/3rdParty/7Zip/7zCrc.c \
	SVLLib/3rdParty/7Zip/7zCrcOpt.c \
	SVLLib/3rdParty/7Zip/Delta.c \
	SVLLib/3rdParty/7Zip/7zDec.c \
	SVLLib/3rdParty/7Zip/7zArcIn.c \
	SVLLib/3rdParty/7Zip/CpuArch.c \
	SVLLib/3rdParty/7Zip/LzmaDec.c \
	SVLLib/3rdParty/7Zip/Lzma2Dec.c \
	SVLLib/3rdParty/7Zip/Bra.c \
	SVLLib/3rdParty/7Zip/BraIA64.c \
	SVLLib/3rdParty/7Zip/Bra86.c \
	SVLLib/3rdParty/7Zip/Bcj2.c \
	SVLLib/3rdParty/7Zip/Ppmd7.c \
	SVLLib/3rdParty/7Zip/Ppmd7Dec.c \
	SVLLib/3rdParty/7Zip/7zFile.c \
	SVLLib/3rdParty/7Zip/7zStream.c \
	SVLLib/3rdParty/7Zip/Alloc.c \
	SVLLib/3rdParty/7Zip/LzFind.c \
	SVLLib/3rdParty/7Zip/LzmaEnc.c \
	SVLLib/3rdParty/7Zip/LzmaLib.c \
	SVLLib/3rdParty/AES/AES.cpp \
	#SVLLib/3rdParty/7Zip/LzFindMt.c \


LOCAL_SRC_FILES +=SVLLib/ChineseCode.cpp \
	SVLLib/Mutex.cpp \
	SVLLib/RapidJsonTools.cpp \
	SVLLib/RemoteClient.cpp \
	SVLLib/Stk_Accessor.cpp \
	SVLLib/Stk_API.cpp \
	SVLLib/Stk_Attribute.cpp \
	SVLLib/Stk_AttributeCollection.cpp \
	SVLLib/Stk_BaseDocument.cpp \
	SVLLib/Stk_BaseDocumentImpl.cpp \
	SVLLib/Stk_BomBlock.cpp \
	SVLLib/Stk_Camera.cpp \
	SVLLib/Stk_CommonAccessor.cpp \
	SVLLib/Stk_CompositeText.cpp \
	SVLLib/Stk_Connector.cpp \
	SVLLib/Stk_CurveEntity.cpp \
	SVLLib/Stk_Document.cpp \
	SVLLib/Stk_DocumentImpl.cpp \
	SVLLib/Stk_File.cpp \
	SVLLib/Stk_Feat.cpp \
	SVLLib/Stk_Feat_Hole.cpp \
	SVLLib/Stk_Feat_MangeticLine.cpp \
	SVLLib/Stk_Feat_Mark.cpp \
	SVLLib/Stk_Feat_Pipe.cpp \
	SVLLib/Stk_GeometryAttribute.cpp \
	SVLLib/Stk_Image.cpp \
	SVLLib/Stk_InsAttribute.cpp \
	SVLLib/Stk_Instance.cpp \
	SVLLib/Stk_LayerInfosSet.cpp \
	SVLLib/Stk_LineSet.cpp \
	SVLLib/Stk_LoadInf.cpp \
	SVLLib/Stk_Log.cpp \
	SVLLib/Stk_Material.cpp \
	SVLLib/Stk_MeshAccessor.cpp \
	SVLLib/Stk_MeshAttribute.cpp \
	SVLLib/Stk_MeshEdge.cpp \
	SVLLib/Stk_MeshFace.cpp \
	SVLLib/Stk_MeshLodBlock.cpp \
	SVLLib/Stk_MeshPrim.cpp \
	SVLLib/Stk_MetaData.cpp \
	SVLLib/Stk_MetaDataDocument.cpp \
	SVLLib/Stk_Model.cpp \
	SVLLib/Stk_ModelBlock.cpp \
	SVLLib/Stk_ModelGroup.cpp \
	SVLLib/Stk_Node.cpp \
	SVLLib/Stk_NodeImpl.cpp \
	SVLLib/Stk_Object.cpp \
	SVLLib/Stk_ObjectImpl.cpp \
	SVLLib/Stk_Part.cpp \
	SVLLib/Stk_PlaneEntity.cpp \
	SVLLib/Stk_PMI.cpp \
	SVLLib/Stk_PMIDocument.cpp \
	SVLLib/Stk_PMIEntity.cpp \
	SVLLib/Stk_ResourceManager.cpp \
	SVLLib/Stk_SpacialPoint.cpp \
	SVLLib/Stk_Texture.cpp \
	SVLLib/Stk_UnitTest.cpp \
	SVLLib/Stk_UserData.cpp \
	SVLLib/Stk_Utility.cpp \
	SVLLib/Stk_Uuid.cpp \
	SVLLib/Stk_View.cpp \
	SVLLib/Stk_ViewDocument.cpp \
	SVLLib/StringTools.cpp \
	SVLLib/StringHash.cpp \
	SVLLib/SVLLib.cpp \
	
	
	#SVLLib/SVLAccessManager.cpp \
#    SVLLib/StringHash.cpp \

LOCAL_SRC_FILES += SVLLib/ExtractCompress/Compress.cpp \
	SVLLib/ExtractCompress/CompressHelper.cpp \
	SVLLib/ExtractCompress/ExtractSVLFiles.cpp \
	SVLLib/ExtractCompress/Extract.cpp \
	SVLLib/ExtractCompress/CompressSVLFiles.cpp \
	SVLLib/ExtractCompress/GetCompressContents.cpp \
	

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)

