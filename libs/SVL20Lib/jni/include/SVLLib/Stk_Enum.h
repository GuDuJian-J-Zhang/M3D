// All Rights Reserved. Copyright(C) 2012 HOTEAMSOFT
/************************************************************************************
 *	@file	Stk_Enum.h
 *	@brief	枚举类型的定义文件
 ************************************************************************************/

#ifndef _SVLLIB_ENUM_DEFINE_H_
#define _SVLLIB_ENUM_DEFINE_H_
namespace HoteamSoft {
	namespace SVLLib {

/** @brief	stk的类型定义	*/
enum stk_type_detk
{
	/** @brief	基本类别	*/
	_SVLLIB_TYPE_OBJECT = 0,
	_SVLLIB_TYPE_DOCUMENT,
	_SVLLIB_TYPE_COMPONENT,
	_SVLLIB_TYPE_NODE,
	_SVLLIB_TYPE_BODY,
	_SVLLIB_TYPE_FILE,
	_SVLLIB_TYPE_PROTOTYPE,
	_SVLLIB_TYPE_INSTANCE,
	_SVLLIB_TYPE_MODEL,
	/** @brief	要素	*/
	_SVLLIB_TYPE_ENTITY,
	_SVLLIB_TYPE_MESH,
	_SVLLIB_TYPE_MESH_EDGE,
	_SVLLIB_TYPE_MESH_FACE,
	_SVLLIB_TYPE_CURVE,
	_SVLLIB_TYPE_CURVE_POLYLINE,
	_SVLLIB_TYPE_CURVE_ELLIPSE,
	_SVLLIB_TYPE_CURVE_LINE,
	_SVLLIB_TYPE_CURVE_NURBS,
	_SVLLIB_TYPE_CURVE_HYPERBOLA,
	_SVLLIB_TYPE_CURVE_PARABOLA,
	_SVLLIB_TYPE_CLIPPINGCURVE,
	_SVLLIB_TYPE_LINESET,
	_SVLLIB_TYPE_SPACIALPOINT,
	_SVLLIB_TYPE_TEXT,
	_SVLLIB_TYPE_TEXT_ENVELOPE,
	_SVLLIB_TYPE_TEXT_COMPOSITE,
	_SVLLIB_TYPE_ARROW,
	_SVLLIB_TYPE_ARROW_LINE,
	_SVLLIB_TYPE_ARROW_CIRCULAR,
	_SVLLIB_TYPE_ARROW_RIGHTANGLE,
	_SVLLIB_TYPE_LISTENSER,
	_SVLLIB_TYPE_READERLISTER,
	_SVLLIB_TYPE_LAYER,
	_SVLLIB_TYPE_LAYERFILTER,
	_SVLLIB_TYPE_LAYERINFOS,
	_SVLLIB_TYPE_PMI,
	_SVLLIB_TYPE_PMI_LEADER,
	_SVLLIB_TYPE_PMI_EXLINE,
	_SVLLIB_TYPE_PMI_SPECIALLINE,
	_SVLLIB_TYPE_PMI_COMTEXT,
	_SVLLIB_TYPE_PMI_OUTFRAME,
	_SVLLIB_TYPE_EXTENSIONLINE,
	_SVLLIB_TYPE_LEADER,
	_SVLLIB_TYPE_SPECIALLINE,
	_SVLLIB_TYPE_OUTFRAME,
	_SVLLIB_TYPE_NODECONNECTOR,
	/** @brief	装配	*/
	_SVLLIB_TYPE_ASSEMBLY_FEATURE,
	_SVLLIB_TYPE_COMPONENT_FEATURE,
	_SVLLIB_TYPE_GROUP_FEATURE,
	_SVLLIB_TYPE_PATTERN_FEATURE,
	/** @brief	特征	*/
	_SVLLIB_TYPE_FEAT,
	_SVLLIB_TYPE_FEAT_AXIS,
	_SVLLIB_TYPE_FEAT_LIMIT,
	_SVLLIB_TYPE_FEAT_INNERTHREAD,
	_SVLLIB_TYPE_FEAT_HOLE,
	_SVLLIB_TYPE_FEAT_MAGNETICLINE,
	_SVLLIB_TYPE_FEAT_MARK,
	_SVLLIB_TYPE_FEAT_MARK_RECTANGLE,
	_SVLLIB_TYPE_FEAT_MARK_ELLIPSE,
	_SVLLIB_TYPE_FEAT_MARK_FREEHAND,
	_SVLLIB_TYPE_FEAT_PIPE,
	_SVLLIB_TYPE_FEAT_FIBERPIPE,
	_SVLLIB_TYPE_TEXTURE,
	_SVLLIB_TYPE_RENDER,
	_SVLLIB_TYPE_LIGHTMAP,
	_SVLLIB_TYPE_VIEW,
	_SVLLIB_TYPE_PREVIEW,
	_SVLLIB_TYPE_MATERIAL,
	_SVLLIB_TYPE_CAMERA,
	/** @brief	几何属性	*/
	_SVLLIB_TYPE_ATTRIBUTE_GEOMETRY,
	_SVLLIB_TYPE_ATTRIBUTE_LINE,
	_SVLLIB_TYPE_ATTRIBUTE_ELLIPSE,
	_SVLLIB_TYPE_ATTRIBUTE_PLANEFACE,
	_SVLLIB_TYPE_ATTRIBUTE_REVOLUTIONFACE,
	_SVLLIB_TYPE_ATTRIBUTE_CYLINDERFACE,
	_SVLLIB_TYPE_ATTRIBUTE_CONICALFACE,
	_SVLLIB_TYPE_ATTRIBUTE_SPHEREFACE,
	_SVLLIB_TYPE_ATTRIBUTE_TOROIDALFACE,
	/** @brief	其他	*/
	_SVLLIB_TYPE_AESHELPER,
	_SVLLIB_TYPE_INFORMATION,
	_SVLLIB_TYPE_INSATTIBUTE,
	_SVLLIB_TYPE_USERDATA,
	_SVLLIB_TYPE_METADATA,
	_SVLLIB_TYPE_VRBOM,
	_SVLLIB_MESHPRIM,
	_SVLLIB_COMMON_ACCESSOR,
	_SVLLIB_TYPE_MESHFACE,
	_SVLLIB_TYPE_MESHEDGE,
	_SVLLIB_PART,
	_SVLLIB_TYPE_VIEWDOCUMENT,
	_SVLLIB_TYPE_METADATADOCUMENT,
	_SVLLIB_TYPE_PMIDOCUMENT,
	_SVLLIB_TYPE_ATTRIBUTEDOCUMENT,
	_SVLLIB_TYPE_ATTRIBUTE,
	_SVLLIB_TYPE_INSTANCEATTRIBUTE,
	_SVLLIB_TYPE_MODELATTRIBUTE,
	_SVLLIB_TYPE_CURVEENTITY,
	_SVLLIB_TYPE_PLANE,
	_SVLLIB_TYPE_PLANE_CLIP,
	_SVLLIB_TYPE_CONNECTOR,
	_SVLLIB_TYPE_IMAGE,
	_SVLLIB_TYPE_GEOMETRYATTRIBUTE,
	_SVLLIB_TYPE_PLANEFACEATTRIBUTE,
	_SVLLIB_TYPE_REVOLUTIONATTRIBUTE,
	_SVLLIB_TYPE_TOROIDALFACEATTRIBUTE,
	_SVLLIB_TYPE_LINEATTRIBUTE,
	_SVLLIB_TYPE_ELLIPSEATTRIBUTE,
	_SVLLIB_TYPE_MESHATTRIBUTE,
	_SVLLIB_TYPE_CYLINDERATTRIBUTE,
	_SVLLIB_TYPE_CONEATTRIBUTE,
	_SVLLIB_TYPE_SPHEREATTRIBUTE,
	_SVLLIB_TYPE_GEOMETRYATTRIBUTECOLLECTION
};

/** @brief	定义类型 */
enum Stk_type_define
{
	STK_TYPE_DOCUMENT				= 1,
	STK_TYPE_COMPONENT				= 2,
	STK_TYPE_NODE					= 3,
	STK_TYPE_BODY					= 4,
	STK_TYPE_MESH					= 5,
	STK_TYPE_MESHFACE				= 6,
	STK_TYPE_MESHEDGE				= 7
};

/** @brief	返回状态 */
enum STK_STATUS
{
	STK_SUCCESS						= 0,
	STK_SUCCESS_EXIST				= 1001,
	STK_ERROR						= -1,
	STK_ERROR_DOC_NOT_FOUND			= -1001,
	STK_ERROR_DOC_NOT_SUPPORT		= -1002,
	STK_ERROR_ID_NOT_FOUND			= -1003,
	STK_ERROR_ID_NOT_SUPPORT		= -1004,
	STK_ERROR_NOMORE				= -1005,
	STK_ERROR_MEMORY				= -1006,

