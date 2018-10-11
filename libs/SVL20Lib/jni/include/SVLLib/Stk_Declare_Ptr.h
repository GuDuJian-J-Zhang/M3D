// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
 *	@file		Stk_Declare_Ptr.h
 *  @brief		智能指针的声明
**************************************************************************/
#ifndef _SVLLIB_DECLARE_H_
#define _SVLLIB_DECLARE_H_
#include "Stk_SharePtr.h"

namespace HoteamSoft {
	namespace SVLLib {

	
/** @brief	基类	*/
class Stk_Object;
/** @brief	日志	*/
class Stk_Log;
/** @brief	节点	*/
class Stk_Node;
/** @brief	实例类	*/
class Stk_Instance;
/** @brief	文档	*/
class Stk_Document;
class Stk_Part;
class Stk_Model;
class Stk_MeshPrim;
class Stk_MeshFace;
class Stk_MeshEdge;
class Stk_View;
class Stk_Camera;
class Stk_InsAttribute;
class Stk_Accessor;
class Stk_MeshAccessor;
class Stk_CommonAccessor;
class Stk_MeshLodBlock;
class Stk_ResourceManager;
class Stk_File;
class Stk_ViewDocument;
class Stk_MetaData;
/** @brief	PMI基类	*/
class Stk_PMI;
class Stk_MetaDataDocument;
class Stk_PMIDocument;
class Stk_AttributeCollection;
class Stk_GeometryAttributeCollection;
/** @brief	引线类	*/
class Stk_Leader;
/** @brief	延长线类	*/
class Stk_Extensionline;
class Stk_CurveEntity;
/** @brief	特殊线类	*/
class Stk_SpecialLine;
/** @brief	文本类	*/
class Stk_Text;
/** @brief	复合文本类	*/
class Stk_ComText;
/** @brief	外框类	*/
class Stk_OutFrame;
class Stk_Image;
/** @brief	用户数据类	*/
class Stk_UserData;
/** @brief	线集类	*/
class Stk_LineSet;
/** @brief	折线类	*/
class Stk_PolyLine;
/** @brief	弧线类	*/
class Stk_Arc;
/** @brief	两点线类	*/
class Stk_Line;
/** @brief	NURBS曲线类	*/
class Stk_NurbsCurve;
/** @brief	双曲线类	*/
class Stk_Hyperbola;
/** @brief	抛物线类	*/
class Stk_Parabola;
/** @brief	包络框类	*/
class Stk_Envelope;
/** @brief	空间点类	*/
class Stk_SpacialPointEntity;
class Stk_Attribute;
/** @brief	材质类	*/
class Stk_Material;
class Stk_PlaneEntity;
class Stk_ClipPlane;
/** @brief	剖切阴影线类	*/
class Stk_ClippingCurve;
class Stk_Connector;
class Stk_Texture;
/** @brief	文件加载信息类	*/
class Stk_LoadInf;
/** @brief	模型分组类	*/
class Stk_ModelGroup;
class Stk_GeometryAttribute;
class Stk_MeshAttribute;
class Stk_PlaneFaceAttribute;
class Stk_RevolutionFaceAttribute;
class Stk_CylinderFaceAttribute;
class Stk_ConeFaceAttribute;
class Stk_SphereFaceAttribute;
class Stk_ToroidalFaceAttribute;
class Stk_LineAttribute;
class Stk_EllipseAttribute;
/** @brief	组件特征类	*/
class Stk_ComponentFeature;
/** @brief	装配特征类	*/
class Stk_AssemblyFeature;
/** @brief	图层类	*/
class Stk_Layer;
/** @brief	图层过滤器类	*/
class Stk_LayerFilter;
/** @brief	图层信息集类	*/
class Stk_LayerInfosSet;
/** @brief	特征类	*/
class Stk_Feat;
/** @brief	线缆特征类	*/
class Stk_Feat_Pipe;
/** @brief	孔特征类	*/
class Stk_Feat_Hole;
/** @brief	磁力线特征类	*/
class Stk_Feat_MagneticLine;
/** @brief	标记特征类	*/
class Stk_Feat_Mark;
/** @brief	徒手画标记特征类	*/
class Stk_Mark_FreeHand;
/** @brief	椭圆标记特征类	*/
class Stk_Mark_Ellipse;
/** @brief	矩形标记特征类	*/
class Stk_Mark_RectAngle;

#if 0
/** @brief	要素	*/
class Stk_Entity;
/** @brief	原型	*/
class Stk_ProtoType;
/** @brief	文件	*/
class Stk_File;
/** @brief	文件管理	*/
class Stk_DocumentManager;
/** @brief	阅览	*/
class Stk_Preview;
/** @brief	Body组件类	*/
class Stk_Body;
/** @brief	Mesh基类	*/
class Stk_MeshEntity;
/** @brief	Mesh 复合图形类	*/
class Stk_Mesh_Com;
/** @brief	Mesh 构成面类	*/
class Stk_Mesh_Face;
/** @brief	Mesh 边界线类	*/
class Stk_Mesh_Edge;
/** @brief	View视图类	*/
class Stk_View;
/** @brief	关联类	*/
class Stk_NodeConnector;
/** @brief	组件特征类	*/
class Stk_ComponentFeature;
/** @brief	渲染类	*/
class Stk_Render;
/** @brief	材质类	*/
class Stk_Material;
/** @brief	线条基类	*/
class Stk_Curve;
/** @brief	NURBS曲线类	*/
class Stk_NurbsCurve;
/** @brief	双曲线类	*/
class Stk_Hyperbola;
/** @brief	抛物线类	*/
class Stk_Parabola;
/** @brief	文件加密	*/
class Stk_AesHelper;
/** @brief	组件特征类	*/
class Stk_ComponentFeature;
/** @brief	装配特征类	*/
class Stk_AssemblyFeature;
/** @brief	镜头类	*/
class Stk_Camera;
/** @brief	特征类	*/
class Stk_Feat;
/** @brief	箭头特征类	*/
class Stk_Feat_Arrow;
/** @brief	直线箭头特征类	*/
class Stk_Feat_LineArrow;
/** @brief	圆形箭头特征类	*/
class Stk_Feat_CircularArrow;
/** @brief	直角箭头特征类	*/
class Stk_Feat_RightAngleArrow;
/** @brief	孔轴特征类	*/
class Stk_Feat_Axis;
/** @brief	Limit特征类	*/
class Stk_FeatLimit;
/** @brief	孔螺纹特征类	*/
class Stk_FeatInnerThread;
/** @brief	孔特征类	*/
class Stk_Feat_Hole;
/** @brief	磁力线特征类	*/
class Stk_Feat_MagneticLine;
/** @brief	标记特征类	*/
class Stk_Feat_Mark;
/** @brief	矩形标记特征类	*/
class Stk_Mark_RectAngle;
/** @brief	椭圆标记特征类	*/
class Stk_Mark_Ellipse;
/** @brief	徒手画标记特征类	*/
class Stk_Mark_FreeHand;
/** @brief	光纤线缆特征类	*/
class Stk_Pipe_Fiber_Segment;
/** @brief	线缆特征类	*/
class Stk_Feat_Pipe;
/** @brief	基本属性基类	*/
class Stk_GeometryAttribute;
/** @brief	线属性类	*/
class Stk_LineAttribute;
/** @brief	Ellipse属性类	*/
class Stk_ArcAttribute;
/** @brief	平面属性类	*/
class Stk_PlaneFaceAttribute;
/** @brief	旋转面属性基类	*/
class Stk_RevolutionFaceAttribute;
/** @brief	圆柱面属性类	*/
class Stk_CylinderFaceAttribute;
/** @brief	圆锥面属性类	*/
class Stk_ConicalFaceAttribute;
/** @brief	球面属性类	*/
class Stk_SphereFaceAttribute;
/** @brief	轮胎面属性类	*/
class Stk_ToroidalFaceAttribute;
/** @brief	组特征类	*/
class Stk_GroupFeature;
/** @brief	信息类	*/
class Stk_Information;
/** @brief	实例属性	*/
class Stk_InsAttribute;
/** @brief	实例类	*/
class Stk_Instance;
/** @brief	图层类	*/
class Stk_Layer;
/** @brief	图层过滤器类	*/
class Stk_LayerFilter;
/** @brief	图层信息集类	*/
class Stk_LayerInfosSet;
/** @brief	灯光类	*/
class Stk_LightMap;
/** @brief	线集类	*/
class Stk_LineSet;
/** @brief	事件监听器类	*/
class Stk_Listener;
/** @brief	自定义属性类	*/
class Stk_MetaData;
/** @brief	阵列特征类	*/
class Stk_PatternFeature;
/** @brief	读取事件监听器类	*/
class Stk_ReaderListener;
/** @brief	空间点类	*/
class Stk_SpacialPoint;
/** @brief	纹理类	*/
class Stk_Texture;
/** @brief	工艺BOM属性信息栏类	*/
class Stk_VirtualBomItemInfo;
/** @brief	全局ID管理类	*/
class Stk_GlobalIDManager;
#endif

//!< 智能指针定义
typedef Stk_SharePtr<Stk_Object>						Stk_ObjectPtr;
typedef Stk_SharePtr<Stk_Log>							Stk_LogPtr;
typedef Stk_SharePtr<Stk_Node>						Stk_NodePtr;
typedef Stk_SharePtr<Stk_Instance>						Stk_InstancePtr;
typedef Stk_SharePtr<Stk_Document>				Stk_DocumentPtr;
typedef Stk_SharePtr<Stk_MeshPrim>				Stk_MeshPrimPtr;
typedef Stk_SharePtr<Stk_MeshFace>				Stk_MeshFacePtr;
typedef Stk_SharePtr<Stk_MeshEdge>				Stk_MeshEdgePtr;
typedef Stk_SharePtr<Stk_Part>				Stk_PartPtr;
typedef Stk_SharePtr<Stk_Model>				Stk_ModelPtr;
typedef Stk_SharePtr<Stk_View>							Stk_ViewPtr;
typedef Stk_SharePtr<Stk_Camera>						Stk_CameraPtr;
typedef Stk_SharePtr<Stk_InsAttribute>					Stk_InsAttributePtr;
typedef Stk_SharePtr<Stk_Accessor>				Stk_AccessorPtr;
typedef Stk_SharePtr<Stk_MeshAccessor>				Stk_MeshAccessorPtr;
typedef Stk_SharePtr<Stk_ResourceManager>				Stk_ResourceManagerPtr;
typedef Stk_SharePtr<Stk_File>				Stk_FilePtr;
typedef Stk_SharePtr<Stk_ViewDocument>					Stk_ViewDocumentPtr;
typedef Stk_SharePtr<Stk_MetaDataDocument>     Stk_MetaDataDocumentPtr;
typedef Stk_SharePtr<Stk_PMIDocument>         Stk_PMIDocumentPtr;
typedef Stk_SharePtr<Stk_MetaData>            Stk_MetaDataPtr;
typedef Stk_SharePtr<Stk_PMI>							Stk_PMIPtr;
typedef Stk_SharePtr<Stk_Leader>						Stk_LeaderPtr;
typedef Stk_SharePtr<Stk_Extensionline>					Stk_ExtensionlinePtr;
typedef Stk_SharePtr<Stk_CurveEntity>                 Stk_CurveEntityPtr;
typedef Stk_SharePtr<Stk_SpecialLine>					Stk_SpecialLinePtr;
typedef Stk_SharePtr<Stk_Text>							Stk_TextPtr;
typedef Stk_SharePtr<Stk_ComText>                          Stk_ComTextPtr;
typedef Stk_SharePtr<Stk_OutFrame>						Stk_OutFramePtr;
typedef Stk_SharePtr<Stk_Image>                      Stk_ImagePtr;
typedef Stk_SharePtr<Stk_UserData>                    Stk_UserDataPtr;
typedef Stk_SharePtr<Stk_LineSet>						Stk_LineSetPtr;
typedef Stk_SharePtr<Stk_Line>							Stk_LinePtr;
typedef Stk_SharePtr<Stk_PolyLine>						Stk_PolyLinePtr;
typedef Stk_SharePtr<Stk_Arc>                    Stk_ArcPtr;
typedef Stk_SharePtr<Stk_NurbsCurve>                    Stk_NurbsCurvePtr;
typedef Stk_SharePtr<Stk_Hyperbola>						Stk_HyperbolaPtr;
typedef Stk_SharePtr<Stk_Parabola>						Stk_ParabolaPtr;
typedef Stk_SharePtr<Stk_Envelope>						Stk_EnvelopePtr;
typedef Stk_SharePtr<Stk_Instance>						Stk_InstancePtr;
typedef Stk_SharePtr<Stk_CommonAccessor>			Stk_CommonAccessorPtr;
typedef Stk_SharePtr<Stk_AttributeCollection>    Stk_AttributeCollectionPtr;
typedef Stk_SharePtr<Stk_GeometryAttributeCollection>   Stk_GeometryAttributeCollectionPtr;
typedef Stk_SharePtr<Stk_Attribute>           Stk_AttributePtr;
typedef Stk_SharePtr<Stk_ClippingCurve>					Stk_ClippingCurvePtr;
typedef Stk_SharePtr<Stk_Material>							Stk_MaterialPtr;
typedef Stk_SharePtr<Stk_PlaneEntity>                Stk_PlaneEntityPtr;
typedef Stk_SharePtr<Stk_Texture>                      Stk_TexturePtr;
typedef Stk_SharePtr<Stk_ClipPlane>               Stk_ClipPlanePtr;
typedef Stk_SharePtr<Stk_Connector>				Stk_ConnectorPtr;
typedef Stk_SharePtr<Stk_SpacialPointEntity>                 Stk_SpacialPointEntityPtr;
typedef Stk_SharePtr<Stk_GeometryAttribute>     Stk_GeometryAttributePtr;
typedef Stk_SharePtr<Stk_MeshAttribute>        Stk_MeshAttributePtr;
typedef Stk_SharePtr<Stk_PlaneFaceAttribute>   Stk_PlaneFaceAttributePtr;
typedef Stk_SharePtr<Stk_RevolutionFaceAttribute>  Stk_RevolutionFaceAttributePtr;
typedef Stk_SharePtr<Stk_CylinderFaceAttribute>  Stk_CylinderFaceAttributePtr;
typedef Stk_SharePtr<Stk_ConeFaceAttribute>  Stk_ConeFaceAttributePtr;
typedef Stk_SharePtr<Stk_SphereFaceAttribute>  Stk_SphereFaceAttributePtr;
typedef Stk_SharePtr<Stk_ToroidalFaceAttribute>  Stk_ToroidalFaceAttributePtr;
typedef Stk_SharePtr<Stk_LineAttribute>     Stk_LineAttributePtr;
typedef Stk_SharePtr<Stk_EllipseAttribute>  Stk_EllipseAttributePtr;
typedef Stk_SharePtr<Stk_ComponentFeature>  Stk_ComponentFeaturePtr;
typedef Stk_SharePtr<Stk_AssemblyFeature>  Stk_AssemblyFeaturePtr;
typedef Stk_SharePtr<Stk_Layer>  Stk_LayerPtr;
typedef Stk_SharePtr<Stk_LayerFilter>  Stk_LayerFilterPtr;
typedef Stk_SharePtr<Stk_LayerInfosSet>  Stk_LayerInfosSetPtr;
typedef Stk_SharePtr<Stk_Feat>  Stk_FeatPtr;
typedef Stk_SharePtr<Stk_Feat_Pipe>  Stk_Feat_PipePtr;
typedef Stk_SharePtr<Stk_Feat_Hole>  Stk_Feat_HolePtr;
typedef Stk_SharePtr<Stk_Feat_MagneticLine>  Stk_Feat_MagneticLinePtr;
typedef Stk_SharePtr<Stk_Feat_Mark>  Stk_Feat_MarkPtr;
typedef Stk_SharePtr<Stk_Mark_FreeHand>  Stk_Mark_FreeHandPtr;
typedef Stk_SharePtr<Stk_Mark_Ellipse>  Stk_Mark_EllipsePtr;
typedef Stk_SharePtr<Stk_Mark_RectAngle>  Stk_Mark_RectAnglePtr;
#if 0
typedef Stk_SharePtr<Stk_Entity>						Stk_EntityPtr;
typedef Stk_SharePtr<Stk_ProtoType>						Stk_ProtoTypePtr;
typedef Stk_SharePtr<Stk_File>							Stk_FilePtr;
typedef Stk_SharePtr<Stk_DocumentManager>				Stk_DocumentManagerPtr;
typedef Stk_SharePtr<Stk_Preview>						Stk_PreviewPtr;
typedef Stk_SharePtr<Stk_Body>							Stk_BodyPtr;
typedef Stk_SharePtr<Stk_NodeConnector>					Stk_NodeConnectorPtr;
typedef Stk_SharePtr<Stk_ComponentFeature>				Stk_ComponentFeaturePtr;
typedef Stk_SharePtr<Stk_Render>						Stk_RenderPtr;
typedef Stk_SharePtr<Stk_Material>						Stk_MaterialPtr;
typedef Stk_SharePtr<Stk_Curve>							Stk_CurvePtr;
typedef Stk_SharePtr<Stk_Arc>							Stk_ArcPtr;
typedef Stk_SharePtr<Stk_NurbsCurve>					Stk_NurbsCurvePtr;
typedef Stk_SharePtr<Stk_Hyperbola>						Stk_HyperbolaPtr;
typedef Stk_SharePtr<Stk_Parabola>						Stk_ParabolaPtr;
typedef Stk_SharePtr<Stk_AesHelper>						Stk_AesHelperPtr;
typedef Stk_SharePtr<Stk_AssemblyFeature>				Stk_AssemblyFeaturePtr;
typedef Stk_SharePtr<Stk_ComText>						Stk_ComTextPtr;
typedef Stk_SharePtr<Stk_Feat>							Stk_FeatPtr;
typedef Stk_SharePtr<Stk_Feat_Arrow>					Stk_Feat_ArrowPtr;
typedef Stk_SharePtr<Stk_Feat_LineArrow>				Stk_Feat_LineArrowPtr;
typedef Stk_SharePtr<Stk_Feat_CircularArrow>			Stk_Feat_CircularArrowPtr;
typedef Stk_SharePtr<Stk_Feat_RightAngleArrow>			Stk_Feat_RightAngleArrowPtr;
typedef Stk_SharePtr<Stk_Feat_Axis>						Stk_Feat_AxisPtr;
typedef Stk_SharePtr<Stk_FeatLimit>						Stk_FeatLimitPtr;
typedef Stk_SharePtr<Stk_FeatInnerThread>				Stk_FeatInnerThreadPtr;
typedef Stk_SharePtr<Stk_Feat_Hole>						Stk_Feat_HolePtr;
typedef Stk_SharePtr<Stk_Feat_MagneticLine>				Stk_Feat_MagneticLinePtr;
typedef Stk_SharePtr<Stk_Feat_Mark>						Stk_Feat_MarkPtr;
typedef Stk_SharePtr<Stk_Mark_RectAngle>				Stk_Mark_RectAnglePtr;
typedef Stk_SharePtr<Stk_Mark_Ellipse>					Stk_Mark_EllipsePtr;
typedef Stk_SharePtr<Stk_Mark_FreeHand>					Stk_Mark_FreeHandPtr;
typedef Stk_SharePtr<Stk_Pipe_Fiber_Segment>			Stk_Pipe_Fiber_SegmentPtr;
typedef Stk_SharePtr<Stk_GeometryAttribute>				Stk_GeometryAttributePtr;
typedef Stk_SharePtr<Stk_LineAttribute>					Stk_LineAttributePtr;
typedef Stk_SharePtr<Stk_EllipseAttribute>				Stk_EllipseAttributePtr;
typedef Stk_SharePtr<Stk_PlaneFaceAttribute>			Stk_PlaneFaceAttributePtr;
typedef Stk_SharePtr<Stk_RevolutionFaceAttribute>		Stk_RevolutionFaceAttributePtr;
typedef Stk_SharePtr<Stk_CylinderFaceAttribute>			Stk_CylinderFaceAttributePtr;
typedef Stk_SharePtr<Stk_ConicalFaceAttribute>			Stk_ConicalFaceAttributePtr;
typedef Stk_SharePtr<Stk_SphereFaceAttribute>			Stk_SphereFaceAttributePtr;
typedef Stk_SharePtr<Stk_ToroidalFaceAttribute>			Stk_ToroidalFaceAttributePtr;
typedef Stk_SharePtr<Stk_GroupFeature>					Stk_GroupFeaturePtr;
typedef Stk_SharePtr<Stk_Information>					Stk_InformationPtr;
typedef Stk_SharePtr<Stk_InsAttribute>					Stk_InsAttributePtr;
typedef Stk_SharePtr<Stk_Layer>							Stk_LayerPtr;
typedef Stk_SharePtr<Stk_LayerFilter>					Stk_LayerFilterPtr;
typedef Stk_SharePtr<Stk_LayerInfosSet>					Stk_LayerInfosSetPtr;
typedef Stk_SharePtr<Stk_LightMap>						Stk_LightMapPtr;
typedef Stk_SharePtr<Stk_LineSet>						Stk_LineSetPtr;
typedef Stk_SharePtr<Stk_Listener>						Stk_ListenerPtr;
typedef Stk_SharePtr<Stk_MeshEntity>					Stk_MeshEntityPtr;
typedef Stk_SharePtr<Stk_Mesh_Com>						Stk_Mesh_ComPtr;
typedef Stk_SharePtr<Stk_Mesh_Face>						Stk_Mesh_FacePtr;
typedef Stk_SharePtr<Stk_Mesh_Edge>						Stk_Mesh_EdgePtr;
typedef Stk_SharePtr<Stk_MetaData>						Stk_MetaDataPtr;
typedef Stk_SharePtr<Stk_PatternFeature>				Stk_PatternFeaturePtr;
typedef Stk_SharePtr<Stk_ReaderListener>				Stk_ReaderListenerPtr;
typedef Stk_SharePtr<Stk_SpacialPoint>					Stk_SpacialPointPtr;
typedef Stk_SharePtr<Stk_Surface>						Stk_SurfacePtr;
typedef Stk_SharePtr<Stk_PlaneSurface>					Stk_PlaneSurfacePtr;
typedef Stk_SharePtr<Stk_ClippingCurve>					Stk_ClippingCurvePtr;
typedef Stk_SharePtr<Stk_Texture>						Stk_TexturePtr;
typedef Stk_SharePtr<Stk_VirtualBomItemInfo>			Stk_VirtualBomItemInfoPtr;
typedef Stk_SharePtr<Stk_GlobalIDManager>				Stk_GlobalIDManagerPtr;
#endif
	}//svllib
}//hoteamsoft
#endif