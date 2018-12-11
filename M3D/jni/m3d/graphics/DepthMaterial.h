#ifndef M3D_DEPTH_MATERIAL_H_
#define M3D_DEPTH_MATERIAL_H_


#include "m3d/graphics/InnerMaterial.h"

namespace M3D
{
	/************************************************************************/
	/* ��������������������ͼ                                                                     */
	/************************************************************************/
	class M3D_API DepthMaterial :public InnerMaterial
	{
	public:
		DepthMaterial();
		DepthMaterial(DepthMaterial& org);
		~DepthMaterial();
		virtual BaseMaterial* Clone();
		bool GetUseLight() const ;
		void SetUseLight(bool val) ;
		
	};
}
#endif//M3D_DEPTH_MATERIAL_H_

