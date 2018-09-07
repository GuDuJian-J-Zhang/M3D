/**@file
 *@brief	剖面动作，完成剖面的添加，位置方向的控制
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */

#ifndef SECTIONACTION_H_
#define SECTIONAction_H_

#include "m3d/scene/ShapeNode.h"
namespace SVIEW
{
class View;
}

namespace M3D
{
class SceneManager;
class Model;
class ShapeNode;
class Section;
class SectionPlane;
class ModelView;
 
/**@class
 * @brief 控制剖面的显示，位置和方向
 *
 *
 *
 */
class M3D_API SectionOperator
{
public:
	static void GetSectionInfo(SVIEW::View* view, int direction,
			float percentage, SectionPlane* outPlane);

public:
	static void Show(SVIEW::View* pView, int id, int direction, float percentage,
			bool isShowClipPlane, bool isShowCappingPlane, bool isReverseClip);

	static void Clear(SVIEW::View* pView);

	void Init(SVIEW::View* pView, int id, int direction, float percentage,
			bool isShowClipPlane, bool isShowCappingPlane, bool isReverseClip);

	void Handle();
	void RequestDraw();

	void CreateSectionPlane(Model* model, int direction, float percentage,
			SectionPlane* outPlane);

	Model* GetModel()
	{
		return m_Model;
	}

public:
	ModelView* m_CurModelView;
    static SectionOperator* Instance;
	int m_Direction;
	float m_fPercentage;
    int m_DirectionX;
    float m_fPercentageX;
    int m_DirectionY;
    float m_fPercentageY;
    int m_DirectionZ;
    float m_fPercentageZ;
	bool m_IsShowClipPlane;
	bool m_IsShowCappingPlane;
	bool m_IsReverseClip;
protected:
private:


	SVIEW::View* m_pView;
	SceneManager* m_pSceneManager;
	ShapeNode* m_pModelNode;

	SectionPlane* m_pCurSectionPlane;

	Model* m_Model;

};
}

#endif //SECTIONACTION_H_
