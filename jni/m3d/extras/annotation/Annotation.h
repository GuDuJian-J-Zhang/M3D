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
		* @brief ��ȡ��עλ��
		* @return
		*/
		Vector3& GetAnnotationPos();
		/**
		* @brief ������עλ��
		* @param pos Ҫ���õ�λ��
		*/
		void SetAnnotationPos(const Vector3& pos);
		/**
		* @brief ��ȡ�ı�λ��
		* @return λ��
		*/
		Vector3& GetTextsPos();
		/**
		* @brief �����ı�λ��
		* @param pos λ��
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
		* ���������ɫ
		* @param color ��ɫ
		* @param g
		* @param b
		* @param a
		*/
		void SetFillColor(const Color &color);


		/**
		* ��ȡ�����ɫ
		* @return
		*/
		virtual Color* GetFillColor();

		/**
		* ���������ɫ
		* @param color ��ɫ
		* @param g
		* @param b
		* @param a
		*/
		void SetFrameColor(const Color &color);

		/**
		* ��ȡ�����ɫ
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
		* @brief ��ʼ��
		*/
		void Init();

		Vector3 m_annotationPos;//!<��עλ��

		Vector3 m_centerPos;//!<�м�λ��

		Vector3 m_textPos;//!<������ʾλ��

		string m_textValue;//!<�ı�����

		Matrix3x4 m_WorldMatrix; //!<����ƽ��

		Matrix4 m_outFrameMatrix; //!<�����ʾʱ�ķ����λ�ý�������Ϊ��ʵ�ֳ�����Ļ�͹̶���ĻЧ��
		Matrix4 m_textMatrix; //!<�����ʾʱ�ķ����λ�ý�������Ϊ��ʵ�ֳ�����Ļ�͹̶���ĻЧ��

		bool m_bLeaderLine;  //!<����
		bool m_bStub;      //!<��������
		bool m_bEnvelope;  //!<����
		bool m_bFixed;  //!<�̶���Ļ

		float m_fTextPos;
		float m_fgap; //!<��ק��עʱʹ�õ���ʱ�������

		Color m_FillColor; //!<�����ɫ
		Color m_FrameColor; //!<�����ɫ

		mutable Mutex m_mutex; //!<�߳�������
	};
}

#endif