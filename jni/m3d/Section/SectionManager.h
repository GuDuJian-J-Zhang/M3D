/**@file
 *@brief
 *@author	CDJ
 *@date		2015-12-24
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_SECTION_MANAGER_H
#define M3D_SECTION_MANAGER_H

#include "m3d/M3D.h"

namespace SVIEW
{
	class View;
}
 
namespace M3D
{
	class Dragger;
	class SectionPlane;
	/**
	* @class
	* @brief 默认剖面方向沿X，Y，Z轴
	*
	*
	*/
	class M3D_API Direction
	{
	public:
		static const int X = 1;
		static const int Y = 2;
		static const int Z = 3;
	};
	/**
	 * @brief 模型管理器，用来对管理对模型的操作
	 */
	class M3D_API SectionManager :public Object
	{
	public:
		SectionManager(SVIEW::View * view);

		virtual ~SectionManager();

	public:
		void OpenThreePlaneSection();

		M3D::Dragger* BindSectionDragger(int draggerType);

		Dragger* InitBindSectionDragger(int draggerType);
		M3D::Dragger* BindSectionAxisDragger();

		M3D::Dragger* BindSectionRotateDragger();


		M3D::Dragger* InitBindSectionAxisDragger();

		M3D::Dragger* InitBindSectionRotateDragger();

		void ShowAxisDragger();

		void ShowRotateDragger();

		void ResetThreePlaneSection();

		void CloseAllSectionPlanes();

		void GetSectionInfo(SVIEW::View* view, int direction,
			float percentage, SectionPlane* outPlane);

		bool GetEnableXY() const;
		void SetEnableXY(bool val);
		bool GetEnableXZ() const;
		void SetEnableXZ(bool val);
		bool GetEnableYZ() const;
		void SetEnableYZ(bool val);
		bool GetShowPlaneRect() const;
		void SetShowPlaneRect(bool val);
		bool GetShowClipPlane() const;
		void SetShowClipPlane(bool val);
		bool GetShowCappingPlane() const;
		void SetShowCappingPlane(bool val);
		bool GetReversePlane();
		void SetReversePlane(bool val);
		void UnBindAllDragger();
		void SetDraggerPlaneVisible();
	private:
		bool _enableXY; //!<开启XY平面剖切
		bool _enableXZ; //!<开启XZ平面剖切
		bool _enableYZ; //!<开启YZ平面剖切
		bool _ShowPlaneRect; //!<是否显示平面长方形
		bool _ShowClipPlane; //!<是否显示盖面的截面（剖面时使用）
		bool _ShowCappingPlane; //!<是否显示剖面的截面（剖面时使用）
		bool _ReverseClipPlane; //!<是否显示剖面的截面（剖面时使用）

		int _xyPlaneID; //内部使用的三个剖切平面的id
		int _xzPlaneID;
		int _yzPlaneID;

		bool _firstOpen;

		Dragger* bindAxisDragger;
		Dragger* bindRotationDragger;

		SVIEW::View * m_view;//!<当前视图
	};

} /* namespace M3D */
#endif /* M3D_MODEL_MANAGER_H */
