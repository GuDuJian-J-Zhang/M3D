#ifndef EFFECTMANAGER_H_
#define EFFECTMANAGER_H_

#include "m3d/M3D.h"
#include "m3d/action/RenderAction.h"
#include <map>
namespace M3D
{
	/************************************************************************/
	/* 效果管理类。用来管理effect所写的效果                                                                     */
	/************************************************************************/
	class Effect;
	class M3D_API EffectManager
	{
	public:		
		EffectManager(RenderAction* renderAction);
		//************************************
		// Method:    GetEffect
		// FullName:  M3D::EffectManager::GetEffect
		// Access:    public 
		// Returns:   Effect*
		// Qualifier: 根据名字获取effect
		// Parameter: string name
		//************************************
		Effect* GetEffect(string name);
		~EffectManager();
		//************************************
		// Method:    ClearResource
		// FullName:  M3D::EffectManager::ClearResource
		// Access:    public 
		// Returns:   void
		// Qualifier: 清理资源
		//************************************
		void ClearResource();
		//************************************
		// Method:    SetSize
		// FullName:  M3D::EffectManager::SetSize
		// Access:    public 
		// Returns:   void
		// Qualifier: 设置大小
		// Parameter: int w
		// Parameter: int h
		//************************************
		void SetSize(int w, int h);
	private:
		map<string, Effect*> m_effects;
		RenderAction* m_renderAction;
	};
}
#endif