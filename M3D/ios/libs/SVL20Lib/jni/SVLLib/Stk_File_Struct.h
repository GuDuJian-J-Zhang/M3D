// All Rights Reserved. Copyright(C) 2012 HOTEAMSOFT
/************************************************************************************
 *	@file	Stk_Define.h
 *	@brief	SVL 物理文件相关定义
 ************************************************************************************/

#ifndef _SVLLIB_FILE_STRUCT_H_
#define _SVLLIB_FILE_STRUCT_H_
#include "Stk_Define.h"

namespace HoteamSoft
{
	namespace SVLLib {


/*
 *	段类型
 */
#define SEG_TYPE_PROTO				1				/*!< 零件段							*/
#define SEG_TYPE_BODY				2				/*!< BODY段							*/
#define SEG_TYPE_MESH				3				/*!< Mesh段							*/
#define SEG_TYPE_PMI				4				/*!< PMI段							*/
#define SEG_TYPE_VIEW				5				/*!< View段							*/
#define SEG_TYPE_CONNECTOR			6				/*!< NodeConnector段				*/
#define SEG_TYPE_TITLE				7				/*!< 文件头段						*/
#define SEG_TYPE_PREVIEW			8				/*!< 预览段							*/
#define SEG_TYPE_OPTION				9				/*!< 文件配置项段					*/
#define SEG_TYPE_SUBLINE			10				/*!< 辅助线段						*/
#define SEG_TYPE_NOTE				11				/*!< 批注Note段						*/
#define SEG_TYPE_METADATA			12				/*!< 自定义属性段					*/
#define SEG_TYPE_LINESET			13				/*!< 线集段							*/
#define SEG_TYPE_Node				14				/*!< Node段							*/
#define SEG_TYPE_LAYERSET			15				/*!< 图层信息集段					*/
#define SEG_TYPE_INFO				16				/*!< Node基本信息段					*/
#define SEG_TYPE_FEAT				17				/*!< 特征段							*/
#define SEG_TYPE_FEAT_PIPE			18				/*!< 线缆特征段						*/
#define SEG_TYPE_SPACIALPOINT		19				/*!< 空间点集段						*/
#define SEG_TYPE_USERDEFDATA		20				/*!< 用户自定义数据段				*/
#define SEG_TYPE_LAYER				21				/*!< 图层段							*/
#define SEG_TYPE_GEOTOL				22				/*!< 几何公差段						*/
#define SEG_TYPE_DIMENSION			23				/*!< 尺寸段							*/
#define SEG_TYPE_DATUMTARGET		24				/*!< 基准目标代号					*/
#define SEG_TYPE_ROUGHNESS			25				/*!< 表面粗糙度						*/
#define SEG_TYPE_WELDING			26				/*!< 焊接符号						*/
#define SEG_TYPE_DATUM				27				/*!< 基准符号						*/
#define SEG_TYPE_COMP_PATTERN		28				/*!< 阵列特征						*/
#define SEG_TYPE_COMP_GROUPFEAT		29				/*!< 组特征							*/
#define SEG_TYPE_COMP_ASSEMBLYFEAT	30				/*!< 装配特征						*/
#define SEG_TYPE_INNERTHREAD		31				/*!< 孔的螺纹特征					*/
#define SEG_TYPE_FEATHOLE			32				/*!< 孔特征							*/
#define SEG_TYPE_FEATLIMIT			33				/*!< Limit特征						*/
#define SEG_TYPE_FEAT_MAGNETICLINE	34				/*!< 磁力线特征						*/
#define SEG_TYPE_INSATTRIBUTE		35				/*!< ProtoType下的所有子实例属性段	*/
#define SEG_TYPE_COMPRESS			36				/*!< 压缩段							*/
#define SEG_TYPE_FEAT_LINEARROW			37			/*!< 直线箭头						*/
#define SEG_TYPE_FEAT_CIRCULARARROW		38			/*!< 圆形箭头						*/
#define SEG_TYPE_FEAT_RIGHTANGLEARROW	39			/*!< 直角箭头						*/
#define SEG_TYPE_RENDER				40				/*!< 渲染							*/
#define SEG_TYPE_TEXTURE			41				/*!< 纹理							*/
#define SEG_TYPE_MARK_RECTANGLE		42				/*!< 矩形标记						*/
#define SEG_TYPE_MARK_ELLIPSE		43				/*!< 椭圆标记						*/
#define SEG_TYPE_MARK_FREEHAND		44				/*!< 徒手画标记						*/
#define SEG_TYPE_VIRTUALBOM			45				/*!< 工艺BOM表格					*/
#define SEG_TYPE_LIGHTMAP			46				/*!< 灯光							*/
#define SEG_TYPE_MATERIAL			47				/*!< 材质							*/


#define SEG_TYPE_PROTO_END			1001			/*!< 零件结束标志					*/
#define SEG_TYPE_BODY_END			1002			/*!< BODY结束标志					*/
#define SEG_TYPE_VIEW_END			1003			/*!< VIEW结束标志					*/
#define SEG_TYPE_PMI_END			1004			/*!< PMI结束标志					*/
#define SEG_TYPE_FILE_END			1005			/*!< File结束标志					*/
#define SEG_TYPE_NOTE_END			1006			/*!< 批注Note结束标志				*/
#define SEG_TYPE_Node_END			1007			/*!< Node段结束标志					*/
#define SEG_TYPE_FEAT_END			1008			/*!< 特征段结束标志					*/
#define SEG_TYPE_LAYER_END			1009			/*!< 图层段结束标志					*/
#define SEG_TYPE_LAYERSET_END		1010			/*!< 图层信息集段结束标志			*/
#define SEG_TYPE_FEATHOLE_END		1011			/*!< 孔特征段结束标志				*/
#define SEG_TYPE_INNERTHREAD_END	1012			/*!< 孔的螺纹特征段结束标志			*/
#define SEG_TYPE_FEATLIMIT_END		1013			/*!< 孔的Limit特征段结束标志		*/
#define SEG_TYPE_RENDER_END			1014			/*!< 渲染段结束标志					*/

/*
 *  Record类型
 */
#define REC_TYPE_HEAD			1
#define REC_TYPE_MESH			2
#define REC_TYPE_FACE			3
#define REC_TYPE_EDGE			4
#define REC_TYPE_PROTO			5
//#define REC_TYPE_PROTO_DISTRIBUTION	6

// 实例状态
#define REC_TYPE_INS_ATR		7
#define REC_TYPE_INS_COL		8
#define REC_TYPE_INS_DSP		9
#define REC_TYPE_INS_MAT		10
// PMI相关
#define REC_TYPE_PMI			11
#define	REC_TYPE_LEADER			12
#define	REC_TYPE_EXLINE			13
#define	REC_TYPE_POLYLINE		14
#define	REC_TYPE_ELLIPSE		15
#define REC_TYPE_COMTEXT		16
#define REC_TYPE_TEXT			17
#define REC_TYPE_OUTFRAME		18
#define	REC_TYPE_ANCHIOR		19
#define	REC_TYPE_SPECIALLINE	20
// LOD相关
#define REC_TYPE_MESH_LOD		21
#define REC_TYPE_FACE_LOD		22
#define REC_TYPE_EDGE_LOD		23
// BODY
#define	REC_TYPE_BODY			24
// VIEW相关
#define REC_TYPE_VIEW			25
#define REC_TYPE_CAMERA			26
#define REC_TYPE_CLIP_PLAN		27
// VIEW与PMI关联相关
#define REC_TYPE_NODELINKBASE	28
#define REC_TYPE_PROTOLINK		29
#define REC_TYPE_NODEID			30
#define REC_TYPE_SUBENTITY		31
// File
#define REC_TYPE_FILE			32
// Curve相关
#define REC_TYPE_LINE			33
// 缩略图
#define REC_TYPE_THUMBNAIL		34
// 配置项
#define REC_TYPE_OPTION			35
// CAMERA相关
#define REC_TYPE_CAMERA_MTX		36

// ProtoType Title
#define REC_TYPE_PROTO_TITLE	40
// ProtoType Key（临时方案）
#define REC_TYPE_PROTO_KEY		41

// 辅助线相关
#define REC_TYPE_NURBSCURVE		42
#define REC_TYPE_HYPERBOLA		43
#define REC_TYPE_PARABOLA		44

// 辅助线
#define	REC_TYPE_DMYLINE		45
#define REC_TYPE_SUBLINE		46

// 批注Note
#define REC_TYPE_NOTE			50
// 面属性相关
#define REC_TYPE_FACE_ATTR_PLN	51
#define REC_TYPE_FACE_ATTR_CYL	52
#define REC_TYPE_FACE_ATTR_CON	53
#define REC_TYPE_FACE_ATTR_SPH	54
#define REC_TYPE_FACE_ATTR_REV	55
#define REC_TYPE_FACE_ATTR_TOR	56

// 自定义属性
#define REC_TYPE_METADATA		57

// 边界线属性相关
#define REC_TYPE_EDGE_ATTR_LINE		58
#define REC_TYPE_EDGE_ATTR_ELLIPSE	59
#define REC_TYPE_EDGE_ATTR_CIRCLE	60

// 边界线
#define REC_TYPE_GEOEDGE		61
#define REC_TYPE_MESHEDGE		62

// 线集
#define REC_TYPE_LINESET		63

// Node
#define REC_TYPE_Node			64

//图层
#define REC_TYPE_LAYERINFOSSET	65
#define REC_TYPE_LAYERINFO		66
#define REC_TYPE_LAYERFILTER	67

//基本信息集Stk_Info
#define REC_TYPE_INFO			68

// 线缆特征中的Fiber点
#define REC_TYPE_PIPEFIBER_PNT	69

// 线缆特征
#define REC_TYPE_FEAT_PIPE		70

// 特征
#define REC_TYPE_FEAT			71

// 配置路径与实例显隐映射
#define REC_TYPE_PLCDISPLAY		72

// 空间点
#define REC_TYPE_SPACIALPOINT	73

// 用户自定义数据
#define REC_TYPE_USERDEFDATA	74

// 参考块
#define REC_TYPE_REFERENCEBLOCK	75

// 样本公差
#define REC_TYPE_TOLSPECIFICATION	76

// 样本公差容器
#define REC_TYPE_TOLSPECIFICATIONCONTAINER	77

// 几何公差
#define REC_TYPE_GEOMETRICALTOL				78
// 尺寸
#define REC_TYPE_DIMENSION					83

// 基准目标代号
#define REC_TYPE_DATUMTARGET				84
// 表面粗糙度
#define REC_TYPE_ROUGHNESS					85

// 焊接符号
#define REC_TYPE_WELDING					95

// 基准符号
#define REC_TYPE_DATUM						107

// 组件特征
#define REC_TYPE_COMPONENTFEATURE			108
#define REC_TYPE_VIRTUALFEATURE				109
#define REC_TYPE_ASSEMBLYFEATURE			110

// 孔的轴特征
#define REC_TYPE_FEATAXIS					111
// 孔的螺纹特征
#define REC_TYPE_FEATINNERTHREAD			112
// 孔特征
#define REC_TYPE_FEATHOLE					113
// 孔的Limit特征
#define REC_TYPE_FEATLIMIT					114
// 磁力线特征
#define REC_TYPE_FEAT_MAGNETICLINE			115
// 默认视图中实例相对于父级的配置矩阵
#define REC_TYPE_INS_PLACEMENT				116
// 要素名称
#define REC_TYPE_ENTITY_NAME				117
// 包络框
#define REC_TYPE_ENVELOPE					118
// 压缩记录
#define REC_TYPE_COMPRESS					119
// 图片(图章)
#define REC_TYPE_PICTURE					120
// 箭头
#define REC_TYPE_ARROW						121
// 直线箭头
#define REC_TYPE_LINE_ARROW					122
// 圆形箭头
#define REC_TYPE_CIRCULAR_ARROW				123
// 直角箭头
#define REC_TYPE_RIGHTANGLE_ARROW			124
// 纹理
#define REC_TYPE_TEXTURE					125
// 渲染
#define REC_TYPE_RENDER						126
// UV
#define REC_TYPE_UV							127
// 剖切线
#define REC_TYPE_CLIPPING_CURVE				128
// 标记特征
#define REC_TYPE_FEAT_MARK					129
// 矩形特征标记
#define REC_TYPE_MARK_RECTANGLE				130
// 椭圆特征标记
#define REC_TYPE_MARK_ELLIPSE				131
// 徒手画特征标记
#define REC_TYPE_MARK_FREEHAND				132
// 工艺BOM表格的每一栏
#define REC_TYPE_VIRTUALBOM_ITEM			133
// 灯光
#define REC_TYPE_LIGHTMAP					134
// 材质
#define REC_TYPE_MATERIAL					135



/*
 *	结构体长度
 */
#define	RECCTLLEN				12				/*!< sizeof(StkControl)				*/
#define	HEADLEN					28				/*!< sizeof(StkSegmentHead)			*/
#define	HEADSEEKLEN				8				/*!< recnum+datlen					*/
#define	STK_MAXNAME				64
#define	STK_ULTRANAME			256
#define	STK_MAXPATH				512

/*
 *  记录控制
 */
typedef  struct {
	STK_UINT32		type;						/*!< 记录类型						*/
	STK_UINT32		length;						/*!< 记录长度						*/
	STK_UINT32		version;					/*!< 记录版本						*/
} StkControl;

/*
 *  Segment Head Structure
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		id;							/*!< 数据表ID						*/
	STK_UINT32		kind;						/*!< 数据表类型						*/
	STK_UINT32		recnum;						/*!< 记录数量						*/
	STK_UINT32		datlen;						/*!< 所有记录的长度					*/
} StkSegmentHead;

/*
 * 文件头
 */
typedef  struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		FileTitle[STK_MAXNAME];		/*!< 文件标志 定为SVLFILE			*/
	STK_WCHAR		FilePathName[STK_MAXPATH];	/*!< 数据源-完整路径				*/
	STK_WCHAR		FileName[STK_MAXNAME];		/*!< 数据源-文件名称				*/
	STK_WCHAR		DataSource[STK_MAXNAME];	/*!< 数据源-软件名称、版本			*/
	STK_WCHAR		ConvTookit[STK_MAXNAME];	/*!< 数据交换类库-名称、版本		*/
	STK_WCHAR		SVLConversion[STK_MAXNAME];	/*!< SVL转换器-名称、版本			*/
	STK_WCHAR		SVLVersion[STK_MAXNAME];	/*!< SVL文件版本 如 2.0001			*/
	STK_WCHAR		SVLStorage[STK_MAXNAME];	/*!< SVL存储方式 如 64bit 分散/统一	*/
	STK_WCHAR		CreateTime[STK_MAXNAME];	/*!< SVL生成时间 如 14:16 2013/8/31	*/
	STK_WCHAR		Compression[STK_MAXNAME];	/*!< SVL压缩算法 如 Zip				*/
	STK_UINT64		PolygonNum;					/*!< 存储的面片数量					*/
	STK_UINT64		ProtoTypeNum;				/*!< 存储的零件数量					*/
	STK_UINT32		TopProtoID;					/*!< 顶级ProtoTypeID				*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
} StkTitleRec;

/*
 * 文件头
 */
typedef  struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		FileTitle[STK_MAXNAME];		/*!< 文件标志 定为SVLFILE			*/
	STK_WCHAR		ConvTookit[STK_MAXNAME];	/*!< 数据交换类库-名称、版本		*/
	STK_WCHAR		SVLConversion[STK_MAXNAME];	/*!< SVL转换器-名称、版本			*/
	STK_WCHAR		SVLVersion[STK_MAXNAME];	/*!< SVL文件版本 如 2.0001			*/
	STK_WCHAR		SVLStorage[STK_MAXNAME];	/*!< SVL存储方式 如 64bit 分散/统一	*/
	STK_WCHAR		CreateTime[STK_MAXNAME];	/*!< SVL生成时间 如 14:16 2013/8/31	*/
	STK_WCHAR		Compression[STK_MAXNAME];	/*!< SVL压缩算法 如 Zip				*/
	STK_UINT64		ProtoTypeNum;				/*!< 存储的零件数量					*/
	STK_UINT32		TopProtoID;					/*!< 顶级ProtoTypeID				*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
} StkTitleRec2;

/*
 * 文件头
 */
typedef  struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		FileTitle[STK_MAXNAME];		/*!< 文件标志 定为SVLFILE			*/
	STK_WCHAR		CADFileName[STK_MAXNAME];	/*!< 数据源-文件名称				*/
	STK_WCHAR		DataSource[STK_MAXNAME];	/*!< 数据源-软件名称、版本			*/
	STK_WCHAR		ConvTookit[STK_MAXNAME];	/*!< 数据交换类库-名称、版本		*/
	STK_WCHAR		SVLConversion[STK_MAXNAME];	/*!< SVL转换器-名称、版本			*/
	STK_WCHAR		SVLVersion[STK_MAXNAME];	/*!< SVL文件版本 如 2.0001			*/
	STK_WCHAR		CreateTime[STK_MAXNAME];	/*!< SVL生成时间 如 14:16 2013/8/31	*/
	STK_WCHAR		Compression[STK_MAXNAME];	/*!< SVL压缩算法 如 Zip				*/
	STK_WCHAR		CADFilePath[STK_MAXPATH];	/*!< 数据源-完整路径				*/
	STK_UINT64		PolygonNum;					/*!< 存储的面片数量					*/
	STK_UINT64		ProtoTypeNum;				/*!< 存储的零件数量					*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
} StkTitleRec3;

/*
 * 文件头
 */
typedef  struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		FileTitle[STK_MAXNAME];		/*!< 文件标志 定为SVLFILE			*/
	STK_WCHAR		ConvTookit[STK_MAXNAME];	/*!< 数据交换类库-名称、版本		*/
	STK_WCHAR		SVLConversion[STK_MAXNAME];	/*!< SVL转换器-名称、版本			*/
	STK_WCHAR		SVLVersion[STK_MAXNAME];	/*!< SVL文件版本 如 2.0001			*/
	STK_WCHAR		CreateTime[STK_MAXNAME];	/*!< SVL生成时间 如 14:16 2013/8/31	*/
	STK_WCHAR		Compression[STK_MAXNAME];	/*!< SVL压缩算法 如 Zip				*/
	STK_WCHAR		DataSource[STK_ULTRANAME];	/*!< 数据源-软件名称、版本			*/
	STK_WCHAR		CADFileName[STK_ULTRANAME];	/*!< 数据源-文件名称				*/
	STK_WCHAR		CADFilePath[STK_MAXPATH];	/*!< 数据源-完整路径				*/
	STK_UINT64		PolygonNum;					/*!< 存储的面片数量					*/
	STK_UINT64		ProtoTypeNum;				/*!< 存储的零件数量					*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
} StkTitleRec4;

/*
 * 文件头
 */
typedef  struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		FileTitle[STK_MAXNAME];		/*!< 文件标志 定为SVLFILE			*/
	STK_WCHAR		ConvTookit[STK_MAXNAME];	/*!< 数据交换类库-名称、版本		*/
	STK_WCHAR		SVLConversion[STK_MAXNAME];	/*!< SVL转换器-名称、版本			*/
	STK_WCHAR		SVLVersion[STK_MAXNAME];	/*!< SVL文件版本 如 2.0001			*/
	STK_WCHAR		CreateTime[STK_MAXNAME];	/*!< SVL生成时间 如 14:16 2013/8/31	*/
	STK_WCHAR		Compression[STK_MAXNAME];	/*!< SVL压缩算法 如 Zip				*/
	STK_WCHAR		DataSource[STK_ULTRANAME];	/*!< 数据源-软件名称、版本			*/
	STK_WCHAR		CADFileName[STK_ULTRANAME];	/*!< 数据源-文件名称				*/
	STK_WCHAR		CADFilePath[STK_MAXPATH];	/*!< 数据源-完整路径				*/
	STK_UINT64		PolygonNum;					/*!< 存储的面片数量					*/
	STK_UINT64		ProtoTypeNum;				/*!< 存储的零件数量					*/
	STK_UINT32		nuCompressLevel;			/*!< 压缩等级						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
} StkTitleRec5;

/*
 * 文件头
 */
typedef  struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		FileTitle[STK_MAXNAME];		/*!< 文件标志 定为SVLFILE			*/
	STK_WCHAR		ConvTookit[STK_MAXNAME];	/*!< 数据交换类库-名称、版本		*/
	STK_WCHAR		SVLConversion[STK_MAXNAME];	/*!< SVL转换器-名称、版本			*/
	STK_WCHAR		SVLVersion[STK_MAXNAME];	/*!< SVL文件版本 如 2.0001			*/
	STK_WCHAR		CreateTime[STK_MAXNAME];	/*!< SVL生成时间 如 14:16 2013/8/31	*/
	STK_WCHAR		Compression[STK_MAXNAME];	/*!< SVL压缩算法 如 Zip				*/
	STK_WCHAR		DataSource[STK_ULTRANAME];	/*!< 数据源-软件名称、版本			*/
	STK_WCHAR		CADFileName[STK_ULTRANAME];	/*!< 数据源-文件名称				*/
	STK_WCHAR		CADFilePath[STK_MAXPATH];	/*!< 数据源-完整路径				*/
	STK_WCHAR		Password[STK_MAXPATH];		/*!< 文件密码						*/
	STK_UINT64		PolygonNum;					/*!< 存储的面片数量					*/
	STK_UINT64		ProtoTypeNum;				/*!< 存储的零件数量					*/
	STK_UINT32		nuCompressLevel;			/*!< 压缩等级						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
} StkTitleRec6;

/*
 * 文件配置项
 */
typedef  struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		Optnum;						/*!< 选项数量						*/
	STK_WCHAR		Optstring[1][STK_MAXNAME];	/*!< 选项字符串数组					*/
} StkOptionRec;