	STK_ERROR_FILE_RECORD			= -1007,
	STK_ERROR_FILE_MEMORY			= -1008,
	STK_ERROR_FILERD				= -1009,
	STK_ERROR_HEADRD				= -1010,
	STK_ERROR_FILEWR				= -1012,
	STK_ERROR_HEADWR				= -1011,

	STK_ERROR_VERSION_UNKNOWNFILE	= -2011,
	STK_ERROR_VERSION_HIGHER		= -2012,

	STK_ERROR_SYSTEM_PERMISSIONS	= -2013,

	STK_ERROR_NOMORE_MESH			= -1010,
	STK_ERROR_NOMORE_BODY			= -1011,
	STK_ERROR_NOMORE_PMI			= -1012,
	STK_ERROR_NOMORE_INS			= -1013,
	STK_ERROR_NOMORE_PROTO			= -1014,
	STK_ERROR_NOMORE_NODE			= -1015,

	STK_ERROR_LOOP					= -1020,
	STK_ERROR_FILE_ADD				= -1021,
	STK_ERROR_ANCHOR_NOT_FOUND		= -1022,
	STK_ERROR_PASSWORD				= -1023,

	STK_ERROR_NO_TOPPROTO			= -1025,

	STK_WAITING_PASSWORD			= 1002,

	STK_ERROR_NO_LICENSE			= -1026,
	STK_ERROR_DECODE_LICENSE		= -1027,
	STK_ERROR_PRODUCT_VER_LICENSE	= -1028,
	STK_ERROR_MAC_LICENSE			= -1029,
	STK_ERROR_TIME_LICENSE			= -1030
};


/** @brief	ProtoType类型 */
enum StkProtoTypeEnum
{
	PROTO_TYPE_UNKNOWN = 0,						/*!< =0 未知						*/
	PROTO_TYPE_ASSY,							/*!< =1 装配						*/
	PROTO_TYPE_UNASSY,							/*!< =2 非装配						*/
	PROTO_TYPE_PART,							/*!< =3 零件						*/
	PROTO_TYPE_SHEET							/*!< =4 图纸						*/
};

/** @brief 显隐状态 */
enum StkDisplayStateEnum
{
	STK_DISPLAY = 0,							/*!< =0 显示						*/
	STK_NO_DISPLAY = 1							/*!< =1 隐藏						*/
};

/** @brief	缩略图类型 */
enum StkPreviewTypeEnum
{
	PREVIEW_TYPE_UNKNOWN = 0,					/*!< =0 未知						*/
	PREVIEW_TYPE_JPG,							/*!< =1 JPG类型						*/
	PREVIEW_TYPE_BMP,							/*!< =2 BMP类型						*/
	PREVIEW_TYPE_PNG,							/*!< =3 PNG类型						*/
	PREVIEW_TYPE_CGM,							/*!< =4 CGM类型						*/
	PREVIEW_TYPE_GIF,							/*!< =5 GIF类型						*/
	PREVIEW_TYPE_TIFF,							/*!< =6 TIFF类型					*/
	PREVIEW_TYPE_ICO							/*!< =7 ICON类型					*/
};

/** @brief	Entity要素类型 */
enum StkEntityTypeEnum
{
	ENTITY_TYPE_UNKNOWN = 0,					/*!< 未知							*/
	ENTITY_TYPE_VIEW = 10,						/*!< View类型						*/
	ENTITY_TYPE_PMI = 900,						/*!< PMI类型						*/
	ENTITY_TYPE_MESH_COM = 1013,				/*!< MeshCom类型					*/
	ENTITY_TYPE_BODY = 4001,					/*!< Body类型						*/
	ENTITY_TYPE_MESH_FACE = 4030,				/*!< MeshFace类型					*/
	//ENTITY_TYPE_NOTE = 20000,					/*!< 3D批注类型						*/
	ENTITY_TYPE_REFLINE = 20001,				/*!< 辅助线							*/
	ENTITY_TYPE_MESH_EDG = 20100,				/*!< MeshEdge类型					*/
	ENTITY_TYPE_LAYER = 20101,					/*!< 图层类型						*/
	ENTITY_TYPE_MAGNETIC_LINE = 20102,			/*!< 磁力线类型						*/
	ENTITY_TYPE_FEAT_PIPE = 20103				/*!< 线缆类型						*/
};

/** @brief Mesh存储方式 */
enum StkMeshStoreTypeEnum
{
	MESH_STORE_NON_LOD = 1,						/*!< =1 不支持LOD方式				*/
	MESH_STORE_LOD_UNSHARE_VERTEX = 2,			/*!< =2 LOD方式一：不共享点列		*/
	MESH_STORE_LOD_SHARE_VERTEX = 3				/*!< =3 LOD方式二：共享点列			*/
};

/** @brief MeshEdge存储方式 */
enum StkMeshEdgeStoreTypeEnum
{
	MESH_EDGE_STORE_IN_FACE = 1,				/*!< =1 以MeshFace属性的方式存储	*/
	MESH_EDGE_STORE_IN_EDGE = 2,				/*!< =2 以MeshEdge的方式存储列		*/
};

/** @brief	基本属性类型 */
enum StkGeoAttrTypeEnum
{
	GEOATTR_TYPE_UNKNOWN = 0,
	GEOATTR_TYPE_PLANEFACE,						/*!< 平面							*/
	GEOATTR_TYPE_REVOLUTIONFACE,				/*!< 旋转面							*/
	GEOATTR_TYPE_CYLINDERFACE,					/*!< 圆柱面							*/
	GEOATTR_TYPE_CONICALFACE,					/*!< 圆锥面							*/
	GEOATTR_TYPE_SPHEREFACE,					/*!< 球面							*/
	GEOATTR_TYPE_TOROIDALFACE,					/*!< 轮胎面							*/
	GEOATTR_TYPE_LINE,							//!< 直线
	GEOATTR_TYPE_ELLIPSE,						//!< 椭圆弧
};

/** @brief	PMI类型 */
enum StkPMITypeEnum
{
	PMI_TYPE_UNKNOWN = 0,						/*!< 未知							*/

