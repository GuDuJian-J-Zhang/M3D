LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := FreetypeLib
LOCAL_CFLAGS := -D__ANDROID__ -DFT2_BUILD_LIBRARY -Wno-psabi
LOCAL_CPPFLAGS := -D__ANDROID__ -DFT2_BUILD_LIBRARY
LOCAL_C_INCLUDES += $(LOCAL_PATH)/FreeType/freetype-2.5.0.1/include \
					$(LOCAL_PATH)/FreeType/ftgles \
					$(LOCAL_PATH)/FreeType/ftgles/src \
					$(LOCAL_PATH)/FreeType/ftgles/src/iGLU-1.0.0/include

LOCAL_LDLIBS    := -lGLESv1_CM
LOCAL_SRC_FILES :=FreeType/freetype-2.5.0.1/src/base/ftsystem.c\
	FreeType/freetype-2.5.0.1/src/base/ftinit.c\
	FreeType/freetype-2.5.0.1/src/base/ftdebug.c\
	FreeType/freetype-2.5.0.1/src/base/ftbase.c\
	FreeType/freetype-2.5.0.1/src/base/ftbbox.c\
	FreeType/freetype-2.5.0.1/src/base/ftbdf.c\
	FreeType/freetype-2.5.0.1/src/base/ftbitmap.c\
	FreeType/freetype-2.5.0.1/src/base/ftcid.c\
	FreeType/freetype-2.5.0.1/src/base/ftfstype.c\
	FreeType/freetype-2.5.0.1/src/base/ftgasp.c\
	FreeType/freetype-2.5.0.1/src/base/ftglyph.c\
	FreeType/freetype-2.5.0.1/src/base/ftgxval.c\
	FreeType/freetype-2.5.0.1/src/base/ftlcdfil.c\
	FreeType/freetype-2.5.0.1/src/base/ftmm.c\
	FreeType/freetype-2.5.0.1/src/base/ftotval.c\
	FreeType/freetype-2.5.0.1/src/base/ftpatent.c\
	FreeType/freetype-2.5.0.1/src/base/ftpfr.c\
	FreeType/freetype-2.5.0.1/src/base/ftstroke.c\
	FreeType/freetype-2.5.0.1/src/base/ftsynth.c\
	FreeType/freetype-2.5.0.1/src/base/fttype1.c\
	FreeType/freetype-2.5.0.1/src/base/ftwinfnt.c\
	FreeType/freetype-2.5.0.1/src/base/ftxf86.c\
	FreeType/freetype-2.5.0.1/src/bdf/bdf.c\
	FreeType/freetype-2.5.0.1/src/bzip2/ftbzip2.c\
	FreeType/freetype-2.5.0.1/src/cache/ftcache.c\
	FreeType/freetype-2.5.0.1/src/cff/cff.c\
	FreeType/freetype-2.5.0.1/src/cid/type1cid.c\
	FreeType/freetype-2.5.0.1/src/gxvalid/gxvalid.c\
	FreeType/freetype-2.5.0.1/src/gzip/ftgzip.c\
	FreeType/freetype-2.5.0.1/src/lzw/ftlzw.c\
	FreeType/freetype-2.5.0.1/src/otvalid/otvalid.c\
	FreeType/freetype-2.5.0.1/src/pcf/pcf.c\
	FreeType/freetype-2.5.0.1/src/pfr/pfr.c\
	FreeType/freetype-2.5.0.1/src/psaux/psaux.c\
	FreeType/freetype-2.5.0.1/src/pshinter/pshinter.c\
	FreeType/freetype-2.5.0.1/src/psnames/psnames.c\
	FreeType/freetype-2.5.0.1/src/raster/raster.c\
	FreeType/freetype-2.5.0.1/src/sfnt/sfnt.c\
	FreeType/freetype-2.5.0.1/src/smooth/smooth.c\
	FreeType/freetype-2.5.0.1/src/truetype/truetype.c\
	FreeType/freetype-2.5.0.1/src/type1/type1.c\
	FreeType/freetype-2.5.0.1/src/type42/type42.c\
	FreeType/freetype-2.5.0.1/src/winfonts/winfnt.c\
	FreeType/freetype-2.5.0.1/src/autofit/autofit.c\
	FreeType/ftgles/src/FTBuffer.cpp \
	FreeType/ftgles/src/FTCharmap.cpp \
	FreeType/ftgles/src/FTContour.cpp \
	FreeType/ftgles/src/FTFace.cpp \
	FreeType/ftgles/src/FTFont/FTBitmapFont.cpp \
	FreeType/ftgles/src/FTFont/FTBufferFont.cpp \
	FreeType/ftgles/src/FTFont/FTFont.cpp \
	FreeType/ftgles/src/FTFont/FTFontGlue.cpp \
	FreeType/ftgles/src/FTFont/FTPolygonFont.cpp \
	FreeType/ftgles/src/FTFont/FTTextureFont.cpp \
	FreeType/ftgles/src/FTGL/ftglesGlue.cpp \
	FreeType/ftgles/src/FTGlyph/FTBufferGlyph.cpp \
	FreeType/ftgles/src/FTGlyph/FTGlyph.cpp \
	FreeType/ftgles/src/FTGlyph/FTGlyphGlue.cpp \
	FreeType/ftgles/src/FTGL/ftgles2Glue.cpp \
	FreeType/ftgles/src/FTGlyph/FTPixmapGlyph.cpp \
	FreeType/ftgles/src/FTGlyph/FTPolygonGlyph.cpp \
	FreeType/ftgles/src/FTGlyph/FTTextureGlyph.cpp\
	FreeType/ftgles/src/FTFont/FTPixmapFont.cpp \
	FreeType/ftgles/src/FTFont/FTOutlineFont.cpp \
	FreeType/ftgles/src/FTGlyph/FTBitmapGlyph.cpp \
	FreeType/ftgles/src/FTGlyph/FTOutlineGlyph.cpp \
	FreeType/ftgles/src/FTGlyphContainer.cpp \
	FreeType/ftgles/src/FTLayout/FTLayout.cpp \
	FreeType/ftgles/src/FTLayout/FTLayoutGlue.cpp \
	FreeType/ftgles/src/FTLayout/FTSimpleLayout.cpp \
	FreeType/ftgles/src/FTLibrary.cpp \
	FreeType/ftgles/src/FTPoint.cpp \
	FreeType/ftgles/src/FTSize.cpp \
	FreeType/ftgles/src/FTVectoriser.cpp \
	FreeType/ftgles/src/iGLU-1.0.0/libtess/dict.c \
	FreeType/ftgles/src/iGLU-1.0.0/libtess/geom.c \
	FreeType/ftgles/src/iGLU-1.0.0/libtess/memalloc.c \
	FreeType/ftgles/src/iGLU-1.0.0/libtess/mesh.c \
	FreeType/ftgles/src/iGLU-1.0.0/libtess/normal.c \
	FreeType/ftgles/src/iGLU-1.0.0/libtess/priorityq.c \
	FreeType/ftgles/src/iGLU-1.0.0/libtess/render.c \
	FreeType/ftgles/src/iGLU-1.0.0/libtess/sweep.c \
	FreeType/ftgles/src/iGLU-1.0.0/libtess/tess.c \
	FreeType/ftgles/src/iGLU-1.0.0/libtess/tessmono.c \

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