/*
 * 缩略图
 */
typedef  struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		filetype;					/*!< 图片类型						*/
	STK_UINT32		buffsize;					/*!< 缓冲大小						*/
	STK_CHAR		buff[1];					/*!< 缓冲区（二进制）				*/
} StkThumbNailRec;

/*
 * Body Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		hasItem;					/*!< has color/mat					*/
	STK_FLOAT32		color[4];					/*!< body color						*/
	STK_FLOAT32		mat[3];						/*!< body material 					*/
	STK_UINT32		disp;						/*!< =0 隐藏  =1 显示				*/
	STK_UINT32		meshnum;					/*!< mesh num						*/
} StkBodyRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		hasItem;					/*!< has color/mat					*/
	STK_FLOAT32		color[4];					/*!< body color						*/
	STK_FLOAT32		mat[3];						/*!< body material 					*/
	STK_UINT32		disp;						/*!< =0 隐藏  =1 显示				*/
	STK_UINT32		meshnum;					/*!< mesh num						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
} StkBodyRec2;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		hasItem;					/*!< has color/mat					*/
	STK_FLOAT32		color[4];					/*!< body color						*/
	STK_FLOAT32		mat[3];						/*!< body material 					*/
	STK_UINT32		disp;						/*!< =0 隐藏  =1 显示				*/
	STK_UINT32		meshnum;					/*!< mesh num						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_WCHAR		chName[STK_MAXNAME];		/*!< Body名字						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
} StkBodyRec3;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		hasItem;					/*!< has color/mat					*/
	STK_FLOAT32		color[4];					/*!< body color						*/
	STK_FLOAT32		mat[3];						/*!< body material 					*/
	STK_UINT32		disp;						/*!< =0 隐藏  =1 显示				*/
	STK_UINT32		meshnum;					/*!< mesh num						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_WCHAR		chName[STK_ULTRANAME];		/*!< Body名字						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
} StkBodyRec4;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		hasItem;					/*!< has color/mat					*/
	STK_FLOAT32		color[4];					/*!< body color						*/
	STK_FLOAT32		mat[3];						/*!< body material 					*/
	STK_UINT32		disp;						/*!< =0 隐藏  =1 显示				*/
	STK_UINT32		meshnum;					/*!< mesh num						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
} StkBodyRec5;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		hasItem;					/*!< has color/mat					*/
	STK_FLOAT32		color[4];					/*!< body color						*/
	STK_ID			idMaterial;					/*!< 材质 ID						*/
	STK_UINT32		disp;						/*!< =0 隐藏  =1 显示				*/
	STK_UINT32		meshnum;					/*!< mesh num						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
} StkBodyRec6;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		nuNumSubLine;				/*!< 辅助线数量						*/
}StkSubLineRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		nuNumLine;					/*!< 线集中线的数量					*/
	STK_UINT32		nuDispStat;					/*!< 线集显隐						*/
	STK_RGBA32		rgbaLineSet;				/*!< 线集颜色						*/
	STK_WCHAR		chLineSetName[STK_MAXNAME];	/*!< 线集名字						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkLineSet;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		nuNumLine;					/*!< 线集中线的数量					*/
	STK_UINT32		nuDispStat;					/*!< 线集显隐						*/
	STK_RGBA32		rgbaLineSet;				/*!< 线集颜色						*/
	STK_WCHAR		chLineSetName[STK_MAXNAME];	/*!< 线集名字						*/
	STK_UINT32		nuType;						/*!< 线集类型						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkLineSetRec2;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		nuNumLine;					/*!< 线集中线的数量					*/
	STK_UINT32		nuDispStat;					/*!< 线集显隐						*/
	STK_RGBA32		rgbaLineSet;				/*!< 线集颜色						*/
	STK_WCHAR		chLineSetName[STK_ULTRANAME];/*!< 线集名字						*/
	STK_UINT32		nuType;						/*!< 线集类型						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkLineSetRec3;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		nuNumLine;					/*!< 线集中线的数量					*/
	STK_UINT32		nuDispStat;					/*!< 线集显隐						*/
	STK_RGBA32		rgbaLineSet;				/*!< 线集颜色						*/
	STK_UINT32		nuType;						/*!< 线集类型						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkLineSetRec4;

//---------------------------Just for LOD---------------------------------
/*
 * Mesh Record
 */
#define  STK_HAS_COLOR			0x00000001
#define  STK_HAS_BOX			0x00000002
#define  STK_HAS_MAT			0x00000004
#define  STK_HAS_RENDER			0x00000008
#define  STK_IS_CLOSED			0x00000001
#define  STK_IS_UNICOLOR		0x00000002
#define  STK_IS_ONTOP			0x00000004
#define  STK_IS_DISPLAY			0x00001000	//废弃
#define  STK_IS_NO_DISPLAY		0x00000008
// NOTE：StkMeshRec2默认支持LOD方式一
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		IsItem;						/*!< is closed/unicolor				*/
	STK_UINT32		hasItem;					/*!< has color/Mat/Box				*/
	STK_FLOAT32		color[4];					/*!< mesh color						*/
	STK_FLOAT32		mat[3];						/*!< mesh material 					*/
	STK_FLOAT32		box[2][3];					/*!< mesh box						*/
	STK_UINT32		facenum;					/*!< face num						*/
} StkMeshRec2;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		IsItem;						/*!< is closed/unicolor/display		*/
	STK_UINT32		hasItem;					/*!< has color/Mat/Box				*/
	STK_UINT32		meshType;					/*!< MESH存储类型					*/
												/*!< =1 不支持LOD方式				*/
												/*!< =2 LOD方式一：不共享点列		*/
												/*!< =3 LOD方式二：共享点列			*/
	STK_FLOAT32		color[4];					/*!< mesh color						*/
	STK_FLOAT32		mat[3];						/*!< mesh material 					*/
	STK_FLOAT32		box[2][3];					/*!< mesh box						*/
	STK_UINT32		facenum;					/*!< face num						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		product;					/*!< 产品类型						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		dmy3;						/*!< 预留3							*/
} StkMeshRec3;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		IsItem;						/*!< is closed/unicolor/display		*/
	STK_UINT32		hasItem;					/*!< has color/Mat/Box				*/
	STK_UINT32		meshType;					/*!< MESH存储类型					*/
												/*!< =1 不支持LOD方式				*/
												/*!< =2 LOD方式一：不共享点列		*/
												/*!< =3 LOD方式二：共享点列			*/
	STK_FLOAT32		color[4];					/*!< mesh color						*/
	STK_FLOAT32		mat[3];						/*!< mesh material 					*/
	STK_FLOAT32		box[2][3];					/*!< mesh box						*/
	STK_UINT32		facenum;					/*!< face num						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		product;					/*!< 产品类型						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		dmy3;						/*!< 预留3							*/
} StkMeshRec4;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		IsItem;						/*!< is closed/unicolor/display		*/
	STK_UINT32		hasItem;					/*!< has color/Mat/Box/Render		*/
	STK_UINT32		meshType;					/*!< MESH存储类型					*/
												/*!< =1 不支持LOD方式				*/
												/*!< =2 LOD方式一：不共享点列		*/
												/*!< =3 LOD方式二：共享点列			*/
	STK_FLOAT32		color[4];					/*!< mesh color						*/
	STK_FLOAT32		mat[3];						/*!< mesh material 					*/
	STK_FLOAT32		box[2][3];					/*!< mesh box						*/
	STK_UINT32		facenum;					/*!< face num						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		product;					/*!< 产品类型						*/
	STK_ID			idRender;					/*!< Render ID						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		dmy3;						/*!< 预留3							*/
} StkMeshRec5;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		IsItem;						/*!< is closed/unicolor/display		*/
	STK_UINT32		hasItem;					/*!< has color/Box/					*/
	STK_UINT32		meshType;					/*!< MESH存储类型					*/
												/*!< =1 不支持LOD方式				*/
												/*!< =2 LOD方式一：不共享点列		*/
												/*!< =3 LOD方式二：共享点列			*/
	STK_FLOAT32		color[4];					/*!< mesh color						*/
	STK_FLOAT32		box[2][3];					/*!< mesh box						*/
	STK_UINT32		facenum;					/*!< face num						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		product;					/*!< 产品类型						*/
	STK_ID			idMaterial;					/*!< Material ID					*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		dmy3;						/*!< 预留3							*/
} StkMeshRec6;