	PMI_TYPE_LinearDimension,
	PMI_TYPE_AngularDimension,
	PMI_TYPE_CurvilinearDimension,
	PMI_TYPE_DiameterDimension, 
	PMI_TYPE_LinearDiameterDimension,
	PMI_TYPE_RadiusDimension,
	PMI_TYPE_LinearRadiusDimension,
	PMI_TYPE_CumulatedDimension, 
	PMI_TYPE_ChamferDimension,
	PMI_TYPE_DistanceDimension,
	PMI_TYPE_LengthDimension, 

	PMI_TYPE_TypeNote , 
	PMI_TYPE_TypeDatum,
	PMI_TYPE_TypeBalloon,
	PMI_TYPE_TypeDatumTarget,
	PMI_TYPE_TypeRoughness,
	PMI_TYPE_TypeDimension,
	PMI_TYPE_TypeFlagNote,
	PMI_TYPE_TypeGeometricalTolerance,
	PMI_TYPE_TypeAnnotationTable,
	PMI_TYPE_TypeCell,
	PMI_TYPE_TypeTable,
	PMI_TYPE_TypeCallout,
	PMI_TYPE_TypeWelding,
	PMI_TYPE_TypeCoordinateDimension,
	
	PMI_TYPE_Symbol,					//	符号基类

	PMI_TYPE_SVIEW_3DNOTE,				//	SView3D批注，应SView要求增加 add on 2014.6.9
	PMI_TYPE_SVIEW_ARC,					//	SView弧长，应SView要求增加 add on 2014.6.19
	PMI_TYPE_SVIEW_NOTE_COMPONENT,		// 零组件标注，应SView要求增加
	PMI_TYPE_SVIEW_NOTE_WELD,			// 焊点标注，应SView要求增加
	PMI_TYPE_SVIEW_NOTE_COMPONENT_SN,	// 零组件序号标注，应SView要求增加
	PMI_TYPE_SVIEW_NOTE_WELD_SN,		// 焊点序号标注，应SView要求增加
	PMI_TYPE_SVIEW_PICTURE,

