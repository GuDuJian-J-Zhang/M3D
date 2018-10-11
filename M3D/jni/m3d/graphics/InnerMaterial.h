#ifndef M3D_INNER_MATERIAL_H_
#define M3D_INNER_MATERIAL_H_


#include "m3d/graphics/BaseMaterial.h"
namespace M3D
{
	/************************************************************************/
	/* �ڲ�����.��phong��pbr�ȵĻ���                                                                     */
	/************************************************************************/
	class M3D_API InnerMaterial :public BaseMaterial
	{
	public:
		InnerMaterial();
		InnerMaterial(InnerMaterial& org);
		~InnerMaterial();
		virtual BaseMaterial* Clone();
		//************************************
		// Method:    UpdateUvTransform
		// FullName:  M3D::InnerMaterial::UpdateUvTransform
		// Access:    public 
		// Returns:   void
		// Qualifier: ����uv����任���󣬸þ���Ӱ����������е�����
		//************************************
		void UpdateUvTransform();
		//************************************
		// Method:    SetUvTranslate
		// FullName:  M3D::InnerMaterial::SetUvTranslate
		// Access:    public 
		// Returns:   void
		// Qualifier:ƽ��
		// Parameter: Vector2 & translate
		//************************************
		void SetUvTranslate(Vector2& translate);
		//************************************
		// Method:    SetUvScale
		// FullName:  M3D::InnerMaterial::SetUvScale
		// Access:    public 
		// Returns:   void
		// Qualifier: ����
		// Parameter: Vector2 & scale
		//************************************
		void SetUvScale(Vector2&scale);
		//************************************
		// Method:    SetUvRotate
		// FullName:  M3D::InnerMaterial::SetUvRotate
		// Access:    public 
		// Returns:   void
		// Qualifier: ��ת
		// Parameter: float rotate
		//************************************
		void SetUvRotate(float rotate);
		//************************************
		// Method:    GetUvTransform
		// FullName:  M3D::InnerMaterial::GetUvTransform
		// Access:    public 
		// Returns:   Matrix3&
		// Qualifier: ��ȡ�任����
		//************************************
		Matrix3& GetUvTransform();

		Vector2 GetUvTranslate();
		Vector2 GetUvScale();
		float GetUvRotate();

	protected:
		//uv transform
		Matrix3 m_uvTransform;
		Vector2 m_uvTranslate;
		Vector2 m_uvScale;
		float m_uvRotate;
		bool m_updateUvTransform;
		
		
	};
}
#endif//M3D_INNER_MATERIAL_H_

