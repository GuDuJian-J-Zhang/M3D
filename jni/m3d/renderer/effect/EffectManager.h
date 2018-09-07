#ifndef EFFECTMANAGER_H_
#define EFFECTMANAGER_H_

#include "m3d/M3D.h"
#include "m3d/action/RenderAction.h"
#include <map>
namespace M3D
{
	/************************************************************************/
	/* Ч�������ࡣ��������effect��д��Ч��                                                                     */
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
		// Qualifier: �������ֻ�ȡeffect
		// Parameter: string name
		//************************************
		Effect* GetEffect(string name);
		~EffectManager();
		//************************************
		// Method:    ClearResource
		// FullName:  M3D::EffectManager::ClearResource
		// Access:    public 
		// Returns:   void
		// Qualifier: ������Դ
		//************************************
		void ClearResource();
		//************************************
		// Method:    SetSize
		// FullName:  M3D::EffectManager::SetSize
		// Access:    public 
		// Returns:   void
		// Qualifier: ���ô�С
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