
MY_PATH := $(call my-dir)
LOCAL_PATH := $(MY_PATH)

#加载svl的prebuilt
include $(CLEAR_VARS)
LOCAL_MODULE := svlLib
LOCAL_SRC_FILES := prebuilt/svl2/libsvlLib.a
#LOCAL_SRC_FILES := prebuilt/svl2/libsvlLib.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/prebuilt/svl2/include
include $(PREBUILT_STATIC_LIBRARY)
#include $(PREBUILT_SHARED_LIBRARY)

#加载freetype的prebuilt
include $(CLEAR_VARS)
LOCAL_MODULE := freetypeLib
LOCAL_SRC_FILES := prebuilt/freetype/libFreetypeLib.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/prebuilt/freetype/include
include $(PREBUILT_STATIC_LIBRARY)

#加载SOIL图片解析为GL纹理库
include $(CLEAR_VARS)
LOCAL_MODULE := libSOIL
LOCAL_SRC_FILES := prebuilt/soil/libSOIL.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/prebuilt/soil/include
include $(PREBUILT_STATIC_LIBRARY)

#加载assimp的prebuilt
include $(CLEAR_VARS)
LOCAL_MODULE := libassimp
LOCAL_SRC_FILES := prebuilt/assimp/libassimp_static.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/prebuilt/assimp/include
include $(PREBUILT_STATIC_LIBRARY)


#编译SviewCore
include $(CLEAR_VARS) 
LOCAL_MODULE    := M3D
LOCAL_C_INCLUDES += $(LOCAL_PATH)/m3d/render/include		
LOCAL_C_INCLUDES += $(LOCAL_PATH)/prebuilt/assimp/include	
LOCAL_C_INCLUDES += $(LOCAL_PATH)/prebuilt/svl2/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/prebuilt/svl2/include/3rdParty/7Zip
LOCAL_C_INCLUDES += $(LOCAL_PATH)/prebuilt/svl2/include/3rdParty/LZ4
LOCAL_C_INCLUDES += $(LOCAL_PATH)/prebuilt/assimp/include	

 #-Wno-psabi to remove warning about GCC 4.4 va_list warning
 
 
 ifeq ($(APP_OPTIM),debug)
  APP_CFLAGS := -O0 -g $(APP_CFLAGS)
  LOCAL_CPPFLAGS:=-O0 -g $(LOCAL_CPPFLAGS)
else
  APP_CFLAGS := -O2 -DNDEBUG -g $(APP_CFLAGS)
  LOCAL_CPPFLAGS := -O2 -DNDEBUG -g $(LOCAL_CPPFLAGS)
endif
 
LOCAL_CFLAGS += -D__ANDROID__ -DFT2_BUILD_LIBRARY -Wno-psabi -DDEBUG
LOCAL_CPPFLAGS += -D__ANDROID__ -DFT2_BUILD_LIBRARY

LOCAL_CFLAGS += -D__MOBILE__
LOCAL_CPPFLAGS += -D__MOBILE__
LOCAL_CFLAGS += -D__ANDROID__
LOCAL_CPPFLAGS += -D__ANDROID__
LOCAL_CPPFLAGS += -DJSON_IS_AMALGAMATION -fexceptions 
#IOSTODO 为IOS没有完全移植暂时定义
LOCAL_CFLAGS += -D__IOSTODO__ -DGL_GLEXT_PROTOTYPES -DGL_EGLEXT_PROTOTYPES
LOCAL_CPPFLAGS += -D__IOSTODO__ -DDEBUG

LOCAL_LDLIBS    := -llog -lGLESv1_CM -lGLESv2 -ldl -lz -luuid
LOCAL_ARM_MODE  := arm
LOCAL_STATIC_LIBRARIES := svlLib freetypeLib libassimp

