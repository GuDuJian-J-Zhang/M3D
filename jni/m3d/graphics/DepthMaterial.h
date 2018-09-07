#ifndef M3D_DEPTH_MATERIAL_H_
#define M3D_DEPTH_MATERIAL_H_


#include "m3d/graphics/InnerMaterial.h"

namespace M3D
{
	/************************************************************************/
	/* 深度纹理。用来生成深度贴图                                                                     */
	/************************************************************************/
	class M3D_API DepthMaterial :public InnerMaterial
	{
	public:
		DepthMaterial();
		~DepthMaterial();
		bool GetUseLight() const override;
		void SetUseLight(bool val) override;
		
	};
}
#endif//M3D_DEPTH_MATERIAL_H_