/*
 *	Mesh LOD Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		LODLevel;					/*!< LOD Level						*/
	STK_FLOAT32		AngularTol;					/*!< Angular Tolerance				*/
	STK_FLOAT32		ChordError;					/*!< Chord Error					*/
	STK_UINT32		PolygonNum;					/*!< polygon num					*/
	STK_UINT32		FloatNum;					/*!< float num of vertex			*/
	STK_FLOAT32		atr[1];						/*!< float data						*/
}StkMeshLODRec;

/*
 *	UV Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		FloatNum;					/*!< float num of UV				*/
	STK_FLOAT32		atr[1];						/*!< float data						*/
}StkUVRec;


/*
 * MeshFace Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		faceId;						/*!< face id						*/
	STK_UINT32		hasItem;					/*!< has color/Mat/Box				*/
	STK_FLOAT32		color[4];					/*!< face color						*/
	STK_FLOAT32		mat[3];						/*!< face material 					*/
	STK_FLOAT32		box[2][3];					/*!< face box						*/
} StkFaceRec2;

/*
 * MeshFace Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		faceId;						/*!< face id						*/
	STK_UINT32		hasItem;					/*!< has color/Mat/Box				*/
	STK_FLOAT32		color[4];					/*!< face color						*/
	STK_FLOAT32		mat[3];						/*!< face material 					*/
	STK_FLOAT32		box[2][3];					/*!< face box						*/
	STK_UINT32		MeshEdgeNum;				/*!< MeshEdge Number				*/
	STK_ID			atr[1];						/*!< uint data						*/
} StkFaceRec3;

/*
 * MeshFace Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		faceId;						/*!< face id						*/
	STK_UINT32		hasItem;					/*!< has color/Mat/Box/Render		*/
	STK_FLOAT32		color[4];					/*!< face color						*/
	STK_FLOAT32		mat[3];						/*!< face material 					*/
	STK_FLOAT32		box[2][3];					/*!< face box						*/
	STK_ID			idRender;					/*!< Render ID						*/
	STK_UINT32		MeshEdgeNum;				/*!< MeshEdge Number				*/
	STK_ID			atr[1];						/*!< uint data						*/
} StkFaceRec4;

/*
 * MeshFace Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		faceId;						/*!< face id						*/
	STK_UINT32		hasItem;					/*!< has color/Mat/Box/Render		*/
	STK_FLOAT32		color[4];					/*!< face color						*/
	STK_FLOAT32		box[2][3];					/*!< face box						*/
	STK_ID			idMaterial;					/*!< 材质 ID						*/
	STK_UINT32		MeshEdgeNum;				/*!< MeshEdge Number				*/
	STK_ID			atr[1];						/*!< uint data						*/
} StkFaceRec5;

/*
 * MeshFace PlaneFace Attribute Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		AttrType;					/*!< face attribute type			*/
	STK_ID			ID;							/*!< Face ID						*/
	STK_DIR32		Normal;						/*!< Face Normal					*/
	STK_PNT32		Origin;						/*!< Face Origin					*/
}StkPlnFaceAttrRec;

/*
 * MeshFace CylinderFace Attribute Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		AttrType;					/*!< face attribute type			*/
	STK_ID			ID;							/*!< Face ID						*/
	STK_DOUBLE64	Radius;						/*!< Radius at origin				*/
	STK_DIR32		RevoAxis;					/*!< Revolution Axis				*/
	STK_PNT32		AxisOrigin;					/*!< Origin of Axis					*/
}StkCylinFaceAttrRec;

/*
 * MeshFace ConicalFace Attribute Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		AttrType;					/*!< face attribute type			*/
	STK_ID			ID;							/*!< Face ID						*/
	STK_DOUBLE64	Radius;						/*!< Radius at origin				*/
	STK_DIR32		RevoAxis;					/*!< Revolution Axis				*/
	STK_PNT32		AxisOrigin;					/*!< Origin of Axis					*/
}StkConicalFaceAttrRec;

/*
 * MeshFace SphereFace Attribute Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		AttrType;					/*!< face attribute type			*/
	STK_ID			ID;							/*!< Face ID						*/
	STK_DOUBLE64	Radius;						/*!< Radius at origin				*/
	STK_DIR32		RevoAxis;					/*!< Revolution Axis				*/
	STK_PNT32		AxisOrigin;					/*!< Origin of Axis					*/
}StkSphereFaceAttrRec;

/*
 * MeshFace RevolutionFace Attribute Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		AttrType;					/*!< face attribute type			*/
	STK_ID			ID;							/*!< Face ID						*/
	STK_DOUBLE64	Radius;						/*!< Radius at origin				*/
	STK_DIR32		RevoAxis;					/*!< Revolution Axis				*/
	STK_PNT32		AxisOrigin;					/*!< Origin of Axis					*/
}StkRevoFaceAttrRec;

/*
 * MeshFace ToroidalFace Attribute Record
 */
typedef struct{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		AttrType;					/*!< face attribute type			*/
	STK_ID			ID;							/*!< Face ID						*/
	STK_DOUBLE64	Radius;						/*!< Radius at origin				*/
	STK_DIR32		RevoAxis;					/*!< Revolution Axis				*/
	STK_PNT32		AxisOrigin;					/*!< Origin of Axis					*/
	STK_FLOAT32		MajorRadius;				/*!< Outer Radius					*/
	STK_FLOAT32		MinorRadius;				/*!< Inner Radius					*/
}StkTorFaceAttrRec;

/*
 *	MeshFace LOD Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		FaceId;						/*!< Face Id						*/
	STK_UINT32		LODLevel;					/*!< LOD Level						*/
	STK_UINT32		Edgenum;					/*!< Edge num						*/
	STK_UINT32		UintNum;					/*!< uint num of triangle index		*/
	STK_UINT32		atr[1];						/*!< float data						*/
}StkFaceLODRec;

/*
 * MeshEdge Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		EdgeId;						/*!< edge id						*/
	STK_UINT32		hasItem;					/*!< has color/Box					*/
	STK_FLOAT32		color[4];					/*!< Edge color						*/
	STK_FLOAT32		box[2][3];					/*!< edge box						*/
} StkMeshEdgeRec;

/*
 * MeshEdge Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		EdgeId;						/*!< edge id						*/
	STK_UINT32		hasItem;					/*!< has color/Box					*/
	STK_FLOAT32		color[4];					/*!< Edge color						*/
	STK_FLOAT32		box[2][3];					/*!< edge box						*/
	STK_FLOAT32		fWidth;						/*!< edge width						*/
} StkMeshEdgeRec2;

/*
 * MeshEdge Line Attribute Record
 */
typedef struct {
	StkControl		Control;			/*!< Table Control				*/
	STK_UINT32		AttrType;			/*!< Edge attribute type		*/
	STK_ID			ID;					/*!< Edge ID					*/
	STK_PNT32		pntStart;			/*!< Start Point				*/
	STK_PNT32		pntEnd;				/*!< End Point					*/
	STK_PNT32		pntCenter;			/*!< Center Point				*/
	STK_FLOAT32		fLength;			/*!< Edge Length				*/
	STK_DIR32		dirDirect;			/*!< Edge Direction				*/
}StkLineAttrRec;

/*
 * MeshEdge Ellipse Attribute Record
 */
typedef struct {
	StkControl		Control;			/*!< Table Control				*/
	STK_UINT32		AttrType;			/*!< Edge attribute type		*/
	STK_ID			ID;					/*!< Edge ID					*/
	STK_PNT32		pntStart;			/*!< Start Point				*/
	STK_PNT32		pntEnd;				/*!< End Point					*/
	STK_PNT32		pntCenter;			/*!< Center Point				*/
	STK_FLOAT32		fMajorRadius;		/*!< Major Radius				*/
	STK_FLOAT32		fMinorRadius;		/*!< Minor Radius				*/
	STK_DIR32		XDir;				/*!< X方向向量					*/
	STK_DIR32		YDir;				/*!< Y方向向量					*/
	STK_DIR32		ZDir;				/*!< Z方向向量					*/
}StkEllipseAttrRec;

/*
 *	MeshEdge LOD Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		EdgeId;						/*!< Edge id						*/
	STK_UINT32		LODLevel;					/*!< LOD Level						*/
	STK_UINT32		uintnum;					/*!< uint num of polyline index		*/
	STK_UINT32		atr[1];						/*!< float data						*/
} StkEdgeLODRec;
//------------------------------------------------------------------------

/*
 * ProtoType Record
 */
typedef struct {
	STK_UINT32		childId;					/*!< Child ProtoType ID				*/
	STK_UINT32		plcId;						/*!< Placement ID					*/
	STK_FLOAT32		Matrix[4][4];				/*!< Placement Matrix				*/
	STK_WCHAR		InsName[STK_MAXNAME];		/*!< Child Instance Name			*/
} StkProtChild;

typedef struct {
	STK_UINT32		childId;					/*!< Child ProtoType ID				*/
	STK_UINT32		plcId;						/*!< Placement ID					*/
	STK_FLOAT32		Matrix[4][4];				/*!< Placement Matrix				*/
	STK_WCHAR		InsName[STK_MAXNAME];		/*!< Child Instance Name			*/
	STK_WCHAR		filePath[STK_MAXPATH];		/*!< File Path						*/
} StkProtChild2;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< ProtoType Name					*/
	STK_UINT32		LODnum;						/*!< LOD Num						*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< ProtoType Name					*/
	STK_UINT32		LODnum	;					/*!< LOD Num						*/
	STK_UINT32		types;						/*!< 文件类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType2;


typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< ProtoType Name					*/
	STK_UINT32		LODnum	;					/*!< LOD Num						*/
	STK_UINT32		types;						/*!< 文件类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild2	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType3;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< ProtoType Name					*/
	STK_UINT32		LODnum	;					/*!< LOD Num						*/
	STK_UINT32		types;						/*!< 文件类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild2	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType4;


typedef struct {
	STK_UINT32		childId;					/*!< Child ProtoType ID				*/
	STK_UINT32		plcId;						/*!< Placement ID					*/
	STK_FLOAT32		Matrix[4][4];				/*!< Placement Matrix				*/
	STK_WCHAR		InsName[STK_ULTRANAME];		/*!< Child Instance Name(256字符)	*/
	STK_WCHAR		filePath[STK_MAXPATH];		/*!< File Path						*/
} StkProtChild3;

typedef struct {
	STK_UINT32		childId;					/*!< Child ProtoType ID				*/
	STK_UINT32		nuType;						/*!< Child ProtoType Type			*/
	STK_UINT32		plcId;						/*!< Placement ID					*/
	STK_FLOAT32		Matrix[4][4];				/*!< Placement Matrix				*/
	STK_WCHAR		InsName[STK_ULTRANAME];		/*!< Child Instance Name(256字符)	*/
	STK_WCHAR		ProtoName[STK_ULTRANAME];	/*!< 子Proto的名字					*/
	STK_WCHAR		filePath[STK_MAXPATH];		/*!< File Path						*/
} StkProtChild4;

typedef struct {
	STK_UINT32		childId;					/*!< Child ProtoType ID				*/
	STK_UINT32		nuType;						/*!< Child ProtoType Type			*/
	STK_UINT32		plcId;						/*!< Placement ID					*/
	STK_INT32		nNumber;					/*!< 特征编号						*/
	STK_FLOAT32		Matrix[4][4];				/*!< Placement Matrix				*/
	STK_WCHAR		InsName[STK_ULTRANAME];		/*!< Child Instance Name(256字符)	*/
	STK_WCHAR		ProtoName[STK_ULTRANAME];	/*!< 子Proto的名字					*/
	STK_WCHAR		filePath[STK_MAXPATH];		/*!< File Path						*/
} StkProtChild5;

typedef struct {
	STK_UINT32		childId;					/*!< Child ProtoType ID				*/
	STK_UINT32		nuType;						/*!< Child ProtoType Type			*/
	STK_UINT32		plcId;						/*!< Placement ID					*/
	STK_INT32		nNumber;					/*!< 特征编号						*/
	STK_FLOAT32		Matrix[4][4];				/*!< Placement Matrix				*/
	STK_WCHAR		InsName[STK_ULTRANAME];		/*!< Child Instance Name(256字符)	*/
	STK_WCHAR		ProtoName[STK_ULTRANAME];	/*!< 子Proto的名字					*/
	STK_WCHAR		filePath[STK_MAXPATH];		/*!< File Path						*/
	STK_WCHAR		wchGUID[STK_MAXNAME];		/*!< 实例标识的GUID					*/
} StkProtChild6;

