#ifndef     M3D_ANNOTATION__H_
#define     M3D_ANNOTATION__H_

#include "m3d/model/Note.h"

namespace M3D
{
	class M3D_API Annotation : public Note
	{
	public: 
		Annotation();
		virtual ~Annotation();

		virtual void FindVisiableObject(RenderAction* renderAction);
		/**
		* @brief 获取批注位置
		* @return
		*/
		Vector3& GetAnnotationPos();
		/**
		* @brief 设置批注位置
		* @param pos 要设置的位置
		*/
		void SetAnnotationPos(const Vector3& pos);
		/**
		* @brief 获取文本位置
		* @return 位置
		*/
		Vector3& GetTextsPos();
		/**
		* @brief 设置文本位置
		* @param pos 位置
		*/
		void SetTextsPos(const Vector3& pos);

		Vector3& GetCenterPos();
		void SetCenterPos(const Vector3& pos);

		Matrix4* GetOutFrameMatrix();
		Matrix4* GetTextMatrix();

		bool GetHaveStub();
		void SetHaveStub(bool value);

		bool GetHaveEnvelope();
		void SetHaveEnvelope(bool value);

		bool GetHaveLeaderLine();
		void SetHaveLeaderLine(bool value);

		bool GetFixed();
		void SetFixed(bool value);

		string getText();
		void SetText(string value);

		float GetPosAverage();
		void SetPosAverage(float data);

		float GetGap();
		void SetGap(float data);



		/**
		* 设置填充颜色
		* @param color 颜色
		* @param g
		* @param b
		* @param a
		*/
		void SetFillColor(const Color &color);


		/**
		* 获取填充颜色
		* @return
		*/
		virtual Color* GetFillColor();

		/**
		* 设置外框颜色
		* @param color 颜色
		* @param g
		* @param b
		* @param a
		*/
		void SetFrameColor(const Color &color);

		/**
		* 获取外框颜色
		* @return
		*/
		Color* GetFrameColor();

	private:
		Matrix3x4 GetWorldMatrix(const Matrix3x4& mvMatrix,
			RenderAction* renderAction, const Vector3& center, bool allowTran,
			bool allowRotate, bool allowScale);
		BoundingBox GetComTextsBox();
	private:
		/**
		* @brief 初始化
		*/
		void Init();

		Vector3 m_annotationPos;//!<批注位置

		Vector3 m_centerPos;//!<中间位置

		Vector3 m_textPos;//!<文字显示位置

		string m_textValue;//!<文本内容

		Matrix3x4 m_WorldMatrix; //!<定义平面

		Matrix4 m_outFrameMatrix; //!<外框显示时的方向和位置矫正矩阵，为了实现朝向屏幕和固定屏幕效果
		Matrix4 m_textMatrix; //!<外框显示时的方向和位置矫正矩阵，为了实现朝向屏幕和固定屏幕效果

		bool m_bLeaderLine;  //!<引线
		bool m_bStub;      //!<引线折弯
		bool m_bEnvelope;  //!<包络
		bool m_bFixed;  //!<固定屏幕

		float m_fTextPos;
		float m_fgap; //!<拖拽批注时使用的临时距离变量

		Color m_FillColor; //!<填充颜色
		Color m_FrameColor; //!<外框颜色

		mutable Mutex m_mutex; //!<线程锁对象
	};
}

#endif