	PMI_TYPE_DISTANCE_POINT_TO_POINT,	// 点点距离
	PMI_TYPE_DISTANCE_POINT_TO_LINE,	// 点线距离
	PMI_TYPE_DISTANCE_POINT_TO_FACE,	// 点面距离
	PMI_TYPE_DISTANCE_LINE_TO_LINE,		// 线线距离
	PMI_TYPE_DISTANCE_LINE_TO_FACE,		// 线面距离
	PMI_TYPE_DISTANCE_FACE_TO_FACE,		// 面面距离
	PMI_TYPE_DISTANCE_CENTER_TO_CENTER,	// 中心距离
	PMI_TYPE_DISTANCE_WHEELBASE			// 轴距
};

/** @brief	PMI存储模式 */
enum StkPMIModeEnum
{
	PMI_MODE_UNKNOWN = 0,						/*!< 未知							*/
	PMI_MODE_PARA,								/*!< 参数存储						*/
	PMI_MODE_POLY								/*!< 折线存储						*/
};

/** @brief	Leader类型 */
enum StkLeaderTypeEnum 
{
	LEADER_TYPE_UNKNOWN = 0,
	LEADER_TYPE_LENGTH,
	LEADER_TYPE_ANGULAR
};

/** @brief	Leader 末端类型 */
enum StkTermTypeEnum
{
	TERM_PATSMNONE				= 0,	//	无
	TERM_PATSMOPENARROW			= 1,	//	箭头
	TERM_PATSMCLOSEARROW		= 2,	//	封闭箭头
	TERM_PATSMFILLARROW			= 3,	//	填充箭头
	TERM_PATSMCROSSARROW		= 4,	//	通过箭头
	TERM_PATSMCIRCLE			= 5,	//	空心点
	TERM_PATSMFILLEDCIRCLE		= 6,	//	实心点
	TERM_PATSMSQUARE			= 7,	//	方形
	TERM_PATSMFILLEDSQUARE		= 8,	//	填充方形
	TERM_PATSMSLASH				= 9,	//	斜线
	TERM_PATSMCROSSCIRCLE		= 10,	//	通过空心点
	TERM_PATSMXCIRCLE			= 11,	//	X形空心点
	TERM_PATSMTRIANGLE			= 12,	//	三角（引线指向三角斜边）
	TERM_PATSMFILLEDTRIANGLE	= 13,	//	填充三角（引线指向三角斜边）
	TERM_PATSMPLUS				= 14,	//	水平十字
	TERM_PATSMXCROSS			= 15,	//	X形十字
	TERM_PATSMINTEGRAL			= 16,	//	波浪线
	TERM_PATSMCIRCLECENTER		= 17,	//	中心点
	TERM_PATSMDOUBLEOPENCENTER	= 18,	//	双箭头
	TERM_PATSMDOUBLECLOSEARROW	= 19,	//	双封闭箭头
	TERM_PATSMDOUBLEFILLARROW	= 20,	//	双填充箭头
	TERM_PATSMDOUBLETRIANGLE	= 21,	//	双逆向填充箭头
	//SV中新增定义的末端符号
	TERM_PATSMWHTTRI			= 22,	//	白三角 (引线指向三角顶点)
	TERM_PATSMBLKTRI			= 23,	//	黑三角 (引线指向三角顶点)
	TERM_PATSMWHTTRI2			= 24,	//	无底边三角 
	TERM_PATSMSPACELIN			= 25	//	直线
};


/** @brief	PMI 特殊线的应用类型 */
enum StkSpecLineUsageEnum
{
	SPEC_LINE_USAGE_UNKNOWN = 0,



};


/** @brief	PMI延长线的类型 */
enum StkExtensTypeEnum 
{
	EXTENS_TYPE_UNKNOWN = 0,
	EXTENS_TYPE_LENGTH,
	EXTENS_TYPE_ANGULAR
};


/** @brief	PMI外框的应用类型 */
enum StkOutFrameUsageEnum
{
	OUTFRAME_USAGE_UNKNOWN = 0,
	STK_FRAME_TYPE_NONE = 0,
	STK_FRAME_TYPE_RECTANGLE = 1,
	STK_FRAME_TYPE_SQUARE = 2,
	STK_FRAME_TYPE_CIRCLE = 3,
	STK_FRAME_TYPE_SCORED_CIRCLE = 4,
	STK_FRAME_TYPE_DIAMOND = 5,
	STK_FRAME_TYPE_TRIANGLE = 6,
	STK_FRAME_TYPE_FLAG_RIGHT = 7,
	STK_FRAME_TYPE_FLAG_LEFT = 8,
	STK_FRAME_TYPE_FLAG_BOTH = 9,
	STK_FRAME_TYPE_OBLONG = 10,
	STK_FRAME_TYPE_OBLONG_RIGHT = 11,
	STK_FRAME_TYPE_OBLONG_LEFT = 12,
	STK_FRAME_TYPE_ELLIPSE = 13,
	STK_FRAME_TYPE_STICKING = 14,
	STK_FRAME_TYPE_SET = 15,
	STK_FRAME_TYPE_FIXED_SUPPORT = 16,
	STK_FRAME_TYPE_NOTA = 17,
	STK_FRAME_TYPE_SYMMETRICAL_PART = 18,
	STK_FRAME_TYPE_SYMMETRICAL_SET = 19,
	STK_FRAME_TYPE_SCORED_RECTANGLE = 20,
	STK_FRAME_TYPE_PARALLELOGRAM = 21,
	STK_FRAME_TYPE_OCTANGLE = 22,
	STK_FRAME_TYPE_FLAG_RIGHT_TRIANGLE = 23,
	STK_FRAME_TYPE_SQUARE_CIRCLE = 24,
	STK_FRAME_TYPE_PENTAGON = 25,
	STK_FRAME_TYPE_HEXAGON = 26,
};

/** @brief	PMI外框的应用类型 */
enum StkOutFrameDefEnum
{
	STK_FRAME_DEF_TEXT,							/*!< 属于文本的外框					*/
	STK_FRAME_DEF_PMI							/*!< 属于PMI的外框					*/
};

/** @brief	线条类型 */
enum StkCurveTypeEnum
{
	CURVE_TYPE_UNKNOWN = 0,
	CURVE_TYPE_POLYLINE,
	CURVE_TYPE_ELLIPSE,
	CURVE_TYPE_LINE,
	CURVE_TYPE_NURBSCURVE,
	CURVE_TYPE_HYPERBOLA,
	CURVE_TYPE_PARABOLA
};

/** @brief	线条应用类型 */
enum StkCurveUsageEnum
{
	CURVE_USAGE_UNKNOWN = 0,
	CURVE_USAGE_LEADER_LINE,					/*!< PMI Leader的引出线				*/
	CURVE_USAGE_LEADER_TERM,					/*!< PMI Leader的末端符号			*/
	CURVE_USAGE_EXTENTION_LINE,					/*!< PMI 延长线						*/
	CURVE_USAGE_OUTFRAME_LINE,					/*!< PMI 外框线						*/
	CURVE_USAGE_SPECIAL_LINE					/*!< PMI 特殊线						*/
};

/** @brief	视图应用类型 */
enum StkViewUsageEnum
{
	VIEW_USAGE_UNKNOWN = 0,						/*!< 未知							*/
	VIEW_USAGE_GENERAL_VIEW,					/*!< 普通的视图						*/
	VIEW_USAGE_SV_BASE_VIEW,					/*!< 对应SV中的基本视图				*/
	VIEW_USAGE_SV_USER_VIEW,					/*!< 对应SV中的自定义视图			*/
	VIEW_USAGE_SV_USER_CLIPVIEW,				/*!< 对应SV中的自定义剖视图			*/
	VIEW_USAGE_SV_OTHER_VIEW,					/*!< 对应SV中的其它视图				*/
	VIEW_USAGE_PROE_BASE_VIEW,					/*!< 对应PROE中的基本视图			*/
	VIEW_USAGE_PROE_USER_VIEW,					/*!< 对应PROE中的自定义视图			*/
	VIEW_USAGE_DEFAULT_VIEW,					/*!< 默认视图						*/
	VIEW_USAGE_USER_CLIPPLANE					/*!< 自定义剖切面					*/
};

/** @brief	面类型 */
enum StkPlaneTypeEnum
{
	SURFACE_TYPE_UNKNOWN = 0,
	SURFACE_TYPE_PLANE



};

/** @brief	线条显示类型	*/
enum StkCurveDispTypeEnum
{
	TypeDispUnknown = 0,
	DISCONTINUOUS_LINE,						//!< 点线(_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ )：注：点线与连续线的命名顺序与Datakit中的命名顺序相反
	CONTINUOUS_LINE,						//!< 连续线(________________________________)
	CURVE_TYPE_PHANTOM,						//!< ________ _ _ _________ _ _ _____
	CURVE_TYPE_CENTERLINE,					//!< ________ _ _________ _ _________
	CURVE_TYPE_DOTTED,						//!< ................................
	CURVE_TYPE_LONG_DASHED,					//!< ___ ___ ___ ___ ___ ___ ___ ___
	CURVE_TYPE_DOTTED_DASHED,				//!< ___ . ___ . ___ . ___ . ___ . __
	CURVE_TYPE_NUM_FONT_LINE_TYPES
};

/* @brief	文件保存方式 */
enum StkFileSaveTypeEnum
{
	FILESAVE_TYPE_UNKNOWN = 0,
	FILESAVE_TYPE_UNIQUE,						/*!< 单一保存					*/
	FILESAVE_TYPE_DISTRIBUTION,					/*!< 分散保存					*/
	FILESAVE_TYPE_BEFOR							/*!< 原始状态（单一/分散）		*/
};

/** @brief	自定义属性值类型	*/
enum StkMetaDataValueTypeEnum
{
	TypeValueUnknown = 0,
	VALUE_TYPE_BOOLEAN,						//!< 布尔
	VALUE_TYPE_INTEGER,						//!< 整数
	VALUE_TYPE_REAL,						//!< 实数
	VALUE_TYPE_STRING						//!< 字符串
};

/** @brief	PMI类型来源		*/
enum StkPMISourceEnum
{
	PMISourceUnknown = 0,
	PMI_SOURCE_CONVERT,						//!< 转换得到的PMI
	PMI_SOURCE_CREATE						//!< 用户创建得到的PMI
};

/** @brief	Stk_Text类型		*/
enum StkTextTypeEnum
{
	text_type_prefix,
	text_type_symbol,
	text_type_value,
	text_type_valueaff,
	text_type_fakevalue,
	text_type_unit,
	text_type_tolerance,
	text_type_suffix,
	text_type_text_add,
	text_type_reference,
	text_type_upper_text,
	text_type_lower_text,
	text_type_undefined,
	text_type_roughness_text1,
	text_type_roughness_text2,
	text_type_roughness_text3,
	text_type_roughness_text4,
	text_type_roughness_text5,
	text_type_roughness_text6,
	text_type_roughness_mode,
	text_type_roughness_type,
	text_type_roughness_contact_area,
	text_type_text_add_left,
	text_type_text_add_right,
	text_type_text_add_top,
	text_type_text_add_bottom,
	text_type_usernote_username,
	text_type_usernote_department,
	text_type_usernote_role,
	text_type_usernote_createtime
};

/** @brief	线集类型		*/
enum StkLineSetTypeEnum
{
	LINESET_TYPE_UNKNOWN = 0,				//!< 线集类型未知
	LINESET_TYPE_PIPE,						//!< 线缆
	LINESET_TYPE_OTHERLINE,					//!< 其它线
	LINESET_TYPE_CLIPLINE					//!< 剖切线
};

/** @brief	Node类型	*/
enum StkNodeTypeEnum
{
	NODE_TYPE_UNKNOWN = 0,
	NODE_TYPE_VIRTUAL,							//!< 虚节点
	NODE_TYPE_ASSEMBLY,						//装配
	NODE_TYPE_PART,								//零件
	NODE_TYPE_BODY,								//!< Body型
	NODE_TYPE_MESH,								//!< Mesh类型
	NODE_TYPE_PMI,								//!< PMI型
	NODE_TYPE_VIEW,								//!< View型
	NODE_TYPE_CAMERA,							//!< Camera型
	NODE_TYPE_META,								//!< 属性
	//NODE_TYPE_NOTE,							//!< 批注
	NODE_TYPE_LAYERINFOSSET = 9,				//!< 图层信息集
	NODE_TYPE_JOINTSET,							//!< 焊点信息集
	NODE_TYPE_ASSYJOINT							//!< 焊点连接点
};

/** @brief	Node类型	*/
enum StkAccessorTypeEnum
{
	ACCESSOR_TYPE_UNKNOWN = 0,
	ACCESSOR_TYPE_MESH_N_P,					//!< MeshBin
	ACCESSOR_TYPE_MESH_I_P_N,							//!< MeshBin
	ACCESSOR_TYPE_MESH_I_P_N_T,							//!< MeshBin
};

/** @brief	特征类型	*/
enum StkFeatTypeEnum
{
	FEAT_TYPE_UNKNOWN = 0,
	FEAT_TYPE_PAD,
	FEAT_TYPE_POCKET,
	FEAT_TYPE_HOLE,
	FEAT_TYPE_HOLE_SERIES,
	FEAT_TYPE_THREAD,
	FEAT_TYPE_GROOVE,
	FEAT_TYPE_SHAFT,
	FEAT_TYPE_SHELL,
	FEAT_TYPE_FILLET,
	FEAT_TYPE_PRISM,
	FEAT_TYPE_REVOL,
	FEAT_TYPE_SWEEP,
	FEAT_TYPE_RIB,
	FEAT_TYPE_SLOT,
	FEAT_TYPE_CHAMFER,
	FEAT_TYPE_SPLIT_SURF,
	FEAT_TYPE_THICK_SURF,
	FEAT_TYPE_SEW_SURF,
	FEAT_TYPE_CLOSE_SURF,
	FEAT_TYPE_PATTERN,
	FEAT_TYPE_RECT_PATTERN,
	FEAT_TYPE_CIRC_PATTERN,
	FEAT_TYPE_USER_PATTERN,
	FEAT_TYPE_MIRROR,
	FEAT_TYPE_SURF_PAD, 
	FEAT_TYPE_SURF_REVOL,
	FEAT_TYPE_BOOLEAN,
	FEAT_TYPE_ADD,
	FEAT_TYPE_REMOVE,
	FEAT_TYPE_INTERSECT,
	FEAT_TYPE_ASSEMBLE,
	FEAT_TYPE_UNION_TRIM, 
	FEAT_TYPE_REMOVE_LUMP,
	FEAT_TYPE_DRAFT,
	FEAT_TYPE_TRANSLATE,
	FEAT_TYPE_ROTATE,
	FEAT_TYPE_REMOVE_FACE,
	FEAT_TYPE_STIFFENER,
	FEAT_TYPE_FACE_FILLET,
	FEAT_TYPE_TRITANGENT_FILLET,
	FEAT_TYPE_MULTI_PAD,
	FEAT_TYPE_MULTI_POCKET,
	FEAT_TYPE_LOFT,
	FEAT_TYPE_THICKNESS,
	FEAT_TYPE_SCALING,
	FEAT_TYPE_HOLE_FOR_FASTENER,
	FEAT_TYPE_SURFACE,
	FEAT_TYPE_BODY,
	FEAT_TYPE_SOLID_BODY, 
	FEAT_TYPE_GEOMETRICAL_SET,
	FEAT_TYPE_IMPORT,
	FEAT_TYPE_SHEET_METAL,
	FEAT_TYPE_POSITIVE_LOFT,
	FEAT_TYPE_NEGATIVE_LOFT,
	FEAT_TYPE_MULTI_PRISM, 
	FEAT_TYPE_REPLACE_FACE, 
	FEAT_TYPE_SYMMETRY,
	FEAT_TYPE_USER_DEFINED,
	FEAT_TYPE_TTR_HOLE,
	FEAT_TYPE_TTR_THREAD,
	FEAT_TYPE_PIPE,
	FEAT_TYPE_MAGNETIC_LINE,
	FEAT_TYPE_ARROW,
	FEAT_TYPE_MARK
};

/** @brief	线缆特征类型	*/
enum StkPipeTypeEnum
{
	PIPE_TYPE_UNKNOWN = 0,
	PIPE_TYPE_CONVEYOR_RUN,
	PIPE_TYPE_PIPE_RUN,
	PIPE_TYPE_RACEWAY_RUN,
	PIPE_TYPE_CONDUIT_RUN,
	PIPE_TYPE_DUCT_RUN,
	PIPE_TYPE_HYDRAULICS_RUN,
	PIPE_TYPE_LINE,
	PIPE_TYPE_ELBOW,
	PIPE_TYPE_ELBOW_SEGMENTS,
	PIPE_TYPE_SPLINE,
	PIPE_TYPE_ELEC3D,
	PIPE_TYPE_PROFILE,
	PIPE_TYPE_SPHERICAL_TERMINATOR,
	PIPE_TYPE_EXCENTRIC_CONE
};

/** @brief	线缆特征SECTION类型	*/
enum StkPipeSectionTypeEnum
{
	PIPESECTION_TYPE_UNKNOWN = 0,
	PIPESECTION_TYPE_ROUND,
	PIPESECTION_TYPE_RECTANGULAR,
	PIPESECTION_TYPE_FLAT_OVAL,
	PIPESECTION_TYPE_RADIUS_CORNER,
	PIPESECTION_TYPE_DOUBLE_RIDGE
};

/** @brief	空间点类型	*/
enum StkSpacialPointTypeEnum
{
	STK_PT_UNSPECIFIED = 0,
	STK_PT_CROSS,									//!< a cross which looks like a "X"
	STK_PT_PLUS,									//!< a cross which looks like a "+"
	STK_PT_CONCENTRIC,								//!< an unfilled circle
	STK_PT_COINCIDENT,								//!< two unfilled concentric circles
	STK_PT_FULLCIRCLE,								//!< a filled circle
	STK_PT_FULLSQUARE,								//!< a filled square
	STK_PT_STAR,									//!< a star which is the union of a 2D marker CROSS, a 2D marker PLUS and a 2D marker DOT
	STK_PT_DOT,										//!< a dot
	STK_PT_SMALLDOT,								//!< a small dot whose size is one pixel
	STK_PT_BIGFULLCIRCLE,							//!< a big filled circle
	STK_PT_BIGFULLSQUARE							//!< a big filled square
};

/** @brief	SVL文件保存方式:保存/另存	*/
enum StkFileSaveStyleEnum
{
	TYPE_SAVE_UNKNOWN = 0,
	TYPE_FILE_SAVE,									/*!< 保存					*/
	TYPE_FILE_SAVE_AS								/*!< 另存					*/
};

/** @brief	外框类型		*/
enum StkFrameTypeEnum
{
	TYPE_FRAME_UNKNOWN = 0,
	TYPE_FRAME_RECTANGLE,
	TYPE_FRAME_SQUARE,
	TYPE_FRAME_CIRCLE,
	TYPE_FRAME_SCORED_CIRCLE,
	TYPE_FRAME_DIAMOND,
	TYPE_FRAME_TRIANGLE,
	TYPE_FRAME_FLAG_RIGHT,
	TYPE_FRAME_FLAG_LEFT,
	TYPE_FRAME_FLAG_BOTH,
	TYPE_FRAME_OBLONG,
	TYPE_FRAME_OBLONG_RIGHT,
	TYPE_FRAME_OBLONG_LEFT,
	TYPE_FRAME_ELLIPSE,
	TYPE_FRAME_STICKING,
	TYPE_FRAME_SET,
	TYPE_FRAME_FIXED_SUPPORT,
	TYPE_FRAME_NOTA,
	TYPE_FRAME_SYMMETRICAL_PART,
	TYPE_FRAME_SYMMETRICAL_SET,
	TYPE_FRAME_SCORED_RECTANGLE,
	TYPE_FRAME_PARALLELOGRAM,
	TYPE_FRAME_OCTANGLE,
	TYPE_FRAME_FLAG_RIGHT_TRIANGLE,
	TYPE_FRAME_SQUARE_CIRCLE,
	TYPE_FRAME_PENTAGON,
	TYPE_FRAME_HEXAGON
};
/** @brief	组件特征类型	*/
enum StkComponentFeatureEnum
{
	TYPE_COMPONENTFEATURE_UNKNOWN = 0,
	TYPE_COMPONENTFEATURE_GROUP,
	TYPE_COMPONENTFEATURE_PATTERN,
	TYPE_COMPONENTFEATURE_ASSEMBLY
};


/** @brief	螺纹底部类型	*/
enum StkThreadLimitTypeEnum
{
	TYPE_THREAD_DIMENSION = 0,	// User defined depth value 
	TYPE_THREAD_SUPPORT_DEPTH,	// Thread Depth is of the same of its support (hole, cylinder...) 
	TYPE_THREAD_UP_TO_PLANE		// The thread limit is defined according to a plane. Get the plane reference using get_thread_limit()
};

/** @brief	螺纹类型		*/
enum StkThreadTypeEnum
{
	TYPE_THREAD_UNKNOWN = 0,
	TYPE_THREAD_TAP,			// Define a Tap
	TYPE_THREAD_THREAD			// Define a Thread
};

/** @brief	孔的固定类型	*/
enum StkHoleAnchorTypeEnum
{
	TYPE_EXTREME_ANCHOR_POINT = 0,
	TYPE_MIDDLE_ANCHOR_POINT,
	TYPE_NO_ANCHOR
};

/** @brief	孔类型			*/
enum StkHoleTypeEnum
{
	TYPE_HOLE_SIMPLE = 0,
	TYPE_HOLE_TAPERED,
	TYPE_HOLE_COUNTER_BORED,
	TYPE_HOLE_COUNTER_SUNK,
	TYPE_HOLE_COUNTER_DRILLED,		//when Sunk and Bore have the same diameter. Catia v5 specific.
	TYPE_HOLE_COUNTER_DRILLED_2,	//when Sunk and Bore have two different diameters.Needed for Pro/E.
	TYPE_HOLE_NO
};

/** @brief	孔的底部类型	*/
enum StkHoleBottomTypeEnum
{
	TYPE_BOTTOM_FLAT = 0,
	TYPE_BOTTOM_V,
	TYPE_BOTTOM_LIMITED,
	TYPE_BOTTOM_NO_HOLE
};

/** @brief	字体风格类型	*/
enum StkFontStyleTypeEnum
{
	TYPE_STYLE_REGULAR = 0,
	TYPE_STYLE_BOLD,
	TYPE_STYLE_ITALIC,
	TYPE_STYLE_BOLDITALIC
};

/** @brief	文本属性类型	*/
enum StkTextAttributeTypeEnum
{
	TYPE_ATTRIBUTE_NONE = 0,
	TYPE_ATTRIBUTE_UNDERLINE = 1,
	TYPE_ATTRIBUTE_STRIKETHROUGH = 2,
	TYPE_ATTRIBUTE_OVERLINE = 4,
	TYPE_ATTRIBUYE_SUPERSCRIPT = 8,
	TYPE_ATTRIBUTE_SUBSCRIPT = 16
};

/** @brief	字体类型		*/
enum StkFontTypeEnum
{
	TYPE_FONT_UNKNOWN = 0,
	TYPE_FONT_TRUESCRIPT,
	TYPE_FONT_POSTSCRIPT
};

/** @brief	Limit类型		*/
enum StkLimitTypeEnum
{
	TYPE_LIMIT_DEFAULT = 0,
	TYPE_LIMIT_LENGTH,
	TYPE_LIMIT_UP_TO_NEXT,
	TYPE_LIMIT_UP_TO_LAST,
	TYPE_LIMIT_UP_TO_PLANE,
	TYPE_LIMIT_UP_TO_SURFACE,
	TYPE_LIMIT_UP_THRU_NEXT_LIMIT,
	TYPE_LIMIT_BIDIRECTIONAL,
	TYPE_LIMIT_NO
};

/** @brief	磁力线关联的要素的排列方式	*/
enum StkMagneticArrangementEnum
{
	TYPE_ARRANGEMENT_EQUAL = 0,
	TYPE_ARRANGEMENT_USER_DEF
};

/** @brief	位置固定点类型	*/
enum StkAnchorTypeEnum
{
	TYPE_ANCHOR_TOP_LEFT = 0,
	TYPE_ANCHOR_TOP_CENTER,
	TYPE_ANCHOR_TOP_RIGHT,
	TYPE_ANCHOR_MIDDLE_LEFT,
	TYPE_ANCHOR_MIDDLE_CENTER,
	TYPE_ANCHOR_MIDDLE_RIGHT,
	TYPE_ANCHOR_BOTTOM_LEFT,
	TYPE_ANCHOR_BOTTOM_CENTER,
	TYPE_ANCHOR_BOTTOM_RIGHT,
	TYPE_ANCHOR_NUMBER
};

/* @brief	文档保存方式类型 */
enum StkDocumentSaveTypeEnum
{
	DOCUMENT_SAVE_TYPE_UNKNOWN = 0,
	DOCUMENT_SAVE_TYPE_SAVE,			//!< 保存
	DOCUMENT_SAVE_TYPE_SAVEAS			//!< 另存为
};

/** @brief	压缩方案：不压缩/压缩比/效率优先	*/
enum StkCompressTypeEnum
{
	TYPE_COMPRESS_NO = 0,			// 不压缩
	TYPE_COMPRESS_SPEED = 1,		// 速度优先(LZ4)
	TYPE_COMPRESS_RATIO				// 压缩比优先(7Z)
};

/**@brief	LZ4/7Z压缩等级				*/
enum StkCompressLevelEnum
{
	LEVEL_COMPRESS_UNKNOWN = 0,			// 不压缩
	LZ4_LEVEL_COMPRESS_FAST = 3,		// LZ4快速压缩(1-3之间是快速压缩)
	LZ4_LEVEL_COMPRESS_HIGH = 9,		// LZ4高度压缩
	Zip7_LEVEL_COMPRESS_DEFAULT = 5		// 7Zip默认压缩等级
};

/** @brief	图片类型 */
enum StkPictureTypeEnum
{
	PICTURE_TYPE_UNKNOWN = 0,					/*!< =0 未知						*/
	PICTURE_TYPE_JPG,							/*!< =1 JPG类型						*/
	PICTURE_TYPE_BMP,							/*!< =2 BMP类型						*/
	PICTURE_TYPE_PNG,							/*!< =3 PNG类型						*/
	PICTURE_TYPE_CGM,							/*!< =4 CGM类型						*/
	PICTURE_TYPE_GIF,							/*!< =5 GIF类型						*/
	PICTURE_TYPE_TIFF,							/*!< =6 TIFF类型					*/
	PICTURE_TYPE_ICO,							/*!< =7 ICON类型					*/
	PICTURE_TYPE_RGB24,							/*!< =8 RGB24类型					*/
	PICTURE_TYPE_RGB32,							/*!< =9 RGB32类型					*/
	PICTURE_TYPE_PS,							/*!< =10 PS类型						*/
	PICTURE_TYPE_SVG,							/*!< =11 SVG类型					*/
	PICTURE_TYPE_OLE							/*!< =12 OLE类型					*/
};

/** @brief	图片内/外部存储标识 */
enum StkPicSaveStyleEnum
{
	PICTRE_SAVE_STYLE_OUTER = 0,					/*!< =0 图片存储在SVL外部			*/
	PICTURE_SAVE_STYLE_INNER						/*!< =1 图片存储在SVL内部			*/
};

/** @brief	Node Sub 类型	*/
enum StkNodeSubTypeEnum
{
	NODE_SUB_TYPE_NO = 0,
	NODE_SUB_TYPE_GEOMETRIC_SET,
	NODE_SUB_TYPE_SKETCH,
	NODE_SUB_TYPE_CLIPLINE
};

/** @brief	灯头位置	*/
enum StkActorPositionEnum
{
	ACTOR_POS_NONE = 0,							/*!< =0 没有端点				*/
	ACTOR_POS_BEGIN,							/*!< =1 端点在开始端			*/
	ACTOR_POS_END,								/*!< =2 端点在结束端			*/
	ACTOR_POS_BOTH								/*!< =3 端点在两端				*/
};

/** @brief	箭头位置	*/
enum StkArrowTypeEnum
{
	ARROW_TYPE_LINE = 0,						/*!< =0 直线箭头				*/
	ARROW_TYPE_CIRCULAR,						/*!< =1 圆形箭头				*/
	ARROW_TYPE_RIGHTANGLE						/*!< =2 直角箭头				*/
};

/** @brief	纹理映射类型	*/
enum StkTextureMappingTypeEnum
{
	MAPPINT_TYPE_STORED_UV = 0,					/*!< =0 UV映射方式				*/
	MAPPING_TYPE_PLANAR,
	MAPPING_TYPE_CYLINDER,
	MAPPING_TYPE_SPHERICAL,
	MAPPING_TYPE_CUBICAL
};

/** @brief	剖切阴影线绘制显示方式	*/
enum StkClippingCurveDrawStyleEnum
{
	CLIPPINGCURVE_STYLE_NO = 0,					/*!< =0 不绘制剖面线			*/
	CLIPPINGCURVE_STYLE_FILL_BODY,				/*!< =1 使用实体填充			*/
	CLIPPINGCURVE_STYLE_PART,					/*!< =2 使用零件的剖面线		*/
	CLIPPINGCURVE_STYLE_DATABASE				/*!< =3 使用库的剖切阴影线		*/
};

/** @brief	剖切线绘制显示类型	*/
enum StkClippingCurveDrawTypeEnum
{
	CLIPPINGCURVE_DRAW_TYPE_NO = 0,					/*!< =0 不绘制剖面线				*/
	// ANSI标准
	CLIPPINGCURVE_DRAW_TYPE_ANSI31,					/*!< =1 ANSI标准图案类型31			*/
	CLIPPINGCURVE_DRAW_TYPE_ANSI32,					/*!< =2 ANSI标准图案类型32			*/
	CLIPPINGCURVE_DRAW_TYPE_ANSI33,					/*!< =3 ANSI标准图案类型33			*/
	CLIPPINGCURVE_DRAW_TYPE_ANSI34,					/*!< =4 ANSI标准图案类型34			*/
	CLIPPINGCURVE_DRAW_TYPE_ANSI35,					/*!< =5 ANSI标准图案类型35			*/
	CLIPPINGCURVE_DRAW_TYPE_ANSI36,					/*!< =6 ANSI标准图案类型36			*/
	CLIPPINGCURVE_DRAW_TYPE_ANSI37,					/*!< =7 ANSI标准图案类型37			*/
	CLIPPINGCURVE_DRAW_TYPE_ANSI38,					/*!< =8 ANSI标准图案类型38			*/
	// ISO标准
	CLIPPINGCURVE_DRAW_TYPE_ISO_ALLOY_STEEL,		/*!< =9 ISO标准图案类型alloysteel	*/
	CLIPPINGCURVE_DRAW_TYPE_ISO_CAST_IRON,			/*!< =10 ISO标准图案类型castiron	*/
	CLIPPINGCURVE_DRAW_TYPE_ISO_CERAMIC,			/*!< =11 ISO标准图案类型ceramic		*/
	CLIPPINGCURVE_DRAW_TYPE_ISO_HEAVY_METAL,		/*!< =12 ISO标准图案类型heavymetal	*/
	CLIPPINGCURVE_DRAW_TYPE_ISO_INSULATION,			/*!< =13 ISO标准图案类型insulation	*/
	CLIPPINGCURVE_DRAW_TYPE_ISO_METAL,				/*!< =14 ISO标准图案类型metal		*/
	CLIPPINGCURVE_DRAW_TYPE_ISO_PLASTIC,			/*!< =15 ISO标准图案类型plastic		*/
	CLIPPINGCURVE_DRAW_TYPE_ISO_PORCELAIN,			/*!< =16 ISO标准图案类型porcelain	*/
	CLIPPINGCURVE_DRAW_TYPE_ISO_THERMOSET_PLASTIC,	/*!< =17 ISO标准图案类型thermoset	*/
	CLIPPINGCURVE_DRAW_TYPE_ISO_UNALLOYED_STEEL,	/*!< =18 ISO标准图案类型unalloyed	*/
	// 用户定义
	CLIPPINGCURVE_DRAW_TYPE_UDF_ALUMINUM = 0,		/*!< =19 用户定义图案类型aluminum	*/
	CLIPPINGCURVE_DRAW_TYPE_UDF_COPPER,				/*!< =20 用户定义图案类型copper		*/
	CLIPPINGCURVE_DRAW_TYPE_UDF_ELECTRIC,			/*!< =21 用户定义图案类型electric	*/
	CLIPPINGCURVE_DRAW_TYPE_UDF_GLASS,				/*!< =22 用户定义图案类型glass		*/
	CLIPPINGCURVE_DRAW_TYPE_UDF_IRON,				/*!< =23 用户定义图案类型iron		*/
	CLIPPINGCURVE_DRAW_TYPE_UDF_PLASTIC,			/*!< =24 用户定义图案类型plastic	*/
	CLIPPINGCURVE_DRAW_TYPE_UDF_STEEL,				/*!< =25 用户定义图案类型steel		*/
	CLIPPINGCURVE_DRAW_TYPE_UDF_TITANIUM,			/*!< =26 用户定义图案类型titanium	*/
	CLIPPINGCURVE_DRAW_TYPE_UDF_ZINC,				/*!< =27 用户定义图案类型zinc		*/
	// 使用实体填充类型
	CLIPPINGCURVE_DRAW_TYPE_FILL_BODY,				/*!< =28 使用实体填充				*/
	// 使用零件的剖切线类型
	CLIPPINGCURVE_DRAW_TYPE_USE_PART				/*!< =29 使用零件的剖面线			*/
};

/** @brief	标记类型	*/
enum StkMarkTypeEnum
{
	MARK_TYPE_RECTANGLE = 0,				/*!< =0 矩形标记				*/
	MARK_TYPE_ELLIPSE,						/*!< =1 椭圆标记				*/
	MARK_TYPE_FREEHAND						/*!< =2 徒手画标记				*/
};

/** @brief	标记填充类型	*/
enum StkMarkFillStyleEnum
{
	MARK_FILLSTYLE_NONE = 0,				/*!< =0 无填充					*/
	MARK_FILLSTYLE_QPAQUE,					/*!< =1 不透明填充				*/
	MARK_FILLSTYLE_TRANSPARENT				/*!< =2 透明填充				*/
};

/** @brief	日志级别类型	*/
enum Stk_Log_Level
{
	STK_LOG_LEVEL_NONE=0,					//!< 不打印日志
	STK_LOG_LEVEL_TRACE,					//!< 流程：运行流程打印 通常放置到函数开始
	STK_LOG_LEVEL_DEBUG,					//!< 调试：调试信息打印
	STK_LOG_LEVEL_INFON,					//!< 信息：通常用于操作成功提示
	STK_LOG_LEVEL_NOTICE,					//!< 提示：通常用于操作成功提示比INFO稍高一级别
	STK_LOG_LEVEL_WARNING,					//!< 警告：可能会造成不可预料的影响
	STK_LOG_LEVEL_ERROR,					//!< 错误：不影响整体性能的小错误
	STK_LOG_LEVEL_CRITICAL,					//!< 危急错误：运行不正确情况
	STK_LOG_LEVEL_FATAL,					//!< 致命错误： 导致挂机
	STK_LOG_LEVEL_HIGHEST					//!< 最高级别，没有级别前缀
};

/** @brief	打印日志到文件或控制台	*/
enum Stk_Log_Type
{
	STK_LOG_TO_FILE = 0,					//!< 打印日志到文件
	STK_LOG_TO_CONSOLE,						//!< 打印日志到控制台
};

/** @brief	日志的额外信息类别	*/
enum Stk_Log_Addition_info
{
	STK_LOG_ADDITION_NONE		= 0,		//!< 不附加信息
	STK_LOG_ADDITION_TIME		= 0x0001	//!< 附加时间信息
};

/** @brief  属性属主类型      */
enum StkAttributeOwnerType
{
	STKAttributeOwnerType_NULL,
	STKAttributeOwnerType_Instance = 1,
	STKAttributeOwnerType_Model,
	STKAttributeOwnerType_Mesh,
	STKAttributeOwnerType_MeshFace,
	STKAttributeOwnerType_MeshEdge
};

/** @brief	材质的类型	*/
enum StkMaterialTypeEnum
{
	STK_MATERIALTYPE_SIMPLAE		= 0,		//只有颜色和透明度信息
	STK_MATERIALTYPE_PHONG			= 1,		//带贴图使用
	STK_MATERIALTYPE_ALL,

	STK_MATERIALTYPE_JADE = 101,		//金正金扩展 玉石
	STK_MATERIALTYPE_PERAL = 102,		//金正金扩展 珍珠
	STK_MATERIALTYPE_CRYSTAL = 103,		//金正金扩展 晶石
	STK_MATERIALTYPE_METAL = 104		//金正金扩展	金属
};

#define  PMISTYLE_DISPLAY     0x001

enum StkFileOpenModeEnum
{
	STK_FILE_OPENMODE_LOCAL = 0,	//打开本地文件
	STK_FILE_OPENMODE_BUFFER,		//打开网络缓存文件
};
enum StkMemBlockTypeEnum
{
	STK_MEMBLOCK_TYPE_ORIGIN = 0,	//未压缩的内存块
	STK_MEMBLOCK_TYPE_COMPRESS_7Z,	//7Z压缩的内存块 
};

enum StkAttributeType
{
	StkAttributeType_NULL = 0,
	StkAttributeType_Geo,           //几何属性
};

	}//svllib
}//hoteamsoft
#endif