#include "m3d/renderer/effect/EffectManager.h"
#include "m3d/renderer/effect/Effect.h"
#include "m3d/renderer/effect/JewelEffect.h"
#include "m3d/action/RenderAction.h"
#include "m3d/renderer/effect/OutlineEffect.h"
#include "m3d/renderer/effect/SSAOEffect.h"
#include "m3d/renderer/effect/SAOEffect.h"
#include "m3d/renderer/effect/XRayEffect.h"
namespace M3D 
{

	EffectManager::EffectManager(RenderAction* renderAction)
	{
		this->m_renderAction = renderAction;
	}

	Effect* EffectManager::GetEffect(string name)
	{
		if (m_effects.find(name) == m_effects.end())
		{
			if (name == "JEWELEFFECT")
			{
				Effect * jewelEffect = new JewelEffect(this->m_renderAction);
				m_effects[name] = jewelEffect;
				return jewelEffect;
			}
			else if (name == "OUTLINEEFFECT")
			{
				Effect * outlineEffect = new OutlineEffect(this->m_renderAction);
				m_effects[name] = outlineEffect;
				return outlineEffect;
			}
			else if (name == "SSAOEFFECT")
			{
				Effect * ssaoEffect = new SSAOEffect(this->m_renderAction);
				m_effects[name] = ssaoEffect;
				return ssaoEffect;
			}
			else if (name == "SAOEFFECT")
			{
				Effect * ssaoEffect = new SAOEffect(this->m_renderAction);
				m_effects[name] = ssaoEffect;
				return ssaoEffect;
			}
			else if (name == "XRAYEFFECT")
			{
				Effect * xrayEffect = new XRayEffect(this->m_renderAction);
				m_effects[name] = xrayEffect;
				return xrayEffect;
			}
			else {
				return nullptr;
			}
		}
		else
		{
			return m_effects[name];
		}
	}

	EffectManager::~EffectManager()
	{
		map<string, Effect*>::iterator it = m_effects.begin();
		for (;it != m_effects.end();++it)
		{
			if (it->second)
			{
				delete it->second;
				it->second = NULL;
			}
		}
		m_effects.clear();
	}

	void EffectManager::ClearResource()
	{
		map<string, Effect*>::iterator it = m_effects.begin();
		for (;it != m_effects.end();it++)
		{
			it->second->ClearResource();
		}
	}

	void EffectManager::SetSize(int w, int h)
	{
		map<string, Effect*>::iterator it = m_effects.begin();
		for (; it != m_effects.end(); it++)
		{
			it->second->SetSize(w,h);
		}
	}

}
