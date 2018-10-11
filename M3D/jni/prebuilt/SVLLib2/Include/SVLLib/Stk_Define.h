// All Rights Reserved. Copyright(C) 2012 HOTEAMSOFT
/************************************************************************************
 *	@file	Stk_Define.h
 *	@brief	系统基础类型定义的文件
 ************************************************************************************/

#ifndef _SVLLIB_DEFINE_H_
#define _SVLLIB_DEFINE_H_
#include <stdio.h>
#include <stddef.h>
#include <string>
#include <map>
#include <vector>
#include "Stk_Enum.h"
using namespace std;

namespace HoteamSoft {
	namespace SVLLib {

#define STK_NEW_ID	0
#define STK_DMY_ID	0xFFFFFFFF

#ifdef __MOBILE__
typedef unsigned int		UINT;
typedef int					INT;
typedef float				FLOAT;
typedef unsigned char		UCHAR;
typedef bool				BOOL;
typedef unsigned long long	UINT64;
typedef int					__int32 ;
typedef long long			__int64 ;
typedef short				__int16 ;
typedef char				CHAR;
typedef wchar_t				WCHAR;	//Andriod下4字节

typedef int					STK_INT32;
typedef long long			STK_INT64;
typedef unsigned int		STK_UINT32;
typedef unsigned long long	STK_UINT64;
typedef size_t				STK_SIZE_T;
typedef char				STK_CHAR;
typedef unsigned short		STK_WCHAR;
typedef double				STK_DOUBLE64;
typedef float				STK_FLOAT32;
typedef bool				STK_BOOL;
#define TRUE				true
#define FALSE				false
typedef std::string			Stk_string;
typedef std::wstring		Stk_wstring;
typedef void*				HMODULE;
typedef void*				HANDLE;

const class nullptr_t_t
{
public:
    template<class T>           operator T*() const {return 0;}
    template<class C, class T>  operator T C::*() const { return 0; }
private:
    void operator& () const;
} nullptr_t = {};

#define nullptr_t nullptr

#define STK_TOOLKIT_EXPORT
#define MAX_PATH 256


#define LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))

#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((unsigned long)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

#else // __MOBILE__

#if 0
//基础类型
typedef char				CHAR;
typedef wchar_t				WCHAR;	//Windows下2字节
#endif

typedef __int32				STK_INT32;
typedef __int64				STK_INT64;
typedef unsigned __int32	STK_UINT32;
typedef unsigned __int64	STK_UINT64;
typedef size_t				STK_SIZE_T;
typedef char				STK_CHAR;
typedef wchar_t				STK_WCHAR;
typedef double				STK_DOUBLE64;
typedef float				STK_FLOAT32;
typedef bool				STK_BOOL;
typedef std::string			Stk_string;
typedef std::wstring		Stk_wstring;

// 为VS2010定义
#if 0
typedef unsigned int		UINT;
typedef unsigned long long	UINT64;
#define VOID void
#define TRUE true
#define FALSE false
#endif

#endif


#define MEMBLOCK		40960
#define MIN_PASSWORD	6
#define MAX_PASSWORD	16
//
//#define PUBLIC(obj)			((obj)->m_stkPublicP)
//#define PRIVATE(obj)		((obj)->m_stkPrivateP)
//#define PAR_PRIVATE(obj)	((obj)->m_stkParPrivateP)
//#define REVOL_PRIVATE(obj)	((obj)->m_stkRevolPrivateP)

//SVL类型
typedef STK_UINT32 STK_ID;

#define MTX_LINE_COUNT	4
#define MTX_COL_COUNT	4
#define BUFSIZE 4096 
typedef struct _stk_mtx32
{ 
	//_stk_mtx32(void){memset(PlcMatrix, 0, sizeof(PlcMatrix)/sizeof(STK_FLOAT32));}
	STK_FLOAT32 PlcMatrix[MTX_LINE_COUNT][MTX_COL_COUNT];
} STK_MTX32;

typedef struct _stk_box32
{ 
	//_stk_box32(void){memset(BoundBox, 0, sizeof(BoundBox)/sizeof(STK_FLOAT32));}
	STK_FLOAT32 BoundBox[2][3];
} STK_BOX32;

typedef struct _stk_rgba32
{ 
	_stk_rgba32(void):Red(0.0),Green(0.0),Blue(0.0),Alpha(0.0){}
	STK_FLOAT32 Red;
	STK_FLOAT32 Green; 
	STK_FLOAT32 Blue; 
	STK_FLOAT32 Alpha;
} STK_RGBA32;

typedef struct _stk_mat32
{ 
	_stk_mat32(void):Ambient(0.0),Diffuse(0.0),Specular(0.0){}
	STK_FLOAT32 Ambient;
	STK_FLOAT32 Diffuse; 
	STK_FLOAT32 Specular;
} STK_MAT32;

typedef struct _stk_pnt32
{ 
	_stk_pnt32(void):X(0.0),Y(0.0),Z(0.0){}
	STK_FLOAT32 X;
	STK_FLOAT32 Y; 
	STK_FLOAT32 Z;
} STK_PNT32;

typedef	struct _stk_dir32
{ 
	_stk_dir32(void):X(0.0),Y(0.0),Z(0.0){}
	STK_FLOAT32 X;
	STK_FLOAT32 Y;
	STK_FLOAT32 Z;
} STK_DIR32;

//Mesh块
typedef struct {
	STK_UINT32		meshID;						/*!MeshID*/
	STK_UINT32		numVertex;				   /*!顶点个数*/
	STK_UINT32		numNormal;				   /*!法线个数*/
	STK_UINT32		numUV;				       /*!UV点个数*/
	STK_UINT32		numEdge;					   /*!Edge个数*/
	STK_UINT32		numFace;					   /*!Face个数*/
	STK_UINT32		numPadding;					   /*!预留字节个数*/
} StkMeshBlock;
//MeshFace块
typedef struct {
	STK_UINT32		meshFaceID;					/*!MeshFaceID*/
	STK_UINT32		materialID;					/*!材质ID*/
	STK_UINT32		numIndex;					/*!MeshFace索引个数*/
	STK_UINT32		numEdge;						/*!MeshFace中边的个数*/
} StkMeshFaceBlock;
//MeshEdge块
typedef struct {
	STK_UINT32		meshEdgeID;					/*!MeshEdgeID*/
	STK_UINT32		numIndex;					/*!MeshEdge索引个数*/
} StkMeshEdgeBlock;
//Bom块
typedef struct {
	STK_INT32		instanceID;					/*!实例ID*/
	STK_INT32 		parentID;						/*!实例父ID*/
	STK_INT32 		modelID;						/*!ModelID*/
	STK_INT32 		plcId;							/*!< 配置ID*/
	STK_INT32		visible;							/*!< 显隐状态*/
	STK_INT32		materialID;					/*!< 材质ID*/ 
	STK_FLOAT32	Matrix[16];					/*!< 配置矩阵*/
} StkBomBlock;
//Model块
typedef struct{
	STK_INT32			modelID;					/*!ModelID*/
	STK_FLOAT32		BBox[6];					/*!包围盒*/
	STK_UINT32			numLod;					/*!Lod个数*/
}StkModelBlock;
//访问器块
typedef struct{
	STK_INT32			fileIndex;					/*!文件索引*/
	STK_UINT32			blockOffset;				/*数据块位移*/
	STK_UINT32			blockLength;				/*数据块长度*/
}StkAccessorBlock;
//回调函数
#ifdef _WIN32
typedef int(__stdcall *RequestFilePathCallback)(const char* pDomainName, const char* pProjectName, const char* pFileName, char*  strFilePath, int bufSize);
#else
typedef int(*RequestFilePathCallback)(const char* pDomainName, const char* pProjectName, const char* pFileName, char*  strFilePath, int bufSize);
#endif
	}//svllib
}//hoteamsoft
#endif
