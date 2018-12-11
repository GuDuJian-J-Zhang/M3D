//
//  StlWriter.cpp
//  M3D
//
//  Created by administrator on 15/3/2.
//  Copyright (c) 2015年 hoteam. All rights reserved.
//

#include "StlWriter.h"
#include "Writer.h"
#include "Utility.h"

#include "sview/views/View.h"
#include "m3d/SceneManager.h"
#include "../jni/m3d/utils/MeshTools.h"
#include "../jni/m3d/model/Body.h"
#include "../jni/m3d/base/Vector3.h"
#include "../jni/m3d/model/MeshData.h"
#include "../jni/m3d/utils/MeshTools.h"
namespace SVIEW
{
    StlWriter::StlWriter(void)
    {
        
    }
    StlWriter::~StlWriter()
    {
        
    }

    int StlWriter::SaveTo(View* view,const string &targetPath)
    {
    	 return 0;
    }

	M3D_STATUS StlWriter::SaveSVLAsSTL(View* view, const string& srcPath, int type)
	{
		M3D_STATUS			eState = M_SUCCESS;
		vector<Vector3>		o_vcVtxs;
		vector<int>			o_vcIndexs;
		string				strSavePath = "";
		
		eState = ConvertToSTLMeshFromTopProto(view->GetSceneManager()->GetModel(), o_vcVtxs, o_vcIndexs);
		if (eState != M_SUCCESS)
		{
			return M_ERROR;
		}
		strSavePath = srcPath;
		if (type == TYPE_SAVE_STL_BINARY)
		{
			MTools::MeshTools::WriteBinarySTL(o_vcVtxs, o_vcIndexs, strSavePath.c_str());
		}
		else if (type == TYPE_SAVE_STL_ASCII)
		{
			MTools::MeshTools::WriteAsciiSTL(o_vcVtxs, o_vcIndexs, strSavePath.c_str());
		}

		return eState;
	}


	M3D_STATUS StlWriter::ConvertToSTLMeshFromTopProto(Model* viewModel, vector<Vector3>& o_vcVtxs, vector<int>& o_vcIndexs)
	{
		M3D_STATUS			eState = M_SUCCESS;
		Matrix3x4		mtxPlacement;

		ComputeSTLVertex(viewModel, mtxPlacement, o_vcVtxs, o_vcIndexs);
		//eState = ConvertChildProtoToSTLMesh(viewModel, mtxPlacement, o_vcVtxs, o_vcIndexs);
		if (eState != M_SUCCESS)
		{
			return M_ERROR;
		}
		return eState;
	}

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
	M3D_STATUS StlWriter::ComputeSTLVertex(Model* viewModel, Matrix3x4 i_mtxPlacement, vector<Vector3>& o_vcVtxs, vector<int>& o_vcIndexs)
	{
		unsigned int	nuBodyNum = 0;
		unsigned int	nuMeshNum = 0;
		unsigned int	nuMeshFaceNum = 0;
				 int	nuMaxFlag = 0;

		std::vector<M3D::Body*>* vec_m3dBodyP = NULL;
		vector<Model*> vec_m3dSubBodyP;
		M3D::VertexSet*	m3dMeshComP=NULL ;
		M3D::Face*	    m3dMeshFaceP = NULL;

		vec_m3dBodyP = viewModel->GetBodys();
		vec_m3dSubBodyP =viewModel->GetChildren();

		for (vector<M3D::Body*>::iterator iter= vec_m3dBodyP->begin();iter!= vec_m3dBodyP->end();iter++)
		{
			m3dMeshComP =(*iter)->GetData();
			if (m3dMeshComP == NULL)
				continue;
			vector<Vector3>* bodyMeshDataNormalArray =m3dMeshComP->GetNormalArray();//计算法向
			vector<Vector3>* bodyMeshDataPosArray =m3dMeshComP->GetPositionArray();//顶定点数组
			vector<Vector3>* bodyMeshDataUVArray = m3dMeshComP->GetTextureCoordArray();//纹理坐标
			vector<M3D_INDEX_TYPE>*bodymeshDataIndexArry = m3dMeshComP->GetIndexArray();//索引


			//获取顶点数据
			nuMaxFlag = o_vcVtxs.size();

			int iNumPosObject = 0;
			iNumPosObject = bodyMeshDataPosArray->size();
			int iPos = 0;
			for (; iPos < iNumPosObject; iPos++)
			{
				Vector3	pntPosSrc;
				Vector3	pntPosDst;

				Vector3& posPnt = (bodyMeshDataPosArray->at(iPos));

				pntPosSrc.m_x = posPnt.m_x;
				pntPosSrc.m_y = posPnt.m_y;
				pntPosSrc.m_z = posPnt.m_z;
				MTools::MeshTools::MultMatrixVec(i_mtxPlacement, pntPosSrc, pntPosDst);
				o_vcVtxs.push_back(pntPosDst);
			}

			//计算索引
			int iNumIndexObject = 0;
			iNumIndexObject = bodymeshDataIndexArry->size();
			int iIndex = 0;

			for (; iIndex < iNumIndexObject; iIndex++)
			{
				unsigned int	pntUVSrc;
				unsigned int	pntUVDst;

				M3D_INDEX_TYPE indexPnt = bodymeshDataIndexArry->at(iIndex);

				o_vcIndexs.push_back(indexPnt+ nuMaxFlag);
			}

		}

		for (vector<Model*>::iterator iter = vec_m3dSubBodyP.begin(); iter != vec_m3dSubBodyP.end(); iter++)
		{
			Model* m_ViewModel = *iter;
			Matrix3x4 submtxPlacement = m_ViewModel->GetWorldTransform();
			ComputeSTLVertex(m_ViewModel, submtxPlacement, o_vcVtxs, o_vcIndexs);
		}

		return M_SUCCESS;
	}
}