#define  STK_PROTO_IS_NODE_MODE		0x00000001
#define  STK_PROTO_IS_CATALOG		0x00000010
#define  STK_PROTO_IS_PIPE			0x00000100
#define  STK_PROTO_IS_TOP			0x00001000
#define  STK_PROTO_IS_USE_NUMBER	0x00010000
#define  STK_PROTO_HAS_COLOR		0x00100000
#define  STK_PROTO_IS_DISPLAY		0x01000000
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_ULTRANAME];		/*!< ProtoType Name(256字符)		*/
	STK_UINT32		LODnum	;					/*!< LOD Num						*/
	STK_UINT32		types;						/*!< 文件类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		IsItem;						/*!< is mode/catalog/Pipe		*/
	STK_UINT32		topNode;					/*!< 顶级Node ID					*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild3	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType5;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_ULTRANAME];		/*!< ProtoType Name(256字符)		*/
	STK_UINT32		LODnum	;					/*!< LOD Num						*/
	STK_UINT32		types;						/*!< 文件类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		IsItem;						/*!< is catalog/Pipe/Top	*/
	STK_UINT32		topNode;					/*!< 顶级Node ID					*/
	STK_DOUBLE64	UnitFactor	;				/*!< 单位							*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild3	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType6;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_ULTRANAME];		/*!< ProtoType Name(256字符)		*/
	STK_WCHAR		CADFilePath[STK_MAXPATH];	/*!< CAD File Path					*/
	STK_UINT32		LODnum	;					/*!< LOD Num						*/
	STK_UINT32		types;						/*!< 文件类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		IsItem;						/*!< is node mode/catalog			*/
	STK_UINT32		topNode;					/*!< 顶级Node ID					*/
	STK_DOUBLE64	UnitFactor	;				/*!< 单位							*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild4	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType7;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_ULTRANAME];		/*!< ProtoType Name(256字符)		*/
	STK_WCHAR		CADFilePath[STK_MAXPATH];	/*!< CAD File Path					*/
	STK_UINT32		LODnum	;					/*!< LOD Num						*/
	STK_UINT32		types;						/*!< 文件类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		IsItem;						/*!< is node mode/catalog			*/
	STK_UINT32		topNode;					/*!< 顶级Node ID					*/
	STK_DOUBLE64	UnitFactor	;				/*!< 单位							*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild5	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType8;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_ULTRANAME];		/*!< ProtoType Name(256字符)		*/
	STK_WCHAR		CADFilePath[STK_MAXPATH];	/*!< CAD File Path					*/
	STK_UINT32		LODnum	;					/*!< LOD Num						*/
	STK_UINT32		types;						/*!< 文件类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		IsItem;						/*!< is node mode/catalog			*/
	STK_UINT32		topNode;					/*!< 顶级Node ID					*/
	STK_DOUBLE64	UnitFactor	;				/*!< 单位							*/
	STK_RGBA32		Color;						/*!< ProtoType颜色					*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild5	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType9;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_ULTRANAME];		/*!< ProtoType Name(256字符)		*/
	STK_WCHAR		CADFilePath[STK_MAXPATH];	/*!< CAD File Path					*/
	STK_UINT32		LODnum	;					/*!< LOD Num						*/
	STK_UINT32		types;						/*!< 文件类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		IsItem;						/*!< is node mode/catalog			*/
	STK_UINT32		topNode;					/*!< 顶级Node ID					*/
	STK_DOUBLE64	UnitFactor	;				/*!< 单位							*/
	STK_RGBA32		Color;						/*!< ProtoType颜色					*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild6	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType10;

typedef struct {
	STK_UINT32		childId;					/*!< Child ProtoType ID				*/
	STK_UINT32		nuType;						/*!< Child ProtoType Type			*/
	STK_UINT32		plcId;						/*!< Placement ID					*/
	STK_INT32		nNumber;					/*!< 特征编号						*/
	STK_FLOAT32		Matrix[4][4];				/*!< Placement Matrix				*/
	STK_WCHAR		InsName[STK_ULTRANAME];		/*!< Child Instance Name(256字符)	*/
	STK_WCHAR		ProtoName[STK_ULTRANAME];	/*!< 子Proto的名字					*/
	STK_WCHAR		filePath[STK_MAXPATH];		/*!< File Path						*/
	STK_WCHAR		wchGUID[STK_MAXNAME];		/*!< 实例标识的GUID					*/
	STK_WCHAR		Password[STK_MAXPATH];		/*!< 子实例对应的文件密码			*/
} StkProtChild7;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_ULTRANAME];		/*!< ProtoType Name(256字符)		*/
	STK_WCHAR		CADFilePath[STK_MAXPATH];	/*!< CAD File Path					*/
	STK_UINT32		LODnum	;					/*!< LOD Num						*/
	STK_UINT32		types;						/*!< 文件类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		IsItem;						/*!< is node mode/catalog			*/
	STK_UINT32		topNode;					/*!< 顶级Node ID					*/
	STK_DOUBLE64	UnitFactor	;				/*!< 单位							*/
	STK_RGBA32		Color;						/*!< ProtoType颜色					*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild7	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType11;

typedef struct {
	STK_UINT32		childId;					/*!< Child ProtoType ID				*/
	STK_UINT32		nuType;						/*!< Child ProtoType Type			*/
	STK_UINT32		plcId;						/*!< Placement ID					*/
	STK_INT32		nNumber;					/*!< 特征编号						*/
	STK_FLOAT32		Matrix[4][4];				/*!< Placement Matrix				*/
	STK_WCHAR		InsName[STK_ULTRANAME];		/*!< Child Instance Name(256字符)	*/
	STK_WCHAR		ProtoName[STK_ULTRANAME];	/*!< 子Proto的名字					*/
	STK_WCHAR		filePath[STK_MAXPATH];		/*!< File Path						*/
	STK_WCHAR		wchGUID[STK_MAXNAME];		/*!< 实例标识的GUID					*/
	STK_WCHAR		Password[STK_MAXPATH];		/*!< 子实例对应的文件密码			*/
	STK_WCHAR		wchBomID[STK_MAXNAME];		/*!< 工艺BOM表格的序号				*/
} StkProtChild8;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_ULTRANAME];		/*!< ProtoType Name(256字符)		*/
	STK_WCHAR		CADFilePath[STK_MAXPATH];	/*!< CAD File Path					*/
	STK_UINT32		LODnum	;					/*!< LOD Num						*/
	STK_UINT32		types;						/*!< 文件类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		IsItem;						/*!< is node mode/catalog			*/
	STK_UINT32		topNode;					/*!< 顶级Node ID					*/
	STK_DOUBLE64	UnitFactor	;				/*!< 单位							*/
	STK_RGBA32		Color;						/*!< ProtoType颜色					*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
	STK_UINT32		childnum;					/*!< Child Num						*/
	StkProtChild8	childdat[1];				/*!< Child Data(n)					*/
} StkProtoType12;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		plcPath[STK_MAXPATH];		/*!< 实例配置路径					*/
	STK_UINT32		nuDisplayState;				/*!< 实例显隐状态					*/
}StkPlcPathDisplayRec;

/*
 * ProtoType头（已废弃）
 */
typedef  struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		FilePathName[STK_MAXPATH];	/*!< 数据源-完整路径					*/
	STK_WCHAR		FileName[STK_MAXNAME];		/*!< 数据源-文件名称					*/
	STK_WCHAR		DataSource[STK_MAXNAME];	/*!< 数据源-软件名称、版本			*/
	STK_UINT64		PolygonNum;					/*!< 存储的面片数量					*/
} StkProtoTitleRec;


typedef struct {
	StkControl			Control;						/*!< 记录控制						*/
	unsigned int		charnum;						/*!< Char Num						*/
	STK_WCHAR		atr[1];							/*!< ProtoType Key(n)				*/
} StkProtoKey;	

/*
 *  实例属性记录
 */
#define  STK_INS_DISP			0
#define  STK_INS_NODISP			1
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		charnum;					/*!< Char Num						*/
	STK_WCHAR		atr[1];						/*!< PlcPath(n)						*/
} StkInstanceAtr;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_FLOAT32		color[4];					/*!< Child Num						*/
} StkInstanceColor;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		disp;						/*!< =0 实例隐藏  =1 实例显示		*/
} StkInstanceDisplay;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_FLOAT32		mat[3];						/*!< material						*/
} StkInstanceMater;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_ID			idMaterial;					/*!< 材质 ID						*/
} StkInsMaterialRec2;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		plcId;						/*!< Placement ID					*/
	STK_FLOAT32		Matrix[4][4];				/*!< Placement Matrix				*/
} StkInsPlacement;

/*
 *	PMI Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_MTX32		DefPlane;					/*!< 定义平面						*/
	STK_UINT32		Type;						/*!< PMI类型						*/
	STK_RGBA32		Color;						/*!< PMI颜色						*/
	STK_UINT32		NumLeader;					/*!< 引出线数量						*/
	STK_UINT32		NumExLine;					/*!< 延长线数量						*/
}StkPMIBaseRec;

#define  STK_PMI_IS_DISPLAY			0x00000001
#define  STK_PMI_IS_DIM3D			0x00000010
#define  STK_PMI_IS_PARALLEL		0x00000100
#define  STK_PMI_IS_FIX				0x00001000
#define  STK_PMI_IS_OLDVERSION		0x00010000
#define  STK_PMI_IS_ADJUSTED		0x00100000
#define  STK_PMI_IS_FRONT			0x01000000
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_MTX32		DefPlane;					/*!< 定义平面						*/
	STK_UINT32		Type;						/*!< PMI类型						*/
	STK_RGBA32		Color;						/*!< PMI颜色						*/
	STK_UINT32		IsItem;						/*!< Is display/xxx					*/ // add on 2013.8.6 by gaoqq
	STK_UINT32		NumLeader;					/*!< 引出线数量						*/
	STK_UINT32		NumExLine;					/*!< 延长线数量						*/
}StkPMIBaseRec2;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_MTX32		DefPlane;					/*!< 定义平面						*/
	STK_UINT32		Type;						/*!< PMI类型						*/
	STK_RGBA32		Color;						/*!< PMI颜色						*/
	STK_UINT32		IsItem;						/*!< Is display/dim/parallel		*/
	STK_UINT32		NumLeader;					/*!< 引出线数量						*/
	STK_UINT32		NumExLine;					/*!< 延长线数量						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkPMIBaseRec3;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_MTX32		DefPlane;					/*!< 定义平面						*/
	STK_UINT32		Type;						/*!< PMI类型						*/
	STK_RGBA32		Color;						/*!< PMI颜色						*/
	STK_UINT32		IsItem;						/*!< Is display/dim/parallel/fix	*/
	STK_UINT32		NumLeader;					/*!< 引出线数量						*/
	STK_UINT32		NumExLine;					/*!< 延长线数量						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< PMI Name						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkPMIBaseRec4;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_MTX32		DefPlane;					/*!< 定义平面						*/
	STK_UINT32		Type;						/*!< PMI类型						*/
	STK_RGBA32		Color;						/*!< PMI颜色						*/
	STK_UINT32		IsItem;						/*!< Is display/dim/parallel/fix	*/
	STK_UINT32		NumLeader;					/*!< 引出线数量						*/
	STK_UINT32		NumExLine;					/*!< 延长线数量						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< PMI Name						*/
	STK_UINT32		nuSource;					/*!< PMI来源：转换/用户创建			*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkPMIBaseRec5;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_MTX32		DefPlane;					/*!< 定义平面						*/
	STK_UINT32		Type;						/*!< PMI类型						*/
	STK_RGBA32		Color;						/*!< PMI颜色						*/
	STK_UINT32		IsItem;						/*!< Is display/dim/parallel/fix/old*/
	STK_UINT32		NumLeader;					/*!< 引出线数量						*/
	STK_UINT32		NumExLine;					/*!< 延长线数量						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< PMI Name						*/
	STK_UINT32		nuSource;					/*!< PMI来源：转换/用户创建			*/
	STK_DIR32		dirX;						/*!< PMI X方向向量					*/
	STK_DIR32		dirY;						/*!< PMI Y方向向量					*/
	STK_PNT32		pntSymBase;					/*!< PMI 符号基准点					*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkPMIBaseRec6;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_MTX32		DefPlane;					/*!< 定义平面						*/
	STK_UINT32		Type;						/*!< PMI类型						*/
	STK_RGBA32		Color;						/*!< PMI颜色						*/
	STK_UINT32		IsItem;						/*!< Is display/dim/parallel/fix/old*/
	STK_UINT32		NumLeader;					/*!< 引出线数量						*/
	STK_UINT32		NumExLine;					/*!< 延长线数量						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_WCHAR		name[STK_ULTRANAME];		/*!< PMI Name						*/
	STK_UINT32		nuSource;					/*!< PMI来源：转换/用户创建			*/
	STK_DIR32		dirX;						/*!< PMI X方向向量					*/
	STK_DIR32		dirY;						/*!< PMI Y方向向量					*/
	STK_PNT32		pntSymBase;					/*!< PMI 符号基准点					*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkPMIBaseRec7;

typedef struct {
	StkControl		Control;					/*!< 记录控制								*/
	STK_MTX32		DefPlane;					/*!< 定义平面								*/
	STK_UINT32		Type;						/*!< PMI类型								*/
	STK_RGBA32		Color;						/*!< PMI颜色								*/
	STK_UINT32		IsItem;						/*!< Is display/dim/parallel/fix/old/front	*/
	STK_UINT32		NumLeader;					/*!< 引出线数量								*/
	STK_UINT32		NumExLine;					/*!< 延长线数量								*/
	STK_UINT32		usetype;					/*!< 使用方式								*/
	STK_UINT32		nuSource;					/*!< PMI来源：转换/用户创建					*/
	STK_DIR32		dirX;						/*!< PMI X方向向量							*/
	STK_DIR32		dirY;						/*!< PMI Y方向向量							*/
	STK_PNT32		pntSymBase;					/*!< PMI 符号基准点							*/
	STK_UINT32		dmy1;						/*!< 预留1									*/
	STK_UINT32		dmy2;						/*!< 预留2									*/
}StkPMIBaseRec8;

/*
 *	Curve PolyLine Record
 */
typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		ID;							/*!< Curve ID						*/
	STK_RGBA32		Color;						/*!< Curve颜色						*/
	STK_UINT32		Type;						/*!< Curve类型						*/
	STK_FLOAT32		CMin;						/*!< 起始范围						*/
	STK_FLOAT32		CMax;						/*!< 终止范围						*/
	STK_UINT32		NumPoints;					/*!< 点列数量						*/
	STK_FLOAT32		atr[1];						/*!< 存储点列区域					*/
}StkPolyLineRec;

/*
 *	Curve PolyLine Record  (Version 2)
 */
typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		ID;							/*!< Curve ID						*/
	STK_RGBA32		Color;						/*!< Curve颜色						*/
	STK_UINT32		Type;						/*!< Curve类型						*/
	STK_UINT32		DefType;					/*!< 定义类型						*/ // add on 2013.10.9 by gaoqq
	STK_FLOAT32		CMin;						/*!< 起始范围						*/
	STK_FLOAT32		CMax;						/*!< 终止范围						*/
	STK_UINT32		NumPoints;					/*!< 点列数量						*/
	STK_FLOAT32		atr[1];						/*!< 存储点列区域					*/
}StkPolyLineRec2;

/*
 *	Curve PolyLine Record  (Version 3)
 */
typedef struct	{
	StkControl	Control;						/*!< 记录控制						*/
	STK_UINT32	ID;								/*!< Curve ID						*/
	STK_RGBA32	Color;							/*!< Curve颜色						*/
	STK_UINT32	Type;							/*!< Curve类型						*/
	STK_UINT32	DefType;						/*!< 定义类型						*/
	STK_UINT32	nuDisp;							/*!< =0 隐藏  =1 显示				*/
	STK_UINT32	nuDispType;						/*!< 线条的显示类型（点线、连线等）	*/
	STK_FLOAT32	CMin;							/*!< 起始范围						*/
	STK_FLOAT32	CMax;							/*!< 终止范围						*/
	STK_UINT32	NumPoints;						/*!< 点列数量						*/
	STK_FLOAT32	atr[1];							/*!< 存储点列区域					*/
}StkPolyLineRec3;

/*
 *	Curve Ellipse Record
 */
typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		ID;							/*!< Curve ID						*/
	STK_RGBA32		Color;						/*!< Curve颜色						*/
	STK_UINT32		Type;						/*!< Curve类型						*/
	STK_FLOAT32		CMin;						/*!< 起始范围						*/
	STK_FLOAT32		CMax;						/*!< 终止范围						*/
	STK_PNT32		Center;						/*!< 中心点							*/
	STK_DIR32		XDir;						/*!< X方向向量						*/
	STK_DIR32		YDir;						/*!< Y方向向量						*/
	STK_DIR32		ZDir;						/*!< Z方向向量						*/
	STK_FLOAT32		MajorRadius;				/*!< 长半径							*/
	STK_FLOAT32		MinorRadius;				/*!< 短半径							*/
	STK_PNT32		BeginPnt;					/*!< 起点坐标位置					*/
	STK_PNT32		EndPnt;						/*!< 终点坐标位置					*/
	STK_FLOAT32		BeginPar;					/*!< 起点参数位置					*/
	STK_FLOAT32		EndPar;						/*!< 终点参数位置					*/
}StkEllipseRec;

/*
 *	Curve Ellipse Record (Version2)
 */
typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		ID;							/*!< Curve ID						*/
	STK_RGBA32		Color;						/*!< Curve颜色						*/
	STK_UINT32		Type;						/*!< Curve类型						*/ // add on 2013.10.9 by gaoqq
	STK_FLOAT32		CMin;						/*!< 起始范围						*/
	STK_FLOAT32		CMax;						/*!< 终止范围						*/
	STK_PNT32		Center;						/*!< 中心点							*/
	STK_DIR32		XDir;						/*!< X方向向量						*/
	STK_DIR32		YDir;						/*!< Y方向向量						*/
	STK_DIR32		ZDir;						/*!< Z方向向量						*/
	STK_DIR32		Normal;						/*!< 圆弧方向						*/
	STK_DIR32		OriginDir;					/*!< 原点方向						*/
	STK_FLOAT32		MajorRadius;				/*!< 长半径							*/
	STK_FLOAT32		MinorRadius;				/*!< 短半径							*/
	STK_PNT32		BeginPnt;					/*!< 起点坐标位置					*/
	STK_PNT32		EndPnt;						/*!< 终点坐标位置					*/
	STK_FLOAT32		BeginPar;					/*!< 起点参数位置					*/
	STK_FLOAT32		EndPar;						/*!< 终点参数位置					*/
}StkEllipseRec2;

/*
 *	Curve Ellipse Record (Version3)
 */
typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		ID;							/*!< Curve ID						*/
	STK_RGBA32		Color;						/*!< Curve颜色						*/
	STK_UINT32		Type;						/*!< Curve类型						*/
	STK_UINT32		DefType;					/*!< 定义类型						*/ // add on 2013.10.9 by gaoqq
	STK_FLOAT32		CMin;						/*!< 起始范围						*/
	STK_FLOAT32		CMax;						/*!< 终止范围						*/
	STK_PNT32		Center;						/*!< 中心点							*/
	STK_DIR32		XDir;						/*!< X方向向量						*/
	STK_DIR32		YDir;						/*!< Y方向向量						*/
	STK_DIR32		ZDir;						/*!< Z方向向量						*/
	STK_DIR32		Normal;						/*!< 圆弧方向						*/
	STK_DIR32		OriginDir;					/*!< 原点方向						*/
	STK_FLOAT32		MajorRadius;				/*!< 长半径							*/
	STK_FLOAT32		MinorRadius;				/*!< 短半径							*/
	STK_PNT32		BeginPnt;					/*!< 起点坐标位置					*/
	STK_PNT32		EndPnt;						/*!< 终点坐标位置					*/
	STK_FLOAT32		BeginPar;					/*!< 起点参数位置					*/
	STK_FLOAT32		EndPar;						/*!< 终点参数位置					*/
}StkEllipseRec3;

/*
 *	Curve Ellipse Record (Version4)
 */
typedef struct	{
	StkControl	Control;						/*!< 记录控制						*/
	STK_UINT32	ID;								/*!< Curve ID						*/
	STK_RGBA32	Color;							/*!< Curve颜色						*/
	STK_UINT32	Type;							/*!< Curve类型						*/
	STK_UINT32	DefType;						/*!< 定义类型						*/
	STK_UINT32	nuDisp;							/*!< =0 隐藏  =1 显示				*/
	STK_UINT32	nuDispType;						/*!< 线条的显示类型（点线、连线等）	*/
	STK_FLOAT32	CMin;							/*!< 起始范围						*/
	STK_FLOAT32	CMax;							/*!< 终止范围						*/
	STK_PNT32	Center;							/*!< 中心点							*/
	STK_DIR32	XDir;							/*!< X方向向量						*/
	STK_DIR32	YDir;							/*!< Y方向向量						*/
	STK_DIR32	ZDir;							/*!< Z方向向量						*/
	STK_DIR32	Normal;							/*!< 圆弧方向						*/
	STK_DIR32	OriginDir;						/*!< 原点方向						*/
	STK_FLOAT32	MajorRadius;					/*!< 长半径							*/
	STK_FLOAT32	MinorRadius;					/*!< 短半径							*/
	STK_PNT32	BeginPnt;						/*!< 起点坐标位置					*/
	STK_PNT32	EndPnt;							/*!< 终点坐标位置					*/
	STK_FLOAT32	BeginPar;						/*!< 起点参数位置					*/
	STK_FLOAT32	EndPar;							/*!< 终点参数位置					*/
}StkEllipseRec4;

/*
 *	Curve Line Record
 */
typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		ID;							/*!< Curve ID						*/
	STK_RGBA32		Color;						/*!< Curve颜色						*/
	STK_UINT32		Type;						/*!< Curve类型						*/
	STK_PNT32		StartPnt;					/*!< 起始点							*/
	STK_PNT32		EndPnt;						/*!< 终止点							*/
}StkLineRec;

/*
 *	Curve Line Record （version 2）
 */
typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		ID;							/*!< Curve ID						*/
	STK_RGBA32		Color;						/*!< Curve颜色						*/
	STK_UINT32		Type;						/*!< Curve类型						*/
	STK_UINT32		DefType;					/*!< 定义类型						*/
	STK_PNT32		StartPnt;					/*!< 起始点							*/
	STK_PNT32		EndPnt;						/*!< 终止点							*/
}StkLineRec2;

/*
 *	Curve Line Record （version 3）
 */
typedef struct	{
	StkControl	Control;						/*!< 记录控制						*/
	STK_UINT32	ID;								/*!< Curve ID						*/
	STK_RGBA32	Color;							/*!< Curve颜色						*/
	STK_UINT32	Type;							/*!< Curve类型						*/
	STK_UINT32	DefType;						/*!< 定义类型						*/
	STK_UINT32	nuDisp;							/*!< =0 隐藏  =1 显示				*/
	STK_UINT32	nuDispType;						/*!< 线条的显示类型（点线、连线等）	*/
	STK_PNT32	StartPnt;						/*!< 起始点							*/
	STK_PNT32	EndPnt;							/*!< 终止点							*/
}StkLineRec3;

/*
 *	Leader Record
 */
typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		LeaderType;					/*!< Leader类型						*/
	STK_UINT32		TermType;					/*!< 末端类型						*/
	STK_FLOAT32		Size[2];					/*!< 末端宽高						*/
	STK_PNT32		TermLoc;					/*!< 末端起始位置					*/
	STK_DIR32		TermDir;					/*!< 末端方向						*/
}StkLeaderRec;

typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		LeaderType;					/*!< Leader类型						*/
	STK_UINT32		TermType;					/*!< 末端类型						*/
	STK_FLOAT32		Size[2];					/*!< 末端宽高						*/
	STK_PNT32		TermLoc;					/*!< 末端起始位置					*/
	STK_DIR32		TermDir;					/*!< 末端方向						*/
	STK_UINT32		nuStub;						/*!< 是否有二维折弯线标识			*/
}StkLeaderRec2;

/*
 *	ExtensionLine Record
 */
typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		ExLineType;					/*!< ExtensionLine类型				*/
}StkExLineRec;

#define  STK_EXTLINE_IS_DISPLAY			0x00000001
typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		ExLineType;					/*!< ExtensionLine类型				*/
	STK_UINT32		IsItem;						/*!< Is display/xx					*/
}StkExLineRec2;

/*
 *	CompositeText Record
 */
typedef	struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		NumText;					/*!< Text数量						*/
	STK_BOX32		outBox;						/*!< 包围框							*/
}StkComTextRec;

typedef	struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		NumText;					/*!< Text数量						*/
	STK_BOX32		outBox;						/*!< 包围框							*/
	STK_UINT32		nuFrameType;				/*!< 外框类型						*/
}StkComTextRec2;

#define  STK_HAS_ENVELOPE_FRAME			0x00000001
typedef struct {
	STK_UINT32		nuAnchorType;				/*!< 锚点类型						*/
	STK_PNT32		pntAnchor;					/*!< 锚点类型对应的点				*/
}StkAnchorPoint;

// StkComTextRec3已废弃不再使用，只为兼容
typedef	struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		NumText;					/*!< Text数量						*/
	STK_BOX32		outBox;						/*!< 包围框							*/
	STK_UINT32		nuFrameType;				/*!< 外框类型						*/
	STK_UINT32		IsItem;						/*!< 是否有包络框/背景色			*/
	STK_RGBA32		rgbaBackground;				/*!< 包络框背景色					*/
	STK_UINT32		nuAnchorPointNum;			/*!< 锚点数量						*/
	StkAnchorPoint	atr[1];						/*!< 扩展区							*/
}StkComTextRec3;
// StkComTextRec4与StkComTextRec2相同，但是是为还原StkComTextRec3的问题
typedef	struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		NumText;					/*!< Text数量						*/
	STK_BOX32		outBox;						/*!< 包围框							*/
	STK_UINT32		nuFrameType;				/*!< 外框类型						*/
}StkComTextRec4;

typedef	struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		IsItem;						/*!< 是否有包络框的边框				*/
	STK_RGBA32		rgbaBackground;				/*!< 包络框背景色					*/
	STK_RGBA32		rgbaFrame;					/*!< 包络框边框的颜色				*/
	STK_UINT32		nuAnchorPointNum;			/*!< 锚点数量						*/
	StkAnchorPoint	atr[1];						/*!< 扩展区							*/
}StkEnvelopeRec;

/*
 *	Text Record
 */
typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_PNT32		InnerLoc;					/*!< 定位点							*/
	STK_DIR32		Axis[2];					/*!< 旋转向量						*/
	STK_FLOAT32		Size[2];					/*!< 字符宽高						*/
	STK_FLOAT32		CharDis;					/*!< 字符间距						*/
	STK_WCHAR		font[STK_MAXNAME];			/*!< 字符字体						*/
	STK_UINT32		NumChar;					/*!< 文本字符数						*/
	STK_WCHAR		atr[1];						/*!< 空位占用						*/
}StkTextRec;

typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_PNT32		InnerLoc;					/*!< 定位点							*/
	STK_DIR32		Axis[2];					/*!< 旋转向量						*/
	STK_FLOAT32		Size[2];					/*!< 字符宽高						*/
	STK_FLOAT32		CharDis;					/*!< 字符间距						*/
	STK_UINT32		usetype;					/*!< 应用类型						*/
	STK_WCHAR		font[STK_MAXNAME];			/*!< 字符字体						*/
	STK_UINT32		NumChar;					/*!< 文本字符数						*/
	STK_WCHAR		atr[1];						/*!< 空位占用						*/
}StkTextRec2;

typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_PNT32		InnerLoc;					/*!< 定位点							*/
	STK_DIR32		Axis[2];					/*!< 旋转向量						*/
	STK_FLOAT32		Size[2];					/*!< 字符宽高						*/
	STK_FLOAT32		CharDis;					/*!< 字符间距						*/
	STK_UINT32		usetype;					/*!< 应用类型						*/
	STK_BOX32		outBox;						/*!< 包围框							*/
	STK_WCHAR		font[STK_MAXNAME];			/*!< 字符字体						*/
	STK_UINT32		NumChar;					/*!< 文本字符数						*/
	STK_WCHAR		atr[1];						/*!< 空位占用						*/
}StkTextRec3;

typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_PNT32		InnerLoc;					/*!< 定位点							*/
	STK_DIR32		Axis[2];					/*!< 旋转向量						*/
	STK_DOUBLE64	dAngle;						/*!< 旋转角度						*/
	STK_FLOAT32		Size[2];					/*!< 字符宽高						*/
	STK_FLOAT32		CharDis;					/*!< 字符间距						*/
	STK_UINT32		usetype;					/*!< 应用类型						*/
	STK_BOX32		outBox;						/*!< 包围框							*/
	STK_WCHAR		font[STK_MAXNAME];			/*!< 字符字体						*/
	STK_UINT32		NumChar;					/*!< 文本字符数						*/
	STK_WCHAR		atr[1];						/*!< 空位占用						*/
}StkTextRec4;

typedef struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_PNT32		InnerLoc;					/*!< 定位点							*/
	STK_DIR32		Axis[2];					/*!< 旋转向量						*/
	STK_DOUBLE64	dAngle;						/*!< 旋转角度						*/
	STK_FLOAT32		Size[2];					/*!< 字符宽高						*/
	STK_FLOAT32		CharDis;					/*!< 字符间距						*/
	STK_UINT32		usetype;					/*!< 应用类型						*/
	STK_BOX32		outBox;						/*!< 包围框							*/
	STK_WCHAR		font[STK_MAXNAME];			/*!< 字符字体						*/
	STK_UINT32		nuFontStyle;				/*!< 字体风格						*/
	STK_UINT32		nuFontType;					/*!< 字体类型						*/
	STK_UINT32		nuTextAttribute;			/*!< 文本属性						*/
	STK_UINT32		NumChar;					/*!< 文本字符数						*/
	STK_WCHAR		atr[1];						/*!< 空位占用						*/
}StkTextRec5;

/*
 *	外框
 */
typedef	struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		FrameType;					/*!< 外框类型						*/
	STK_UINT32		NumCurve;					/*!< 记录Curve数量					*/
}StkFrameRec;

/*
 *	外框2
 */
typedef	struct	{
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		FrameType;					/*!< 外框类型						*/
	STK_UINT32		defType;					/*!< 定义类型						*/
	STK_UINT32		NumCurve;					/*!< 记录Curve数量					*/
}StkFrameRec2;

/*
 *	Special Line Record
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		SpecLineUse;				/*!< 记录特殊线的用途				*/
}StkSpecLinRec;

/*
 *  自定义视图
 */
#define  STK_VIEW_IS_ACTIVED	0x00000001
#define  STK_VIEW_HAS_CLIP		0x00000001

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< 视图名称						*/
	STK_UINT32		isItem;						/*!< 是否活动/XX					*/
	STK_UINT32		hasItem;					/*!< 有无剖面/XX					*/
}StkViewRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< 视图名称						*/
	STK_UINT32		isItem;						/*!< 是否活动/XX					*/
	STK_UINT32		hasItem;					/*!< 有无剖面/XX					*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkViewRec2;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< 视图名称						*/
	STK_UINT32		isItem;						/*!< 是否活动/XX					*/
	STK_UINT32		hasItem;					/*!< 有无剖面/XX					*/
	STK_UINT32		DefType;					/*!< 定义类型						*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkViewRec3;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< 视图名称						*/
	STK_UINT32		isItem;						/*!< 是否活动/XX					*/
	STK_UINT32		hasItem;					/*!< 有无剖面/XX					*/
	STK_UINT32		DefType;					/*!< 应用类型						*/
	STK_UINT32		usetype;					/*!< 定义方式						*/
	STK_FLOAT32		Transparency;				/**< 透明度							*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkViewRec4;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_WCHAR		name[STK_ULTRANAME];		/*!< 视图名称						*/
	STK_UINT32		isItem;						/*!< 是否活动/XX					*/
	STK_UINT32		hasItem;					/*!< 有无剖面/XX					*/
	STK_UINT32		DefType;					/*!< 应用类型						*/
	STK_UINT32		usetype;					/*!< 定义方式						*/
	STK_FLOAT32		Transparency;				/**< 透明度							*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkViewRec5;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		isItem;						/*!< 是否活动/XX					*/
	STK_UINT32		hasItem;					/*!< 有无剖面/XX					*/
	STK_UINT32		DefType;					/*!< 应用类型						*/
	STK_UINT32		usetype;					/*!< 定义方式						*/
	STK_FLOAT32		Transparency;				/**< 透明度							*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkViewRec6;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_ID			ID;							/*!< ID								*/
	STK_PNT32		OriginPoint;				/*!< 定位点							*/
	STK_DIR32		TargetVector;				/*!< Target Vector					*/
	STK_DIR32		UpVector;					/*!< Up Vector						*/
	STK_FLOAT32		Angle;						/*!< Angle							*/
	STK_FLOAT32		Focus;						/*!< Focus							*/
	STK_FLOAT32		ZoomFactor;					/*!< 缩放因子						*/
	STK_UINT32		ProjType;					/*!< 投影类型						*/
}StkCameraRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_ID			ID;							/*!< ID								*/
	STK_PNT32		OriginPoint;				/*!< 定位点							*/
	STK_FLOAT32		ZoomFactor;					/*!< 缩放因子						*/
	STK_UINT32		ProjType;					/*!< 投影类型						*/
	STK_MTX32		Matrix;						/*!< 旋转平移矩阵					*/
}StkCameraMatrixRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_ID			ID;							/*!< ID								*/
	STK_PNT32		OriginPoint;				/*!< 定位点							*/
	STK_DIR32		Normal;						/*!< 法向量							*/
	STK_DIR32		UVector;					/*!< U-Vector						*/
	STK_DIR32		VVector;					/*!< V-Vector						*/
}StkClipPlanRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_ID			ID;							/*!< ID								*/
	STK_PNT32		OriginPoint;				/*!< 定位点							*/
	STK_DIR32		Normal;						/*!< 法向量							*/
	STK_DIR32		UVector;					/*!< U-Vector						*/
	STK_DIR32		VVector;					/*!< V-Vector						*/
	STK_FLOAT32		fDistance;					/*!< 深度							*/
}StkClipPlanRec2;

//Node Connector Record
typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		PointingEntityType;			/*!< 要素类型						*/
	STK_UINT32		PointedEntityType;			/*!< 被指向要素类型					*/
}StkNodeLinkBaseRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		PointingEntityType;			/*!< 要素类型						*/
	STK_UINT32		PointedEntityType;			/*!< 被指向要素类型					*/
	STK_UINT32		usetype;					/*!< 使用方式						*/
	STK_UINT32		dmy1;						/*!< 预留1							*/
	STK_UINT32		dmy2;						/*!< 预留2							*/
}StkNodeLinkBaseRec2;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		NumChar;					/*!< 字符个数						*/
	STK_WCHAR		atr[1];						/*!< 空位占用						*/
}StkProtoLinkRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		NodeID;						/*!< Node节点对应的ID				*/
}StkNodeIDRec;

typedef struct {
	STK_ID			EntityID;					/*!< Entity ID						*/
	STK_UINT32		EntityType;					/*!< Entity Type					*/
}StkOneEntity;

typedef struct {
	STK_ID			EntityID;					/*!< Entity ID						*/
	STK_UINT32		EntityType;					/*!< Entity Type					*/
	STK_PNT32		pntPick;					/*!< Pick点：为攻长义应用层需要提供	*/
	STK_DIR32		dirPick;					/*!< Pick点方向向量					*/
}StkOneEntity2;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		NumSubEntity;				/*!< SubEntity数量					*/
	StkOneEntity	atr[1];						/*!< 空位占用						*/
}StkSubEntityRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制						*/
	STK_UINT32		NumSubEntity;				/*!< SubEntity数量					*/
	StkOneEntity2	atr[1];						/*!< 空位占用						*/
}StkSubEntityRec2;

/*
 *	3D批注
 */
typedef struct {
	StkControl		RecCtl;						//!< Table Control
	STK_UINT32		nuDispStat;					//!< 是否隐藏 =0 隐藏 =1 显示
	STK_RGBA32		rgbaNote;					//!< 批注颜色
	STK_UINT32		nuArrowType;				//!< 批注箭头类型
	STK_FLOAT32		fHeight;					//!< 箭头高度
	STK_FLOAT32		fRadius;					//!< 箭头基底面半径
	STK_DIR32		dirArrow;					//!< 箭头方向向量
	STK_PNT32		pntArrowPos;				//!< 箭头位置
	STK_PNT32		pntLineStart;				//!< 批注引线起点
	STK_PNT32		pntLineEnd;					//!< 批注引线终点
	STK_INT32		niIndexA[3];				//!< 批注引线索引
	STK_WCHAR		wchCreator[STK_MAXNAME];	//!< 创建者
	STK_WCHAR		wchCreTime[STK_MAXNAME];	//!< 创建时间
	STK_PNT32		pntTextPos;					//!< 文本显示位置
	STK_UINT32		nuCharNum;					//!< 文本字符数
	STK_WCHAR		atr[1];						//!< 占位
}StkNoteRec;

/*
 *	辅助线 Record
 */
/*
 *	NurbsCurve Record
 */
typedef struct {
	StkControl	Control;						/*!< 记录控制						*/
	STK_UINT32	ID;								/*!< Curve ID						*/
	STK_RGBA32	Color;							/*!< Curve颜色						*/
	STK_UINT32	nuType;							/*!< Curve类型						*/
	STK_UINT32	nuDefType;						/*!< 定义类型						*/
	STK_UINT32	nuDisp;							/*!< =0 隐藏  =1 显示				*/
	STK_UINT32	nuDispType;						/*!< 线条的显示类型（点线、连线等）	*/
	STK_FLOAT32	fUMin;							/*!< 起始范围						*/
	STK_FLOAT32	fUMax;							/*!< 终止范围						*/
	STK_UINT32	nuDegree;						/*!< degree值						*/
	STK_UINT32	nuNumKnotPlicity;				/*!< KnotPlicity数量				*/
	STK_UINT32	nuNumKnot;						/*!< Knot数量						*/
	STK_UINT32	nuNumPoint;						/*!< 控制点数量						*/
	STK_UINT32	nuNumWeight;					/*!< 权重数量						*/
	STK_FLOAT32	atr[1];
}StkNurbsCurveRec;

/*
 *	Hyperbola Record
 */
typedef struct {
	StkControl	Control;						/*!< 记录控制						*/
	STK_UINT32	ID;								/*!< Curve ID						*/
	STK_RGBA32	Color;							/*!< Curve颜色						*/
	STK_UINT32	nuType;							/*!< Curve类型						*/
	STK_UINT32	nuDefType;						/*!< 定义类型						*/
	STK_UINT32	nuDisp;							/*!< =0 隐藏  =1 显示				*/
	STK_UINT32	nuDispType;						/*!< 线条的显示类型（点线、连线等）	*/
	STK_FLOAT32	fUMin;							/*!< 起始范围						*/
	STK_FLOAT32	fUMax;							/*!< 终止范围						*/
	STK_PNT32	pntCenter;						/*!< 中心点							*/
	STK_DIR32	dirX;							/*!< X方向向量						*/
	STK_DIR32	dirY;							/*!< Y方向向量						*/
	STK_DIR32	dirZ;							/*!< Z方向向量						*/
	STK_FLOAT32	fSemiAxis;						/*!< 半轴							*/
	STK_FLOAT32	fSemiImgAxis;
	STK_PNT32	pntStart;						/*!< 起点坐标						*/
	STK_PNT32	pntEnd;							/*!< 终点坐标						*/
}StkHyperbolaRec;

/*
 *	Parabola Record
 */
typedef struct {
	StkControl	Control;						/*!< 记录控制						*/
	STK_UINT32	ID;								/*!< Curve ID						*/
	STK_RGBA32	Color;							/*!< Curve颜色						*/
	STK_UINT32	nuType;							/*!< Curve类型						*/
	STK_UINT32	nuDefType;						/*!< 定义类型						*/
	STK_UINT32	nuDisp;							/*!< =0 隐藏  =1 显示				*/
	STK_UINT32	nuDispType;						/*!< 线条的显示类型（点线、连线等）	*/
	STK_FLOAT32	fUMin;							/*!< 起始范围						*/
	STK_FLOAT32	fUMax;							/*!< 终止范围						*/
	STK_PNT32	pntStart;						/*!< 起点坐标						*/
	STK_PNT32	pntEnd;							/*!< 终点坐标						*/
	STK_PNT32	pntCenter;						/*!< 中心点							*/
	STK_DIR32	dirX;							/*!< X方向向量						*/
	STK_DIR32	dirY;							/*!< Y方向向量						*/
	STK_DIR32	dirZ;							/*!< Z方向向量						*/
	STK_FLOAT32	fFocalDistance;					/*!< 焦点距离						*/
}StkParabolaRec;

/*
 *	自定义属性
 */
typedef struct {
	StkControl	Control;						/*!< 记录控制							*/
	STK_UINT32	nuNumTitle;						/*!< 自定义属性名称的字符数				*/
	STK_UINT32	nuValueType;					/*!< 自定义属性值类型					*/
												/*!< =1 布尔 =2 整数 =3 实数 =4 字符串	*/
	STK_UINT32	nuNumValue;						/*!< 自定义属性值字符数					*/
	STK_UINT32	nuNumUnits;						/*!< 自定义属性单位的字符数				*/
	STK_WCHAR	atr[1];							/*!< 占位								*/
}StkMetaDataRec;

/*
 *	用户自定义数据
 */
typedef struct {
	StkControl	Control;						/*!< 记录控制							*/
	STK_WCHAR	FileName[STK_MAXNAME];			/*!< 物理文件名							*/
	STK_WCHAR	dataName[STK_MAXNAME];			/*!< 数据名称							*/
	STK_UINT32	dataID;							/*!< 数据ID								*/
	STK_UINT32	nuCharNum;						/*!< 流文件字符数						*/
	STK_CHAR	atr[1];							/*!< 占位								*/
}StkUserDataRec;

