//
//  StlWriter.h
//  M3D
//
//  Created by administrator on 15/3/2.
//  Copyright (c) 2015年 hoteam. All rights reserved.
//

#ifndef __M3D__StlWriter__
#define __M3D__StlWriter__
#include "m3d/M3D.h"
#include "Writer.h"
//#include "Stk_DocumentManager.h"

#include "../jni/m3d/base/Vector3.h"
#include "../jni/m3d/base/Matrix3x4.h"
#include "../jni/m3d/M3DStates.h"
enum STLSaveTypeEnum
{
	TYPE_SAVE_STL_BINARY = 0,
	TYPE_SAVE_STL_ASCII
};

namespace SVIEW
{
    class M3D_API StlWriter: public Writer {
    public:
        StlWriter(void);
        virtual ~StlWriter();
//        virtual int SaveTo(Model* model,const string &targetPath);
        virtual int SaveTo(View* view,const string &targetPath);

	public:
		/****************************************************************************
		*	@brief		保存STL文件
		*	@param[in]	i_stkDocumentP			SVL文件指针
		*	@param[in]	i_wstrSavePath			保存路径
		*	@param[in]	i_eSaveSTLType			保存类型
		*	@param[out]	无
		*	@retval		STK_SUCCESS				保存成功
		*	@retval		其它					保存失败
		*	@note		无
		*	@attention	无
		****************************************************************************/
		M3D_STATUS SaveSVLAsSTL(View* view, const string& srcPath, int type);
    
		/*****************************************************************
		函数名	：ConvertToSTLMeshFromTopProto
		功能	：从TopProto转换SVL文件数据到STL Mesh
		参数	：Stk_ProtoType* i_stkTopProtoTypeP, 零件对象
		vector<STK_PNT32>& o_vcVtxs, 点列
		vector<STK_UINT32>& o_vcIndexs, 索引
		返回值	：STK_SUCCESS, 转换成功  其它,转换失败
		更新历史：
		*****************************************************************/
		M3D_STATUS	ConvertToSTLMeshFromTopProto(Model* viewModel, vector<M3D::Vector3>& o_vcVtxs, vector<int>& o_vcIndexs);
	
		/************************************************************************//**
		*	@brief		计算STL点列索引
		*	@param[in]	i_stkProtoTypeP			零件对象
		*	@param[in]	i_mtxPlacement			实例配置矩阵
		*	@param[out]	o_vcVtxs				点列
		*	@param[out]	o_vcIndexs				索引
		*	@retval		STK_SUCCESS				计算成功
		*	@retval		其它					计算失败
		*	@note		无
		*	@attention	内部接口
		****************************************************************************/
		M3D_STATUS	ComputeSTLVertex(Model* viewModel, M3D::Matrix3x4 i_mtxPlacement, vector<M3D::Vector3>& o_vcVtxs, vector<int>& o_vcIndexs);

	};
}

#endif /* defined(__M3D__StlWriter__) */
