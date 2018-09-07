// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Camera.cpp
 *
 *	@brief	镜头类
 *
 *	@par	历史:
 *		2013/08/14	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#include "Stk_Camera.h"
#include "Stk_Utility.h"
#include "Stk_ObjectImpl.h"
#include "Stk_BaseDocumentImpl.h"
#include "Stk_Declare_Ptr.h"
#include "ChineseCode.h"
#include "StringTools.h"

#define CAMERA_NODE_NAME_PROJECTTYPE				"projectType"
#define CAMERA_NODE_NAME_FOV						"angle"
#define CAMERA_NODE_NAME_ORIGIN						"origin"
#define CAMERA_NODE_NAME_TARGETVEC					"targetVector"
#define CAMERA_NODE_NAME_UPVEC						"upVector"
#define CAMERA_NODE_NAME_MATIX						"matix"
#define CAMERA_NODE_NAME_ASPECTRATIO				"aspectRatio"
#define CAMERA_NODE_NAME_NEARDISTANCE				"nearDistance"
#define CAMERA_NODE_NAME_FARDISTANCE				"farDistance"
#define CAMERA_NODE_NAME_FOCALDISTANCE				"focalDistance"
#define CAMERA_NODE_NAME_HEIGHT						"height"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_CameraImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Camera)
		public:
			Stk_CameraImpl();
			virtual ~Stk_CameraImpl(void);

			/****************************************************************************
			*	@brief		读取文件中的Body数据
			*	@param[in]	i_pJsonNode			View 父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		STK_ERROR			读取失败
			*	@retval		STK_ERROR_MEMORY	内存申请失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		写View数据
			*	@param[in]	Value* i_pJsonParentNode			View 父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其它				写入失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);

		public:
			/****************************************************************************
			*	@brief		设置投影类型
			*	@param[in]	i_nuProjectType	投影类型
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetProjectType(const STK_UINT32 &i_nuProjectType);

			/****************************************************************************
			*	@brief		获取投影类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_nuProjectType	投影类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_UINT32					GetProjectType();

			/****************************************************************************
			*	@brief		设置镜头角度
			*	@param[in]	i_fAngle	角度
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetAngle(const STK_FLOAT32 &i_fAngle);

			/****************************************************************************
			*	@brief		获取镜头角度
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fAngle	镜头角度
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32					GetAngle();

			/****************************************************************************
			*	@brief		设置镜头原始位置
			*	@param[in]	i_pntOrigin	镜头位置
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetOriginPoint(const STK_PNT32 &i_pntOrigin);

			/****************************************************************************
			*	@brief		获取镜头原始位置
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntOrigin		镜头原始位置
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32					GetOriginPoint();

			/****************************************************************************
			*	@brief		设置Target方向向量
			*	@param[in]	i_dirTargetVector	Target方向向量
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetTargetVector(const STK_DIR32 &i_dirTargetVector);

			/****************************************************************************
			*	@brief		获取Target方向向量
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dirTargetVector	Target方向向量
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32					GetTargetVector();


			/****************************************************************************
			*	@brief		设置Up方向向量
			*	@param[in]	i_dirUpVector	Up方向向量
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetUpVector(const STK_DIR32 &i_dirUpVector);

			/****************************************************************************
			*	@brief		获取Up方向向量
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dirUpVector	Up方向向量
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32					GetUpVector();

			/****************************************************************************
			*	@brief		获取是否有旋转矩阵标志
			*	@param[in]	i_bHasMatrix	是否有旋转矩阵标识
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void SetMatrixFlag(STK_BOOL i_bHasMatrix);

			/****************************************************************************
			*	@brief		获取是否有旋转矩阵标志
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_bHasMatrix	是否有旋转矩阵标识
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					GetMatrixFlag();

			/****************************************************************************
			*	@brief		设置旋转矩阵
			*	@param[in]	i_mtxTransform	4*4旋转矩阵
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetMatrix(STK_MTX32 i_mtxTransform);

			/****************************************************************************
			*	@brief		获取旋转矩阵
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_mtxTransform	旋转矩阵
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_MTX32					GetMatrix();

			/****************************************************************************
			*	@brief		设置视景体的宽高比
			*	@param[in]	i_fAspectRatio	宽高比
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetAspectRatio(const STK_FLOAT32 &i_fAspectRatio);

			/****************************************************************************
			*	@brief		获取视景体的宽高比
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fAspectRatio	宽高比
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32					GetAspectRatio();

			/****************************************************************************
			*	@brief		设置焦距
			*	@param[in]	i_fFocus	焦距
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetFocalDistance(const STK_FLOAT32 &i_fFocalDistance);

			/****************************************************************************
			*	@brief		获取焦距
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fFocalDistance	焦距
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32					GetFocalDistance();

			/****************************************************************************
			*	@brief		设置摄像机与近平面距离
			*	@param[in]	i_fNearlDistance	摄像机与近平面距离
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetNearDistance(const STK_FLOAT32 &i_fNearlDistance);

			/****************************************************************************
			*	@brief		获取摄像机与近平面距离
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fNearlDistance	摄像机与近平面距离
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32					GetNearDistance();

			/****************************************************************************
			*	@brief		设置摄像机与远平面距离
			*	@param[in]	i_fFarDistance	摄像机与远平面距离
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetFarDistance(const STK_FLOAT32 &i_fFarDistance);

			/****************************************************************************
			*	@brief		获取摄像机与远平面距离
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fFarDistance	摄像机与远平面距离
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32					GetFarDistance();

			

		public:
			STK_UINT32			m_nuProjectType;					//!< 投影方式
			STK_FLOAT32		m_fAngle;							//!< 摄像机的角度
			STK_PNT32		m_pntOrigin;						//!< 位置
			STK_DIR32		m_dirTargetVector;					//!< 目标向量
			STK_DIR32		m_dirUpVector;						//!< Up向量
			STK_BOOL		m_bHasMatrix;						//!< 是否有矩阵
			STK_MTX32		m_mtxTransform;						//!< 旋转平移矩阵
			STK_FLOAT32		m_fAspectRatio;						//!< 宽高比
			STK_FLOAT32		m_fNearDistance;					//!< 摄像机近平面距离
			STK_FLOAT32		m_fFarDistance;						//!< 摄像机远平面距离
			STK_FLOAT32		m_fFocalDistance;					//!< 摄像机焦距
			STK_FLOAT32		m_fHeight;							//!< 视景体高度
		};

		STK_CLASS_IMPL_IMPLEMENT(Stk_Camera)
			Stk_CameraImpl::Stk_CameraImpl()
		{
			m_nuProjectType = 0;
			m_fAngle = 1.0f;
			m_pntOrigin.X = 0.0f;
			m_pntOrigin.Y = 0.0f;
			m_pntOrigin.Z = 0.0f;
			m_dirTargetVector.X = -1.0f;
			m_dirTargetVector.Y = -1.0f;
			m_dirTargetVector.Z = -1.0f;
			m_dirUpVector.X = -1.0f;
			m_dirUpVector.Y = -1.0f;
			m_dirUpVector.Z = -1.0f;

			m_bHasMatrix = false;
			for (int ii = 0; ii < 4; ii++)
			{
				for (int jj = 0; jj < 4; jj++)
				{
					m_mtxTransform.PlcMatrix[ii][jj] = -1.0f;
				}
			}

			m_fAspectRatio = 1.0f;
			m_fNearDistance = -1.0f;
			m_fFarDistance = -1.0f;
			m_fFocalDistance = 1.0f;
			m_fHeight = 100.0f;
		}

		Stk_CameraImpl::~Stk_CameraImpl(void)
		{
			
		}

		STK_STATUS Stk_CameraImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			m_nuProjectType = (*i_pJsonNode)[CAMERA_NODE_NAME_PROJECTTYPE].GetInt();
			m_fAngle = (*i_pJsonNode)[CAMERA_NODE_NAME_FOV].GetFloat();

			Value::MemberIterator originItor = i_pJsonNode->FindMember(CAMERA_NODE_NAME_ORIGIN);
			if (originItor != i_pJsonNode->MemberEnd())
			{
				string strOrigin = (*i_pJsonNode)[CAMERA_NODE_NAME_ORIGIN].GetString();
				std::vector<std::string> vecOriginValue;
				CStringTools::StringSplit(strOrigin, " ", vecOriginValue);
				if (vecOriginValue.size() == 3)
				{
					m_pntOrigin.X = atof(vecOriginValue[0].c_str());
					m_pntOrigin.Y = atof(vecOriginValue[1].c_str());
					m_pntOrigin.Z = atof(vecOriginValue[2].c_str());
				}
			}

			Value::MemberIterator targetVectorIter = i_pJsonNode->FindMember(CAMERA_NODE_NAME_TARGETVEC);
			if (targetVectorIter != i_pJsonNode->MemberEnd())
			{
				string strTargetVector = targetVectorIter->value.GetString();
				std::vector<std::string> vecTargetVectorValue;
				CStringTools::StringSplit(strTargetVector, " ", vecTargetVectorValue);
				if (vecTargetVectorValue.size() == 3)
				{
					m_dirTargetVector.X = atof(vecTargetVectorValue[0].c_str());
					m_dirTargetVector.Y = atof(vecTargetVectorValue[1].c_str());
					m_dirTargetVector.Z = atof(vecTargetVectorValue[2].c_str());
				}
			}

			Value::MemberIterator upVectorIter = i_pJsonNode->FindMember(CAMERA_NODE_NAME_UPVEC);
			if (upVectorIter != i_pJsonNode->MemberEnd())
			{
				string strUpVector = upVectorIter->value.GetString();
				std::vector<std::string> vecUpVectorValue;
				CStringTools::StringSplit(strUpVector, " ", vecUpVectorValue);
				if (vecUpVectorValue.size() == 3)
				{
					m_dirUpVector.X = atof(vecUpVectorValue[0].c_str());
					m_dirUpVector.Y = atof(vecUpVectorValue[1].c_str());
					m_dirUpVector.Z = atof(vecUpVectorValue[2].c_str());
				}
			}

			m_bHasMatrix = false;
			
			Value::MemberIterator matrixIter = i_pJsonNode->FindMember(CAMERA_NODE_NAME_MATIX);
			if (matrixIter != i_pJsonNode->MemberEnd() && matrixIter->value.IsString())
			{
				string strMatrix = matrixIter->value.GetString();
				std::vector<std::string> vecMatrixValue;
				CStringTools::StringSplit(strMatrix, " ", vecMatrixValue);
				if (vecMatrixValue.size() == 16)
				{
					for (int ii = 0; ii < 4; ii++)
					{
						for (int jj = 0; jj < 4; jj++)
						{
							m_mtxTransform.PlcMatrix[ii][jj] = atof(vecMatrixValue[ii * 4 + jj].c_str());
						}
					}
				}
				vecMatrixValue.clear();
				m_bHasMatrix = true;
			}

			m_fAspectRatio = (*i_pJsonNode)[CAMERA_NODE_NAME_ASPECTRATIO].GetFloat();

			Value::MemberIterator nearDistanceIter = i_pJsonNode->FindMember(CAMERA_NODE_NAME_NEARDISTANCE);
			if (nearDistanceIter != i_pJsonNode->MemberEnd())
			{
				m_fNearDistance = nearDistanceIter->value.GetFloat();
			}

			Value::MemberIterator farDistanceIter = i_pJsonNode->FindMember(CAMERA_NODE_NAME_FARDISTANCE);
			if (farDistanceIter != i_pJsonNode->MemberEnd())
			{
				m_fFarDistance = farDistanceIter->value.GetFloat();
			}
			
			m_fFocalDistance = (*i_pJsonNode)[CAMERA_NODE_NAME_FOCALDISTANCE].GetFloat();
			m_fHeight = (*i_pJsonNode)[CAMERA_NODE_NAME_HEIGHT].GetFloat();
			
			return ret;
		}

		STK_STATUS Stk_CameraImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !i_JsonDocP)
				return STK_ERROR;
			char cTemp[256];

			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(CAMERA_NODE_NAME_PROJECTTYPE, m_nuProjectType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CAMERA_NODE_NAME_FOV, m_fAngle, i_JsonDocP->GetAllocator());

			if(m_bHasMatrix)
			{
				string strMatirx = "";
				sprintf(cTemp, "%.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f",
					m_mtxTransform.PlcMatrix[0][0], m_mtxTransform.PlcMatrix[0][1], m_mtxTransform.PlcMatrix[0][2], m_mtxTransform.PlcMatrix[0][3],
					m_mtxTransform.PlcMatrix[1][0], m_mtxTransform.PlcMatrix[1][1], m_mtxTransform.PlcMatrix[1][2], m_mtxTransform.PlcMatrix[1][3],
					m_mtxTransform.PlcMatrix[2][0], m_mtxTransform.PlcMatrix[2][1], m_mtxTransform.PlcMatrix[2][2], m_mtxTransform.PlcMatrix[2][3],
					m_mtxTransform.PlcMatrix[3][0], m_mtxTransform.PlcMatrix[3][1], m_mtxTransform.PlcMatrix[3][2], m_mtxTransform.PlcMatrix[3][3]);
				strMatirx = cTemp;
				i_pJsonNode->AddMember(CAMERA_NODE_NAME_MATIX, strMatirx, i_JsonDocP->GetAllocator());
			}
			else
			{
				sprintf(cTemp, "%.8f %.8f %.8f", m_pntOrigin.X, m_pntOrigin.Y, m_pntOrigin.Z);
				string strOrigin = cTemp;
				i_pJsonNode->AddMember(CAMERA_NODE_NAME_ORIGIN, strOrigin, i_JsonDocP->GetAllocator());

				sprintf(cTemp, "%.8f %.8f %.8f", m_dirTargetVector.X, m_dirTargetVector.Y, m_dirTargetVector.Z);
				string strTargetVector = cTemp;
				i_pJsonNode->AddMember(CAMERA_NODE_NAME_TARGETVEC, strTargetVector, i_JsonDocP->GetAllocator());

				sprintf(cTemp, "%.8f %.8f %.8f", m_dirUpVector.X, m_dirUpVector.Y, m_dirUpVector.Z);
				string strUpVector = cTemp;
				i_pJsonNode->AddMember(CAMERA_NODE_NAME_UPVEC, strUpVector, i_JsonDocP->GetAllocator());
			}
			i_pJsonNode->AddMember(CAMERA_NODE_NAME_ASPECTRATIO, m_fAspectRatio, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CAMERA_NODE_NAME_NEARDISTANCE, m_fNearDistance, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CAMERA_NODE_NAME_FARDISTANCE, m_fFarDistance, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CAMERA_NODE_NAME_FOCALDISTANCE, m_fFocalDistance, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CAMERA_NODE_NAME_HEIGHT, m_fHeight, i_JsonDocP->GetAllocator());
			return ret;
		}

		void Stk_CameraImpl::SetProjectType(const STK_UINT32 &i_nuProjectType)
		{
			m_nuProjectType = i_nuProjectType;
		}


		STK_UINT32 Stk_CameraImpl::GetProjectType()
		{
			return m_nuProjectType;
		}


		void Stk_CameraImpl::SetAngle(const STK_FLOAT32 &i_fAngle)
		{
			m_fAngle = i_fAngle;
		}

		STK_FLOAT32 Stk_CameraImpl::GetAngle()
		{
			return m_fAngle;
		}

		void Stk_CameraImpl::SetOriginPoint(const STK_PNT32 &i_pntOrigin)
		{
			m_pntOrigin = i_pntOrigin;
		}

		STK_PNT32 Stk_CameraImpl::GetOriginPoint()
		{
			return m_pntOrigin;
		}

		void Stk_CameraImpl::SetTargetVector(const STK_DIR32 &i_dirTargetVector)
		{
			m_dirTargetVector = i_dirTargetVector;
		}

		STK_DIR32 Stk_CameraImpl::GetTargetVector()
		{
			return m_dirTargetVector;
		}

		void Stk_CameraImpl::SetUpVector(const STK_DIR32 &i_dirUpVector)
		{
			m_dirUpVector = i_dirUpVector;
		}

		STK_DIR32 Stk_CameraImpl::GetUpVector()
		{
			return m_dirUpVector;
		}

		void Stk_CameraImpl::SetMatrixFlag(STK_BOOL i_bHasMatrix)
		{
			m_bHasMatrix = i_bHasMatrix;
		}

		STK_BOOL Stk_CameraImpl::GetMatrixFlag()
		{
			return m_bHasMatrix;
		}

		void Stk_CameraImpl::SetMatrix(STK_MTX32 i_mtxTransform)
		{
			for (int ii = 0; ii < 4; ii++)
			{
				for (int jj = 0; jj < 4; jj++)
				{
					m_mtxTransform.PlcMatrix[ii][jj] = i_mtxTransform.PlcMatrix[ii][jj];
				}
			}
			m_bHasMatrix = true;

			Stk_Utility::TransMatrixToUV(i_mtxTransform, m_dirUpVector, m_dirTargetVector);
		}

		STK_MTX32 Stk_CameraImpl::GetMatrix()
		{
			if (m_bHasMatrix)
			{
				return m_mtxTransform;
			}
			else
			{
				STK_DIR32 VVec;
				VVec.X = m_dirTargetVector.Y * m_dirUpVector.Z - m_dirTargetVector.Z * m_dirUpVector.Y;
				VVec.Y = m_dirTargetVector.Z * m_dirUpVector.X - m_dirTargetVector.X * m_dirUpVector.Z;
				VVec.Z = m_dirTargetVector.X * m_dirUpVector.Y - m_dirTargetVector.Y * m_dirUpVector.X;

				STK_MTX32 mtxTmp = {
					VVec.X,VVec.Y,VVec.Z,0.0f,
					m_dirUpVector.X,m_dirUpVector.Y,m_dirUpVector.Z,0.0f,
					-m_dirTargetVector.X,-m_dirTargetVector.Y,-m_dirTargetVector.Z,0.0f/*,
					0.0f,0.0f,0.0f,1.0f*/
				};

				memcpy(&m_mtxTransform, &mtxTmp, sizeof(STK_MTX32));
				return m_mtxTransform;
			}
		}

		void Stk_CameraImpl::SetAspectRatio(const STK_FLOAT32 &i_fAspectRatio)
		{
			m_fAspectRatio = i_fAspectRatio;
		}

		STK_FLOAT32 Stk_CameraImpl::GetAspectRatio()
		{
			return m_fAspectRatio;
		}

		void Stk_CameraImpl::SetFocalDistance(const STK_FLOAT32 &i_fFocalDistance)
		{
			m_fFocalDistance = i_fFocalDistance;
		}

		STK_FLOAT32 Stk_CameraImpl::GetFocalDistance()
		{
			return m_fFocalDistance;
		}

		void Stk_CameraImpl::SetNearDistance(const STK_FLOAT32 &i_fNearlDistance)
		{
			m_fNearDistance = i_fNearlDistance;
		}

		STK_FLOAT32 Stk_CameraImpl::GetNearDistance()
		{
			return m_fNearDistance;
		}

		void Stk_CameraImpl::SetFarDistance(const STK_FLOAT32 &i_fFarDistance)
		{
			m_fFarDistance = i_fFarDistance;
		}

		STK_FLOAT32 Stk_CameraImpl::GetFarDistance()
		{
			return m_fFarDistance;
		}


		STK_IMPLEMENT_CLASS(Stk_Camera, Stk_Object)
		/************************************************************************//**
		 *	@brief		Stk_Camera构造函数
		 *	@param[in]	i_stkProtoTypeP		组件对象指针
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_Camera::Stk_Camera(void) :Stk_Object(new Stk_CameraImpl())
			
		{
			
		}

		/************************************************************************//**
		 *	@brief		Stk_Camera析构函数
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
		 *	@attention	无
		 ****************************************************************************/

		Stk_Camera::~Stk_Camera(void)
		{
			DELETE_STK_CLASS_IMPL
		}
		
		void Stk_Camera::SetProjectType(const STK_UINT32 &i_nuProjectType)
		{
			((Stk_CameraImpl*)m_pImpl)->SetProjectType(i_nuProjectType);
		}


		STK_UINT32 Stk_Camera::GetProjectType()
		{
			return ((Stk_CameraImpl*)m_pImpl)->GetProjectType();
		}


		void Stk_Camera::SetAngle(const STK_FLOAT32 &i_fAngle)
		{
			((Stk_CameraImpl*)m_pImpl)->SetAngle(i_fAngle);
		}

		STK_FLOAT32 Stk_Camera::GetAngle()
		{
			return ((Stk_CameraImpl*)m_pImpl)->GetAngle();
		}

		void Stk_Camera::SetOriginPoint(const STK_PNT32 &i_pntOrigin)
		{
			((Stk_CameraImpl*)m_pImpl)->SetOriginPoint(i_pntOrigin);
		}

		STK_PNT32 Stk_Camera::GetOriginPoint()
		{
			return ((Stk_CameraImpl*)m_pImpl)->GetOriginPoint();
		}

		void Stk_Camera::SetTargetVector(const STK_DIR32 &i_dirTargetVector)
		{
			((Stk_CameraImpl*)m_pImpl)->SetTargetVector(i_dirTargetVector);
		}

		STK_DIR32 Stk_Camera::GetTargetVector()
		{
			return ((Stk_CameraImpl*)m_pImpl)->GetTargetVector();
		}

		void Stk_Camera::SetUpVector(const STK_DIR32 &i_dirUpVector)
		{
			((Stk_CameraImpl*)m_pImpl)->SetUpVector(i_dirUpVector);
		}

		STK_DIR32 Stk_Camera::GetUpVector()
		{
			return ((Stk_CameraImpl*)m_pImpl)->GetUpVector();
		}

		void Stk_Camera::SetMatrixFlag(STK_BOOL i_bHasMatrix)
		{
			((Stk_CameraImpl*)m_pImpl)->SetMatrixFlag(i_bHasMatrix);
		}

		STK_BOOL Stk_Camera::GetMatrixFlag()
		{
			return ((Stk_CameraImpl*)m_pImpl)->GetMatrixFlag();
		}

		void Stk_Camera::SetMatrix(STK_MTX32 i_mtxTransform)
		{
			((Stk_CameraImpl*)m_pImpl)->SetMatrix(i_mtxTransform);
		}

		STK_MTX32 Stk_Camera::GetMatrix()
		{
			return ((Stk_CameraImpl*)m_pImpl)->GetMatrix();
		}

		void Stk_Camera::SetAspectRatio(const STK_FLOAT32 &i_fAspectRatio)
		{
			((Stk_CameraImpl*)m_pImpl)->SetAspectRatio(i_fAspectRatio);
		}

		STK_FLOAT32 Stk_Camera::GetAspectRatio()
		{
			return ((Stk_CameraImpl*)m_pImpl)->GetAspectRatio();
		}

		void Stk_Camera::SetFocalDistance(const STK_FLOAT32 &i_fFocalDistance)
		{
			((Stk_CameraImpl*)m_pImpl)->SetFocalDistance(i_fFocalDistance);
		}

		STK_FLOAT32 Stk_Camera::GetFocalDistance()
		{
			return ((Stk_CameraImpl*)m_pImpl)->GetFocalDistance();
		}

		void Stk_Camera::SetNearDistance(const STK_FLOAT32 &i_fNearlDistance)
		{
			((Stk_CameraImpl*)m_pImpl)->SetNearDistance(i_fNearlDistance);
		}

		STK_FLOAT32 Stk_Camera::GetNearDistance()
		{
			return ((Stk_CameraImpl*)m_pImpl)->GetNearDistance();
		}

		void Stk_Camera::SetFarDistance(const STK_FLOAT32 &i_fFarDistance)
		{
			((Stk_CameraImpl*)m_pImpl)->SetFarDistance(i_fFarDistance);
		}

		STK_FLOAT32 Stk_Camera::GetFarDistance()
		{
			return ((Stk_CameraImpl*)m_pImpl)->GetFarDistance();
		}

		void Stk_Camera::SetHeight(const STK_FLOAT32 &i_fHeight)
		{
			((Stk_CameraImpl*)m_pImpl)->m_fHeight = i_fHeight;
		}

		STK_FLOAT32 Stk_Camera::GetHeight()
		{
			return ((Stk_CameraImpl*)m_pImpl)->m_fHeight;
		}

		STK_STATUS Stk_Camera::FromJson(void* i_pJsonNode, void* i_JsonDocP)
		{
			return ((Stk_CameraImpl*)m_pImpl)->FromJson((Value*)i_pJsonNode, (Document*)i_JsonDocP);
		}

		STK_STATUS Stk_Camera::ToJson(void* i_pJsonNode, void* i_JsonDocP)
		{
			return ((Stk_CameraImpl*)m_pImpl)->ToJson((Value*)i_pJsonNode, (Document*)i_JsonDocP);
		}

	}//SVLLib
}//HoteamSoft