typedef struct {
	StkControl	Control;						/*!< 记录控制							*/
	STK_WCHAR	FileName[STK_ULTRANAME];		/*!< 物理文件名							*/
	STK_WCHAR	dataName[STK_ULTRANAME];		/*!< 数据名称							*/
	STK_UINT32	dataID;							/*!< 数据ID								*/
	STK_UINT32	nuCharNum;						/*!< 流文件字符数						*/
	STK_CHAR	atr[1];							/*!< 占位								*/
}StkUserDataRec2;

//----------------------------------------------------------------------

/*
 * Child Node Record
 */
typedef struct {
	STK_UINT32		PlcId;							/*!< Plc ID								*/
	STK_UINT32		NodeId;							/*!< Child NodeID						*/
} StkNodeChild;

/*
 * Node Record
 */
#define  STK_Node_IS_DISPLAY			0x00000001
typedef struct {
	StkControl		Control;						/*!< 记录控制							*/
	STK_UINT32		type;							/*!< Node类型							*/
	STK_WCHAR		name[STK_MAXNAME];				//!< Node名称							*/
	STK_UINT32		IsItem;							/*!< is Display							*/
	STK_UINT32		usetype;						/*!< 使用方式							*/
	STK_UINT32		dmy1;							/*!< 预留1								*/
	STK_UINT32		dmy2;							/*!< 预留2								*/
	STK_UINT32		childnum;						/*!< Child Num							*/
	StkNodeChild	childdat[1];					/*!< Child Data(n)						*/
}StkNodeRec;

typedef struct {
	StkControl		Control;						/*!< 记录控制							*/
	STK_UINT32		type;							/*!< Node类型							*/
	STK_WCHAR		name[STK_ULTRANAME];			//!< Node名称							*/
	STK_UINT32		IsItem;							/*!< is Display							*/
	STK_UINT32		usetype;						/*!< 使用方式							*/
	STK_UINT32		dmy1;							/*!< 预留1								*/
	STK_UINT32		dmy2;							/*!< 预留2								*/
	STK_UINT32		childnum;						/*!< Child Num							*/
	StkNodeChild	childdat[1];					/*!< Child Data(n)						*/
}StkNodeRec2;

typedef struct {
	StkControl		Control;						/*!< 记录控制							*/
	STK_UINT32		type;							/*!< Node类型							*/
	STK_WCHAR		name[STK_MAXPATH];				/*!< Node名称							*/
	STK_UINT32		IsItem;							/*!< is Display							*/
	STK_UINT32		usetype;						/*!< 使用方式							*/
	STK_UINT32		dmy1;							/*!< 预留1								*/
	STK_UINT32		dmy2;							/*!< 预留2								*/
	STK_UINT32		childnum;						/*!< Child Num							*/
	StkNodeChild	childdat[1];					/*!< Child Data(n)						*/
}StkNodeRec3;

typedef struct {
	StkControl		Control;						/*!< 记录控制							*/
	STK_UINT32		type;							/*!< Node类型							*/
	STK_UINT32		IsItem;							/*!< is Display							*/
	STK_UINT32		usetype;						/*!< 使用方式							*/
	STK_UINT32		dmy1;							/*!< 预留1								*/
	STK_UINT32		dmy2;							/*!< 预留2								*/
	STK_UINT32		childnum;						/*!< Child Num							*/
	StkNodeChild	childdat[1];					/*!< Child Data(n)						*/
}StkNodeRec4;

typedef struct {
	StkControl		Control;						/*!< 记录控制							*/
	STK_UINT32		type;							/*!< Node类型							*/
	STK_UINT32		subType;						/*!< Node Sub类型						*/
	STK_UINT32		IsItem;							/*!< is Display							*/
	STK_UINT32		usetype;						/*!< 使用方式							*/
	STK_UINT32		dmy1;							/*!< 预留1								*/
	STK_UINT32		dmy2;							/*!< 预留2								*/
	STK_UINT32		childnum;						/*!< Child Num							*/
	StkNodeChild	childdat[1];					/*!< Child Data(n)						*/
}StkNodeRec5;

/*
 *	要素名称记录
 */
typedef struct {
	StkControl		Control;						/*!< 记录控制							*/
	STK_UINT32		nuNameSize;						/*!< 要素名称数量，包括结束符			*/
	STK_WCHAR		atr[1];							/*!< 要素名称							*/
}StkEntityNameRec;

/*
 *	基本信息集
 */
typedef struct {
	StkControl		Control;						/*!< 记录控制							*/
	STK_INT32		nLayer;							/*!< 图层								*/
}StkInfoRec;

/*
 *	图层信息集
 */
typedef struct {
	StkControl		Control;						/*!< 记录控制								*/
	STK_INT32		nDefaultLayer;					/*!< 默认图层								*/
	STK_INT32		nDefLayerFilter;				/*!< 默认图层过滤器编号						*/
	STK_UINT32		nuLayerNum;						/*!< 图层数量								*/
	STK_UINT32		nuLayerFilterNum;				/*!< 图层过滤器数量（不包括默认图层过滤器）	*/
}StkLayerInfosSetRec;

/*
 *	图层编号、图层名称
 */
#define  STK_LAYER_IS_DISPLAY			0x00000001
typedef struct {
	StkControl		Control;						/*!< 记录控制								*/
	STK_INT32		nLayer;							/*!< 图层编号								*/
	STK_UINT32		IsItem;							/*!< Is display								*/
	STK_UINT32		nuNameSize;						/*!< 图层名称字符数							*/
	STK_WCHAR		atr[1];							/*!< 占位符									*/
}StkLayerRec;

/*
 *	图层过滤器（首先存默认图层过滤器）
 */
typedef struct {
	StkControl		Control;						/*!< 记录控制									*/
	STK_UINT32		nuNameNum;						/*!< 过滤器名称字符数							*/
	STK_UINT32		nuDescriptionNum;				/*!< 过滤器描述字符数量							*/
	//STK_UINT32	nuSelectedStateNum;				/*!< 符合过滤器条件的图层状态:=0未选择，=1选择	*/
	STK_UINT32		nuSelectedLayerNum;				/*!< 符合过滤器条件的图层数量					*/
	char			atr[1];							/*!< 占位符（图层+图层选择状态）				*/
}StkLayerFilterRec;

/*
 *	线缆中的Fiber点
 */
typedef struct {
	StkControl		Control;						/*!< 记录控制									*/
	STK_PNT32		pntStart;						/*!< Fiber 起点									*/
	STK_PNT32		pntEnd;							/*!< Fiber 终点									*/
	STK_PNT32		pntCenter;						/*!< Fiber 中心点								*/
}StkPipeFiberPntRec;

/*
 *	线缆特征
 */
typedef struct {
	StkControl		Control;						/*!< 记录控制									*/
	STK_UINT32		nuPipeType;						/*!< 线缆类型									*/
	STK_UINT32		nuPipeSectionType;				/*!< 线缆的Section类型							*/
	STK_UINT32		nuPipeFiberNum;					/*!< 线缆的Fiber数量							*/
}StkFeatPipeRec;

typedef struct {
	StkControl		Control;						/*!< 记录控制									*/
	STK_UINT32		nuPipeType;						/*!< 线缆类型									*/
	STK_UINT32		nuPipeSectionType;				/*!< 线缆的Section类型							*/
	STK_UINT32		nuPipeFiberNum;					/*!< 线缆的Fiber数量							*/
	STK_FLOAT32		fDiameter;						/*!< 线缆的直径/厚度							*/
	STK_FLOAT32		fTurnRadius;					/*!< 最小折弯半径								*/
	STK_FLOAT32		fHeight;						/*!< 线缆Section的高度							*/
	STK_FLOAT32		fWidth;							/*!< 线缆Section的宽度							*/
}StkFeatPipeRec2;

/*
 *	特征
 */
typedef struct {
	StkControl		Control;						/*!< 记录控制									*/
	STK_UINT32		nuType;							/*!< 特征类型									*/
	STK_UINT32		nuActive;						/*!< 是否活动状态：0 = 未； 1 = 是				*/
	STK_WCHAR		name[STK_MAXNAME];				/*!< 特征名称									*/
}StkFeatRec;

typedef struct {
	StkControl		Control;						/*!< 记录控制									*/
	STK_UINT32		nuType;							/*!< 特征类型									*/
	STK_UINT32		nuActive;						/*!< 是否活动状态：0 = 未； 1 = 是				*/
	STK_UINT32		nuCharSize;						/*!< 特征名称字符数量，包括结束符				*/
	STK_WCHAR		atr[1];							/*!< 特征名称									*/
}StkFeatRec2;

/*
 *	空间点
 */
typedef struct {
	StkControl	Control;						/*!< 记录控制							*/
	STK_ID		idPoint;						/*!< 空间点ID							*/
	STK_UINT32	nuDispStat;						/*!< 空间点的显隐状态					*/
	STK_UINT32	nuType;							/*!< 空间点的类型						*/
	STK_RGBA32	rgbaPoint;						/*!< 空间点的颜色						*/
	STK_PNT32	pntPos;							/*!< 空间点的位置						*/
	STK_UINT32	nuCharNum;						/*!< 空间点名字的字符数量				*/
	STK_WCHAR	atr[1];							/*!< 占位								*/
}StkSpacialPointRec;

/*
 *	用户自定义数据
 */
typedef struct {
	StkControl	Control;						/*!< 记录控制							*/
	STK_WCHAR	name[STK_MAXNAME];				/*!< 流文件名称							*/
	STK_UINT32	nuCharNum;						/*!< 流文件字符数						*/
	STK_CHAR	atr[1];							/*!< 占位								*/
}StkUserDefDataRec;

/*
 *	组件特征
 */
typedef struct {
	StkControl	Control;						/*!< 记录控制									*/
	STK_INT32	nNumber;						/*!< 特征编号									*/
	STK_UINT32	nuType;							/*!< 特征类型									*/
}StkComponentFeatRec;

typedef struct {
	StkControl	Control;						/*!< 记录控制									*/
	STK_WCHAR	wchName[STK_ULTRANAME];			/*!< 特征名称									*/
	STK_INT32	nNumber;						/*!< 特征编号									*/
	STK_UINT32	nuType;							/*!< 特征类型									*/
	STK_UINT32	nuChildFeatNum;					/*!< 子特征数量									*/
}StkComponentFeatRec2;

/*
 *	组、阵列特征
 */
typedef struct {
	StkControl	Control;						/*!< 记录控制									*/
	//STK_UINT32	nuPlcID;						/*!< 当前特征相对上级的配置ID					*/
	STK_UINT32	nuChildInsNum;					/*!< 当前特征下的子实例数量						*/
	STK_UINT32	atr[1];							/*!< 占位符										*/
}StkVirtualFeatRec;

/*
 *	装配特征
 */
typedef struct {
	StkControl	Control;						/*!< 记录控制									*/
	STK_WCHAR	wchPlcPath[STK_MAXPATH];		/*!< 配置路径									*/
	STK_UINT32	nuNodeNum;						/*!< 装配特征关联的Node数量						*/
	STK_UINT32	atr[1];							/*!< 占位符										*/
}StkAssemblyFeatRec;

/*
 *	孔周特征
 */
typedef struct {
	StkControl	Control;						/*!< 记录控制									*/
	STK_DIR32	dirAxis;						/*!< 轴											*/
	STK_PNT32	pntOrigin;						/*!< 原点										*/
}StkFeatAxisRec;

/*
 *	孔螺纹特征
 */
#define STK_HAS_DEPTHTOL				0x00000001
#define STK_HAS_DIAMETERTOL				0x00000010
#define STK_HAS_PITCHTOL				0x00000100
#define STK_IS_RIGHT_THREAD				0x00001000
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuThreadLimitType;			/*!< 底部类型										*/
	STK_UINT32		nuThreadType;				/*!< 螺纹类型										*/
	STK_DOUBLE64	dDepth;						/*!< 螺纹深度										*/
	STK_DOUBLE64	dDiameter;					/*!< 螺纹直径										*/
	STK_DOUBLE64	dPitch;						/*!< 螺距											*/
	STK_UINT32		IsItem;						/*!< 是否有螺纹深度公差/直径公差/螺距公差/右旋螺纹	*/
	STK_DOUBLE64	dDepthTol;					/*!< 螺纹深度公差									*/
	STK_DOUBLE64	dDiameterTol;				/*!< 螺纹直径公差									*/
	STK_DOUBLE64	dPitchTol;					/*!< 螺距公差										*/
}StkFeatInnerThreadRec;

/*
 *	孔特征
 */
#define STK_HAS_BOTTOMANGLETOL			0x00001000
#define STK_HAS_COUNTER_DEPTHTOL		0x00010000
#define STK_HAS_COUNTER_DIAMETERTOL		0x00100000
#define STK_HAS_COUNTER_ANGLETOL		0x01000000
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuAnchorType;				/*!< 孔的固定类型									*/
	STK_UINT32		nuType;						/*!< 孔的类型										*/
	STK_UINT32		nuBottomType;				/*!< 孔的底部类型									*/
	STK_DOUBLE64	dDepth;						/*!< 孔的深度										*/
	STK_DOUBLE64	dDiameter;					/*!< 孔的直径										*/
	STK_DOUBLE64	dBottomAngle;				/*!< 孔的底部角度									*/
	STK_DOUBLE64	dCounterAngle;				/*!< 孔的反向角度									*/
	STK_DOUBLE64	dCounterDepth;				/*!< 孔的反向深度									*/
	STK_DOUBLE64	dCounterDiameter;			/*!< 孔的反向直径									*/
	STK_UINT32		IsItem;						/*!< 是否有孔的深度/直径/底部角度/反向角度/反向深度/反向直径	*/
	STK_DOUBLE64	dDepthTol;					/*!< 孔的深度公差									*/
	STK_DOUBLE64	dDiameterTol;				/*!< 孔的直径公差									*/
	STK_DOUBLE64	dBottomAngleTol;			/*!< 孔的底部角度公差								*/
	STK_DOUBLE64	dCounterAngleTol;			/*!< 孔的反向角度公差								*/
	STK_DOUBLE64	dCounterDepthTol;			/*!< 孔的反向深度公差								*/
	STK_DOUBLE64	dCounterDiameterTol;		/*!< 孔的反向直径公差								*/
}StkFeatHoleRec;

/*
 *	孔的Limit特征
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuType;						/*!< Limit类型										*/
	STK_DOUBLE64	dValue;						/*!< Limit值										*/
}StkFeatLimitRec;

