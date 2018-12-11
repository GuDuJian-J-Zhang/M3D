/**@file
*@brief
*@author	WBY
*@date		2018-11-2
*@version	1.0
*
*/

#ifndef M3D_PIVOT_CALLBACK_H_
#define M3D_PIVOT_CALLBACK_H_

#include "m3d/handler/dragger.h"

//定义回调函数类型
#ifdef WIN32
typedef void(_stdcall M3D_PIVOTCB)(const int& nCmdType, wstring& strDraggerName, Matrix3x4& matrix3x4);
#else
typedef void(M3D_PIVOTCB)(const int& nCmdType, wstring& strDraggerName, Matrix3x4& matrix3x4);
#endif // WIN32

namespace M3D
{
	class M3D_API PivotCallback :public DraggerCallback
	{
	public:
		PivotCallback();
		PivotCallback(Matrix3x4& ma);
		virtual ~PivotCallback();
		/**
		* Receive motion commands. Returns true on success.
		*/
		virtual bool receive(const MotionCommand&);
		virtual bool receive(const TranslateInLineCommand& command);
		virtual bool receive(const TranslateInPlaneCommand& command);
		//virtual bool receive(const Scale1DCommand& command);
		//virtual bool receive(const Scale2DCommand& command);
		//virtual bool receive(const ScaleUniformCommand& command);
		virtual bool receive(const Rotate3DCommand& command);

		bool AddModel(Model* model);
		void AddModels(vector<Model*> models);
		void ClearModel();
		void SetPivotCB(M3D_PIVOTCB* pivotCB) { m_pivotCB = pivotCB; }
		Matrix3x4 ma;

	private:
		vector<Model*> m_draggerModels;
		M3D_PIVOTCB* m_pivotCB;
	};
}
#endif // SHAPE_NODE_H