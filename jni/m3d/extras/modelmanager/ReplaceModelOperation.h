/**@file
*@brief
*@author	QYP
*@date		2018-06-07
*@version	1.0
*@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
*
*/
#ifndef M3D_REPLACEMODEL_COMMAND_H
#define M3D_REPLACEMODEL_COMMAND_H

#include 	"m3d/extras/Operation.h"

namespace M3D
{
	/**
	* @class �滻ģ�Ͳ���
	*/
	class ReplaceModelOperation :public Operation
	{
	public:
		/**
		* @brief ��ʼ��
		* @param view ��ǰ��ͼ
		* @param oldModel �����滻��ģ��
		* @param oldParentModel �����滻��ģ�͵ĸ�ģ��
		* @param filePath �����滻���ļ�ȫ·��
		*/
		ReplaceModelOperation(View * view, Model* oldModel, Model* oldParentModel, string& filePath);
		virtual ~ReplaceModelOperation();
	public:
		/**
		* @Brief ִ�в���
		* @return �ɹ� true ʧ�� false
		*/
		bool OnExecute();
		/**
		* @brief ��ȡ�µ�ģ��
		* @return ����ģ��
		*/
		Model * GetNewModel() const;
		/**
		* @brief ʹ��ԭ��ģ�͵������滻��ģ�͵����ݣ�����ģ�ͣ�
		* @param oldModel ԭ��ģ�� newModel ��ģ��
		* @return ��
		*/
		void ReplaceModelData(Model* oldModel, Model* newModel);
	private:
		/**
		* @brief ��ʼ��
		* @param view ��ǰ��ͼ
		* @param oldModel �����滻��ģ��
		* @param oldParentModel �����滻��ģ�͵ĸ�ģ��
		* @param filePath �����滻���ļ�ȫ·��
		*/
		void Init(View * view, Model* oldModel, Model* oldParentModel, string& filePath);

	protected:
		Model*  m_oldModel;//!<���滻��ģ��
		Model*  m_oldParentModel;//!<���滻��ģ�͸�ģ��
		string  m_filePath;//!<�ļ�·��
		mutable Model*  m_newModel;//!<�½���ģ��
	};
}

#endif //M3D_REPLACEMODEL_COMMAND_H