/*
 *	磁力线特征
 */
#define STK_IS_UNEFFECTIVE			0x00000001
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_PNT32		pntStatrt;					/*!< 起点											*/
	STK_PNT32		pntEnd;						/*!< 终点											*/
	STK_UINT32		IsItem;						/*!< is effective									*/
	STK_UINT32		Arrangement;				/*!< 排列方式										*/
}StkFeatMagneticLineRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_PNT32		pntStatrt;					/*!< 起点											*/
	STK_PNT32		pntEnd;						/*!< 终点											*/
	STK_UINT32		IsItem;						/*!< is effective									*/
	STK_UINT32		Arrangement;				/*!< 排列方式										*/
	STK_RGBA32		rgbaMagneticLine;			/*!< 磁力线颜色										*/
}StkFeatMagneticLineRec2;

/*
 *	压缩流记录
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuSegmentNum;				/*!< 被压缩的段数量									*/
	STK_UINT32		nuBufferSize;				/*!< 被压缩的Buff大小								*/
	STK_UINT32		nuStreamSize;				/*!< 压缩后的流大小									*/
	STK_CHAR		atr[1];						/*!< 流数据											*/
}StkCompressRec;

/*
 *	图片(图章)
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuType;						/*!< 图片类型										*/
	STK_PNT32		pntLocation;				/*!< 图片位置										*/
	STK_FLOAT32		fWidth;						/*!< 图片宽度										*/
	STK_FLOAT32		fHeight;					/*!< 图片高度										*/
	STK_UINT32		nuSaveStyle;				/*!< 图片保存方式:内部/外部							*/
	STK_WCHAR		wchPicturePath[STK_MAXPATH];/*!< 图片保存路径									*/
	STK_UINT32		nudataSize;					/*!< 图片数据流大小									*/
	STK_CHAR		atr[1];						/*!< 占位											*/
}StkPictureRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuType;						/*!< 图片类型										*/
	STK_PNT32		pntLocation;				/*!< 图片位置										*/
	STK_FLOAT32		fDispalyWidth;				/*!< 图片在场景图中的显示宽度						*/
	STK_FLOAT32		fDispalyHeight;				/*!< 图片在场景图中的显示高度						*/
	STK_FLOAT32		fActualWidth;				/*!< 图片的实际宽度									*/
	STK_FLOAT32		fActualHeight;				/*!< 图片的实际高度									*/
	STK_INT32		nNC;						/*!< 图片位图										*/
	STK_UINT32		nuSaveStyle;				/*!< 图片保存方式:内部/外部							*/
	STK_WCHAR		wchPicturePath[STK_MAXPATH];/*!< 图片保存路径									*/
	STK_UINT32		nudataSize;					/*!< 图片数据流大小									*/
	STK_CHAR		atr[1];						/*!< 占位											*/
}StkPictureRec2;

typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuType;						/*!< 图片类型										*/
	STK_PNT32		pntLocation;				/*!< 图片位置										*/
	STK_FLOAT32		fDispalyWidth;				/*!< 图片在场景图中的显示宽度						*/
	STK_FLOAT32		fDispalyHeight;				/*!< 图片在场景图中的显示高度						*/
	STK_FLOAT32		fActualWidth;				/*!< 图片的实际宽度									*/
	STK_FLOAT32		fActualHeight;				/*!< 图片的实际高度									*/
	STK_FLOAT32		fScaleX;					/*!< X方向缩放因子									*/
	STK_FLOAT32		fScaleY;					/*!< Y方向缩放因子									*/
	STK_INT32		nNC;						/*!< 图片位图										*/
	STK_UINT32		nuSaveStyle;				/*!< 图片保存方式:内部/外部							*/
	STK_WCHAR		wchPicturePath[STK_MAXPATH];/*!< 图片保存路径									*/
	STK_UINT32		nudataSize;					/*!< 图片数据流大小									*/
	STK_CHAR		atr[1];						/*!< 占位											*/
}StkPictureRec3;

/*
 *	箭头
 */
#define  STK_ARROW_IS_NO_ONTOP				0x00000001
#define  STK_BACKGROUND_IS_GRADUALCHANGED	0x00000002
#define  STK_BORDER_IS_NODISPLAY			0x00000004
#define	 STK_AXIS_IS_DISPLAY				0x00000001
#define  STK_PLANE_IS_RENDER				0x00000002
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< 箭头名称										*/
	STK_UINT32		nuType;						/*!< 箭头类型：直线、圆形、直角						*/
	STK_FLOAT32		fAlpha;						/*!< 不透明性										*/
	STK_UINT32		IsItem;						/*!< 最前端/背景色渐变/显示边界						*/
	STK_UINT32		nuActorPosition;			/*!< 灯头位置										*/
	STK_FLOAT32		fActorForm;					/*!< 灯头形状										*/
	STK_FLOAT32		fActorLength;				/*!< 灯头长度										*/
	STK_FLOAT32		fActorWidth;				/*!< 灯头宽度										*/
	STK_FLOAT32		fArrowWidth;				/*!< 箭头宽度										*/
	STK_FLOAT32		fArrowGradient;				/*!< 箭头倾斜度										*/
	STK_RGBA32		rgbaBackground;				/*!< 箭头背景色										*/
	STK_FLOAT32		fBorderWidth;				/*!< 边界宽度										*/
	STK_RGBA32		rgbaBorder;					/*!< 边界颜色										*/
	STK_UINT32		nuBorderType;				/*!< 边界类型										*/
}StkFeatArrowRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< 箭头名称										*/
	STK_UINT32		nuType;						/*!< 箭头类型：直线、圆形、直角						*/
	STK_FLOAT32		fAlpha;						/*!< 不透明性										*/
	STK_UINT32		IsItem;						/*!< 最前端/背景色渐变/显示边界						*/
	STK_UINT32		nuActorPosition;			/*!< 灯头位置										*/
	STK_FLOAT32		fActorForm;					/*!< 灯头形状										*/
	STK_FLOAT32		fActorLength;				/*!< 灯头长度										*/
	STK_FLOAT32		fActorWidth;				/*!< 灯头宽度										*/
	STK_FLOAT32		fArrowWidth;				/*!< 箭头宽度										*/
	STK_FLOAT32		fArrowGradient;				/*!< 箭头倾斜度										*/
	STK_RGBA32		rgbaBackground;				/*!< 箭头背景色										*/
	STK_FLOAT32		fBorderWidth;				/*!< 边界宽度										*/
	STK_RGBA32		rgbaBorder;					/*!< 边界颜色										*/
	STK_UINT32		nuBorderType;				/*!< 边界类型										*/
	STK_MTX32		DefPlane;					/*!< 定义平面										*/
}StkFeatArrowRec2;

/*
 *	直线箭头
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_PNT32		pntFirstAnchor;				/*!< 锚点1											*/
	STK_PNT32		pntSecondAnchor;			/*!< 锚点2											*/
	STK_DIR32		dirFirstX;					/*!< 锚点1的X方向向量								*/
	STK_DIR32		dirFirstY;					/*!< 锚点1的Y方向向量								*/
	STK_DIR32		dirSecondX;					/*!< 锚点2的X方向向量								*/
	STK_DIR32		dirSecondY;					/*!< 锚点2的Y方向向量								*/
}StkFeatLineArrowRec;

/*
 *	圆形箭头
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		IsItem;						/*!< 是否显示轴/平面渲染							*/
	STK_FLOAT32		fRadius;					/*!< 半径											*/
	STK_PNT32		pntOrigin;					/*!< 圆心											*/
	STK_DIR32		dirX;						/*!< X方向向量										*/
	STK_DIR32		dirY;						/*!< Y方向向量										*/
	STK_UINT32		nuStep;						/*!< 进程											*/
	STK_FLOAT32		fAngle;						/*!< 圆角开口角度									*/
}StkFeatCircularArrowRec;

/*
 *	直角箭头
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_FLOAT32		fFirstLength;				/*!< 直角箭头的长度1								*/
	STK_FLOAT32		fSecondLength;				/*!< 直角箭头的长度2								*/
	STK_PNT32		pntOrigin;					/*!< 直角箭头的原点									*/
	STK_DIR32		dirX;						/*!< 直角箭头的X方向向量							*/
	STK_DIR32		dirY;						/*!< 直角箭头的Y方向向量							*/
}StkFeatRightAngleArrowRec;

/*
 *	纹理
 */
#define  STK_TEXTURE_IS_UFLIPPED	0x00000001
#define  STK_TEXTURE_IS_VFLIPPED	0x00000002
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuMappingType;				/*!< 映射类型										*/
	STK_FLOAT32		fRotateAngle;				/*!< 旋转角度										*/
	STK_FLOAT32		fUOffset;					/*!< U方向偏移距离									*/
	STK_FLOAT32		fVOffset;					/*!< V方向偏移距离									*/
	STK_FLOAT32		fUScale;					/*!< U方向缩放因子									*/
	STK_FLOAT32		fVScale;					/*!< U方向缩放因子									*/
	STK_UINT32		IsItem;						/*!< IsUFlipped/IsVFlipped							*/
}StkTextureRec;

/*
 *	灯光
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_RGBA32		rgbaAmbientColor;			/*!< 环境反射颜色值									*/
	STK_FLOAT32		fAmbientRatio;				/*!< 环境反射比例									*/
	STK_RGBA32		rgbaDiffuseColor;			/*!< 漫反射颜色值									*/
	STK_FLOAT32		fDiffuseRatio;				/*!< 漫反射比例										*/
	STK_RGBA32		rgbaEmissiveColor;			/*!< 发光反射颜色值									*/
	STK_FLOAT32		fEmissiveRatio;				/*!< 发光反射比例									*/
	STK_RGBA32		rgbaReflectiveColor;		/*!< 反射颜色值										*/
	STK_FLOAT32		fReflectiveRatio;			/*!< 反射比例										*/
	STK_RGBA32		rgbaSpecularColor;			/*!< 镜反射颜色值									*/
	STK_FLOAT32		fSpecularRatio;				/*!< 镜反射比例										*/
	STK_RGBA32		rgbaTransparentColor;		/*!< 透明颜色值										*/
	STK_FLOAT32		fTransparentRatio;			/*!< 透明色比例										*/
}StkLightMapRec;

/*
 *	材质
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_ID			idRender;					/*!< Render ID										*/
	STK_FLOAT32		fAmbient;					/*!< 材质环境反射光属性								*/
	STK_FLOAT32		fDiffuse;					/*!< 材质漫反射光属性								*/
	STK_FLOAT32		fSpecular;					/*!< 材质高光属性									*/
	STK_FLOAT32		fReflectivity;				/*!< 材质反射率										*/
	STK_FLOAT32		fRefraction;				/*!< 材质折射率										*/
	STK_FLOAT32		fRoughness;					/*!< 材质高光粗糙程度								*/
	STK_FLOAT32		fTransparency;				/*!< 材质透明度										*/
	STK_FLOAT32		fDensity;					/*!< 材质密度										*/
	STK_UINT32		nuNameSize;					/*!< 材质类型字符数									*/
	STK_WCHAR		atr[1];						/*!< 占位符											*/
}StkMaterialRec;

/*
 *	渲染
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_WCHAR		name[STK_MAXNAME];			/*!< 纹理名称										*/
}StkRenderRec;

typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuNameSize;					/*!< 纹理名称字符数									*/
	STK_WCHAR		atr[1];						/*!< 占位符											*/
}StkRenderRec2;

/*
 *	剖切线
 */
#define  STK_CLIPPINGCURVE_IS_DISPLAY	0x00000001
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		IsItem;						/*!< IsDisplay										*/
	STK_UINT32		nuDrawStyle;				/*!< 绘制显示方式									*/
	STK_UINT32		nuDrawType;					/*!< 绘制显示类型									*/
	STK_FLOAT32		fAngle;						/*!< 剖切线角度										*/
	STK_FLOAT32		fRatio;						/*!< 剖切线显示比例									*/
	STK_RGBA32		rgbaCurve;					/*!< 剖切线颜色										*/
}StkClippingCurveRec;

/*
 *	标记特征
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuType;						/*!< 标记类型										*/
	STK_RGBA32		rgbaMark;					/*!< 标记颜色										*/
	STK_MTX32		mtxDefPlane;				/*!< 定义平面										*/
	STK_FLOAT32		fCurveWidth;				/*!< 线宽											*/
	STK_UINT32		nuCurveType;				/*!< 线型											*/
}StkFeatMarkRec;

/*
 *	矩形标记特征
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuFillStyle;				/*!< 填充方式										*/
	STK_PNT32		pntLeftTop;					/*!< 左上角锚点										*/
	STK_PNT32		pntRightBottom;				/*!< 右下角锚点										*/
}StkMarkRectAngleRec;

/*
 *	椭圆标记特征
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_UINT32		nuFillStyle;				/*!< 填充方式										*/
	STK_PNT32		pntCenter;					/*!< 中心点											*/
	STK_FLOAT32		fMajorRadius;				/*!< 长半径											*/
	STK_FLOAT32		fMinorRadius;				/*!< 短半径											*/
	STK_DIR32		dirNormal;					/*!< 法线方向										*/
	STK_DIR32		dirOrigin;					/*!< 原点方向										*/
}StkMarkEllipseRec;

/*
 *	徒手画标记特征
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_FLOAT32		fUMin;						/*!< 徒手画起始范围									*/
	STK_FLOAT32		fUMax;						/*!< 徒手画终止范围									*/
	STK_UINT32		nuPointNum;					/*!< 徒手画的点列数量								*/
	STK_CHAR		atr[1];						/*!< 占位											*/
}StkMarkFreeHandRec;

/*
 *	工艺BOM表格
 */
typedef struct {
	StkControl		Control;					/*!< 记录控制										*/
	STK_WCHAR		chID[STK_MAXNAME];			/*!< 序号											*/
	STK_WCHAR		chSymbol[STK_MAXNAME];		/*!< 代号											*/
	STK_WCHAR		chName[STK_MAXNAME];		/*!< 名称											*/
	STK_WCHAR		chMaterial[STK_MAXNAME];	/*!< 材料											*/
	STK_UINT32		nuNumber;					/*!< 当前序号对应的实例数量							*/
}StkVirtualBomRec;
	}//svllib
}//hoteamsoft
#endif