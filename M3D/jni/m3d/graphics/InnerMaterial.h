#ifndef M3D_INNER_MATERIAL_H_
#define M3D_INNER_MATERIAL_H_


#include "m3d/graphics/BaseMaterial.h"
namespace M3D
{
	/************************************************************************/
	/* 内部纹理.是phong和pbr等的基类                                                                     */
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
		// Qualifier: 更新uv坐标变换矩阵，该矩阵影响材质中所有的纹理
		//************************************
		void UpdateUvTransform();
		//************************************
		// Method:    SetUvTranslate
		// FullName:  M3D::InnerMaterial::SetUvTranslate
		// Access:    public 
		// Returns:   void
		// Qualifier:平移
		// Parameter: Vector2 & translate
		//************************************
		void SetUvTranslate(Vector2& translate);
		//************************************
		// Method:    SetUvScale
		// FullName:  M3D::InnerMaterial::SetUvScale
		// Access:    public 
		// Returns:   void
		// Qualifier: 缩放
		// Parameter: Vector2 & scale
		//************************************
		void SetUvScale(Vector2&scale);
		//************************************
		// Method:    SetUvRotate
		// FullName:  M3D::InnerMaterial::SetUvRotate
		// Access:    public 
		// Returns:   void
		// Qualifier: 旋转
		// Parameter: float rotate
		//************************************
		void SetUvRotate(float rotate);
		//************************************
		// Method:    GetUvTransform
		// FullName:  M3D::InnerMaterial::GetUvTransform
		// Access:    public 
		// Returns:   Matrix3&
		// Qualifier: 获取变换矩阵
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