LOCAL_SRC_FILES +=\
	animation/SimulationAnimationPlay/AnimationPlayApi.cpp \
	animation/SimulationAnimationPlay/AnimationHelper.cpp \
	animation/SimulationCommon/UtilityTextParser.cpp \
	animation/SimulationCommon/UtilityXMLParser.cpp \
	animation/SimulationCommon/stdafx.cpp \
	animation/SimulationCommon/utf_utils.cpp \
	animation/SimulationCommon/vhash.cpp \
	animation/SimulationCommon/vlist.cpp\
	animation/SimulationAnimation/TickTimer.cpp \
	animation/SimulationAnimation/SAnimation.cpp \
	animation/SimulationAnimation/SBehaviorAction.cpp \
	animation/SimulationAnimation/SInterpolator.cpp \
	animation/SimulationAnimation/STimeline.cpp \
	animation/SimulationAnimation/KeyFrame.cpp \
	animation/SimulationAnimation/TimerManager.cpp \
	animation/SimulationAnimation/SimulationAnimationManager.cpp \
	animation/SimulationAnimation/stdafx.cpp \
	animation/SimulationAnimation/CTimer.cpp \
	animation/SimulationCommon/SAUtility.cpp \
	animation/SimulationCommon/MatrixOperation.cpp \
	animation/SimulationCommon/SATools.cpp \
	animation/SimulationAnimation/AnimationSound.cpp \
	animation/SimulationAnimation/AnimationStepManager.cpp \
	animation/SimulationAnimation/ProcessManager.cpp \
	animation/SimulationAnimation/CProcess.cpp \
	animation/SimulationAnimation/Mutex.cpp \
	animation/SimulationAnimation/GroupItem.cpp \
	animation/SimulationAnimation/Group.cpp \
	animation/SimulationAnimation/GroupManager.cpp \
	Utility.cpp\
	m3d/M3DMacros.cpp\
	m3d/SceneManager.cpp \
	m3d/ResourceManager.cpp \
    m3d/utils/AnimationCreator.cpp \
	m3d/utils/Trackball.cpp \
	m3d/utils/IDCreator.cpp \
	m3d/utils/CullerHelper.cpp\
	m3d/utils/FileHelper.cpp\
	m3d/utils/M3DTools.cpp\
	m3d/utils/LoggerHelper.cpp\
	m3d/utils/MeshHelper.cpp \
	m3d/utils/ShapeHelper.cpp \
	m3d/utils/StringHelper.cpp \
	m3d/utils/PathHelper.cpp \
	m3d/utils/mem_leak_detector.cpp \
	m3d/utils/MeshTools.cpp \
	m3d/utils/Platform.cpp \
	m3d/RenderManager.cpp \
	m3d/renderer/GL20/GL20Context.cpp \
	m3d/renderer/gl10/GLShapeDrawer.cpp \
	m3d/renderer/gl10/GL10Context.cpp \
	m3d/renderer/gl10/GLDrawer.cpp \
	m3d/renderer/GL20/GLDrawer20.cpp \
	m3d/renderer/GL20/GLShapeDrawer20.cpp \
	m3d/graphics/GraphicsDefs.cpp	\
	m3d/renderer/gl20/Shader.cpp \
	m3d/renderer/gl20/ShaderParameter.cpp \
	m3d/renderer/gl20/ShaderProgram.cpp \
	m3d/renderer/gl20/ShaderManager.cpp \
	m3d/renderer/gl20/ShaderSrcCode.cpp \
	m3d/renderer/gl20/GLShapeDrawerHelper.cpp \
    m3d/renderer/gl20/GLRenderTarget.cpp \
    m3d/graphics/HardWareRenderBuffer.cpp \
    m3d/graphics/GeometryBuffer.cpp \
	m3d/graphics/HardWareFrameBuffer.cpp \
	m3d/scene/SceneGroundNode.cpp	\
	m3d/renderer/RenderContext.cpp \
	m3d/graphics/Texture.cpp \
	m3d/graphics/Texture2D.cpp \
	m3d/graphics/TextureCube.cpp \
	m3d/graphics/SectionPlane.cpp\
	m3d/graphics/ImageBoard.cpp\
	m3d/graphics/MovedMatrixInfo.cpp\
	m3d/graphics/Section.cpp \
	m3d/graphics/Billboard.cpp \
	m3d/graphics/CameraNode.cpp \
	m3d/graphics/Light.cpp \
	m3d/graphics/TextNode.cpp \
	m3d/graphics/Material.cpp \
	m3d/graphics/Renderable.cpp \
	m3d/graphics/MeshBoard.cpp \
	m3d/graphics/Resource.cpp \
	m3d/graphics/HardWareIndexBuffer.cpp \
	m3d/graphics/HardWareVertexBuffer.cpp \
	m3d/graphics/GPUObject.cpp \
	m3d/graphics/CrossSection.cpp \
	m3d/graphics/SectionLine.cpp  \
	m3d/graphics/SectionLineHelper.cpp \
	m3d/model/Shape.cpp \
	m3d/model/MeshData.cpp \
	m3d/model/Body.cpp \
	m3d/model/Face.cpp \
	m3d/model/Edge.cpp \
	m3d/model/Model.cpp \
	m3d/model/Line3D.cpp \
	m3d/model/PMIData.cpp \
	m3d/model/PolyLine.cpp \
	m3d/model/Note.cpp\
	m3d/model/ModelView.cpp\
	m3d/model/ComText.cpp\
	m3d/model/CText.cpp\
    m3d/model/Curve.cpp\
    m3d/model/GeoAttribute.cpp\
    m3d/model/LODData.cpp\
    m3d/model/Image.cpp \
    m3d/model/Point.cpp \
    m3d/model/ShapeSet.cpp \
    m3d/model/ShapeProperty.cpp \
	m3d/extras/measure/Measure.cpp\
	m3d/extras/measure/MeasureAngle.cpp\
	m3d/extras/measure/MeasureDistance.cpp\
	m3d/extras/measure/MeasureFactory.cpp\
	m3d/extras/measure/MeasureGroup.cpp\
	m3d/extras/measure/MeasureProperty.cpp\
	m3d/extras/measure/tools/GeometryHelper.cpp\
	m3d/extras/measure/tools/MeasureCalculateHelper.cpp\
	m3d/extras/measure/tools/MeasureDisplayHelper.cpp\
	m3d/scene/GroupNode.cpp \
	m3d/scene/SceneNode.cpp \
	m3d/scene/ShapeNode.cpp \
	m3d/scene/BackgroundNode.cpp \
	m3d/scene/AxisNode.cpp \
	m3d/scene/FPSNode.cpp \
	m3d/scene/ModelNode.cpp \
	m3d/scene/LSceneNode.cpp \
	m3d/scene/LShapeNode.cpp \
	m3d/extras/note/VoiceNote.cpp \
	m3d/extras/note/TextNote.cpp \
	m3d/extras/note/SequenceNumberNote.cpp \
	m3d/extras/note/ThreeDGesturesNote.cpp \
	m3d/extras/note/NoteFactory.cpp \
	m3d/extras/note/NoteGroup.cpp \
	m3d/handler/Handler.cpp \
	m3d/handler/HandlerGroup.cpp \
	m3d/Handler/HandlerNode.cpp \
	m3d/handler/HandlerNodeRotateCenter.cpp \
	m3d/handler/HandlerRotateCenter.cpp \
	m3d/handler/HandlerPoint.cpp \
	m3d/handler/Rotate1DHandler.cpp \
	m3d/handler/Scale1DHandler.cpp \
	m3d/handler/TransformHandlerNode.cpp \
	m3d/handler/Translate1DHandler.cpp \
	m3d/base/ControlInfo.cpp \
	m3d/base/Object.cpp \
	m3d/base/CTimer.cpp \
	m3d/base/AreaAllocator.cpp \
    m3d/base/BoundingBox.cpp \
	m3d/base/Color.cpp \
	m3d/base/Frustum.cpp \
	m3d/base/Matrix3.cpp \
	m3d/base/Matrix4.cpp \
    m3d/base/Matrix3x4.cpp \
	m3d/base/Plane.cpp \
	m3d/base/Quaternion.cpp \
	m3d/base/Random.cpp \
	m3d/base/Ray.cpp \
	m3d/base/Rect.cpp \
	m3d/base/Sphere.cpp \
	m3d/base/StringHash.cpp \
	m3d/base/Vector2.cpp \
	m3d/base/Vector3.cpp \
	m3d/base/Vector4.cpp \
    m3d/base/Viewport.cpp \
	m3d/base/Mutex.cpp \
	m3d/base/Thread.cpp \
	m3d/base/FileCacheManager.cpp \
	m3d/base/json/jsoncpp.cpp \
	m3d/base/glue/GlueObj.cpp \
	m3d/base/poly2tri/common/shapes.cpp \
	m3d/base/poly2tri/sweep/advancing_front.cpp \
	m3d/base/poly2tri/sweep/cdt.cpp \
	m3d/base/poly2tri/sweep/sweep_context.cpp \
	m3d/base/poly2tri/sweep/sweep.cpp \
	m3d/action/RenderAction.cpp \
	m3d/action/CallBackAction.cpp \
	m3d/action/RayPickAction.cpp \
	m3d/action/Action.cpp \
	m3d/base/event/BaseEvent.cpp \
	m3d/base/event/CameraEvent.cpp \
	m3d/extras/modelmanager/AddFileOperation.cpp \
	m3d/extras/modelmanager/CopyModelOperation.cpp \
	m3d/extras/modelmanager/MoveModelOperation.cpp \
	m3d/extras/modelmanager/RemoveModelOperation.cpp \
	m3d/extras/modelmanager/RenameOperation.cpp \
	m3d/extras/OperationHistoryManager.cpp \
	m3d/extras/modelmanager/ModelManager.cpp \
	m3d/extras/modelmanager/ModelAssemblyHelper.cpp \
	m3d/extras/Operation.cpp \
	sview/extern/WorkNodes.cpp \
	sview/extern/PerspectiveData.cpp \
	sview/extern/PerspectiveOperator.cpp \
	sview/extern/SectionOperator.cpp \
	sview/extern/ExplosiveViewOperator.cpp \
	sview/extern/SpecialEffectOperator.cpp \
	sview/extern/BaseAnimation.cpp \
	sview/extern/AnimationOperator.cpp \
	sview/extern/NormalAnimation.cpp \
	sview/views/Parameters.cpp \
	sview/extern/ViewOperator.cpp \	sview/views/IView.cpp \
	sview/views/View.cpp \
	sview/views/Selector.cpp \
	sview/views/LicenseManager.cpp \
	sview/utils/ViewHelper.cpp \
	sview/utils/SAES.cpp \
	sview/utils/SAESHelper.cpp \
	sview/utils/TriangleLib.cpp \
	sview/io/Reader.cpp \
	sview/io/Svlreader.cpp \
	sview/io/Writer.cpp \
	sview/io/StlWriter.cpp \
	sview/io/SvlWriter.cpp \
	sview/io/XmlSceneSerializer.cpp\
	sview/io/AssimpReader.cpp \
	sview/manipulator/TouchHandler.cpp \
	sview/manipulator/CommonTouchHandler.cpp \
	sview/manipulator/OribitControlHandler.cpp \
	sview/manipulator/WalkthroughHandler.cpp \
	sview/manipulator/DraggerHandler.cpp \
	sview/PMIAnalysis/PMICreator.cpp \
	sview/PMIAnalysis/TessCircle.cpp\
	sview/PMIAnalysis/PMIUtilityLib.cpp\
	sview/PMIAnalysis/toolkit/MathGeolib.cpp\
	sview/PMIAnalysis/toolkit/MathVector.cpp\
	sview/PMIAnalysis/toolkit/PMIESymbol.cpp \
	tinyxml/tinyxml2.cpp \

#include $(BUILD_SHARED_LIBRARY) 
include $(BUILD_STATIC_LIBRARY)

