// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
 *	@file		Stk_Declare_Ptr.h
 *  @brief		����ָ�������
**************************************************************************/
#ifndef _SVLLIB_DECLARE_H_
#define _SVLLIB_DECLARE_H_
#include "Stk_SharePtr.h"

namespace HoteamSoft {
	namespace SVLLib {

	
/** @brief	����	*/
class Stk_Object;
/** @brief	��־	*/
class Stk_Log;
/** @brief	�ڵ�	*/
class Stk_Node;
/** @brief	ʵ����	*/
class Stk_Instance;
/** @brief	�ĵ�	*/
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
/** @brief	PMI����	*/
class Stk_PMI;
class Stk_MetaDataDocument;
class Stk_PMIDocument;
class Stk_AttributeCollection;
class Stk_GeometryAttributeCollection;
/** @brief	������	*/
class Stk_Leader;
/** @brief	�ӳ�����	*/
class Stk_Extensionline;
class Stk_CurveEntity;
/** @brief	��������	*/
class Stk_SpecialLine;
/** @brief	�ı���	*/
class Stk_Text;
/** @brief	�����ı���	*/
class Stk_ComText;
/** @brief	�����	*/
class Stk_OutFrame;
class Stk_Image;
/** @brief	�û�������	*/
class Stk_UserData;
/** @brief	�߼���	*/
class Stk_LineSet;
/** @brief	������	*/
class Stk_PolyLine;
/** @brief	������	*/
class Stk_Arc;
/** @brief	��������	*/
class Stk_Line;
/** @brief	NURBS������	*/
class Stk_NurbsCurve;
/** @brief	˫������	*/
class Stk_Hyperbola;
/** @brief	��������	*/
class Stk_Parabola;
/** @brief	�������	*/
class Stk_Envelope;
/** @brief	�ռ����	*/
class Stk_SpacialPointEntity;
class Stk_Attribute;
/** @brief	������	*/
class Stk_Material;
class Stk_PlaneEntity;
class Stk_ClipPlane;
/** @brief	������Ӱ����	*/
class Stk_ClippingCurve;
class Stk_Connector;
class Stk_Texture;
/** @brief	�ļ�������Ϣ��	*/
class Stk_LoadInf;
/** @brief	ģ�ͷ�����	*/
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
/** @brief	���������	*/
class Stk_ComponentFeature;
/** @brief	װ��������	*/
class Stk_AssemblyFeature;
/** @brief	ͼ����	*/
class Stk_Layer;
/** @brief	ͼ���������	*/
class Stk_LayerFilter;
/** @brief	ͼ����Ϣ����	*/
class Stk_LayerInfosSet;
/** @brief	������	*/
class Stk_Feat;
/** @brief	����������	*/
class Stk_Feat_Pipe;
/** @brief	��������	*/
class Stk_Feat_Hole;
/** @brief	������������	*/
class Stk_Feat_MagneticLine;
/** @brief	���������	*/
class Stk_Feat_Mark;
/** @brief	ͽ�ֻ����������	*/
class Stk_Mark_FreeHand;
/** @brief	��Բ���������	*/
class Stk_Mark_Ellipse;
/** @brief	���α��������	*/
class Stk_Mark_RectAngle;

#if 0
/** @brief	Ҫ��	*/
class Stk_Entity;
/** @brief	ԭ��	*/
class Stk_ProtoType;
/** @brief	�ļ�	*/
class Stk_File;
/** @brief	�ļ�����	*/
class Stk_DocumentManager;
/** @brief	����	*/
class Stk_Preview;
/** @brief	Body�����	*/
class Stk_Body;
/** @brief	Mesh����	*/
class Stk_MeshEntity;
/** @brief	Mesh ����ͼ����	*/
class Stk_Mesh_Com;
/** @brief	Mesh ��������	*/
class Stk_Mesh_Face;
/** @brief	Mesh �߽�����	*/
class Stk_Mesh_Edge;
/** @brief	View��ͼ��	*/
class Stk_View;
/** @brief	������	*/
class Stk_NodeConnector;
/** @brief	���������	*/
class Stk_ComponentFeature;
/** @brief	��Ⱦ��	*/
class Stk_Render;
/** @brief	������	*/
class Stk_Material;
/** @brief	��������	*/
class Stk_Curve;
/** @brief	NURBS������	*/
class Stk_NurbsCurve;
/** @brief	˫������	*/
class Stk_Hyperbola;
/** @brief	��������	*/
class Stk_Parabola;
/** @brief	�ļ�����	*/
class Stk_AesHelper;
/** @brief	���������	*/
class Stk_ComponentFeature;
/** @brief	װ��������	*/
class Stk_AssemblyFeature;
/** @brief	��ͷ��	*/
class Stk_Camera;
/** @brief	������	*/
class Stk_Feat;
/** @brief	��ͷ������	*/
class Stk_Feat_Arrow;
/** @brief	ֱ�߼�ͷ������	*/
class Stk_Feat_LineArrow;
/** @brief	Բ�μ�ͷ������	*/
class Stk_Feat_CircularArrow;
/** @brief	ֱ�Ǽ�ͷ������	*/
class Stk_Feat_RightAngleArrow;
/** @brief	����������	*/
class Stk_Feat_Axis;
/** @brief	Limit������	*/
class Stk_FeatLimit;
/** @brief	������������	*/
class Stk_FeatInnerThread;
/** @brief	��������	*/
class Stk_Feat_Hole;
/** @brief	������������	*/
class Stk_Feat_MagneticLine;
/** @brief	���������	*/
class Stk_Feat_Mark;
/** @brief	���α��������	*/
class Stk_Mark_RectAngle;
/** @brief	��Բ���������	*/
class Stk_Mark_Ellipse;
/** @brief	ͽ�ֻ����������	*/
class Stk_Mark_FreeHand;
/** @brief	��������������	*/
class Stk_Pipe_Fiber_Segment;
/** @brief	����������	*/
class Stk_Feat_Pipe;
/** @brief	�������Ի���	*/
class Stk_GeometryAttribute;
/** @brief	��������	*/
class Stk_LineAttribute;
/** @brief	Ellipse������	*/
class Stk_ArcAttribute;
/** @brief	ƽ��������	*/
class Stk_PlaneFaceAttribute;
/** @brief	��ת�����Ի���	*/
class Stk_RevolutionFaceAttribute;
/** @brief	Բ����������	*/
class Stk_CylinderFaceAttribute;
/** @brief	Բ׶��������	*/
class Stk_ConicalFaceAttribute;
/** @brief	����������	*/
class Stk_SphereFaceAttribute;
/** @brief	��̥��������	*/
class Stk_ToroidalFaceAttribute;
/** @brief	��������	*/
class Stk_GroupFeature;
/** @brief	��Ϣ��	*/
class Stk_Information;
/** @brief	ʵ������	*/
class Stk_InsAttribute;
/** @brief	ʵ����	*/
class Stk_Instance;
/** @brief	ͼ����	*/
class Stk_Layer;
/** @brief	ͼ���������	*/
class Stk_LayerFilter;
/** @brief	ͼ����Ϣ����	*/
class Stk_LayerInfosSet;
/** @brief	�ƹ���	*/
class Stk_LightMap;
/** @brief	�߼���	*/
class Stk_LineSet;
/** @brief	�¼���������	*/
class Stk_Listener;
/** @brief	�Զ���������	*/
class Stk_MetaData;
/** @brief	����������	*/
class Stk_PatternFeature;
/** @brief	��ȡ�¼���������	*/
class Stk_ReaderListener;
/** @brief	�ռ����	*/
class Stk_SpacialPoint;
/** @brief	������	*/
class Stk_Texture;
/** @brief	����BOM������Ϣ����	*/
class Stk_VirtualBomItemInfo;
/** @brief	ȫ��ID������	*/
class Stk_GlobalIDManager;
#endif

//!< ����ָ�붨��
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