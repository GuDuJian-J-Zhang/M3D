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
class WorkNodes;
class Section;
class SectionPlane;
class ModelView;

/**
 * @class
 * @brief 默认剖面方向沿X，Y，Z轴
 *
 *
 */
class M3D_API Direction
{
public:
	static const int X = 1;
	static const int Y = 2;
	static const int Z = 3;
};

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
	static void Show(SVIEW::View* pView, int direction, float percentage,
			bool isShowClipPlane, bool isShowCutPlane);

	static void Clear(SVIEW::View* pView);

	void Init(SVIEW::View* pView, int direction, float percentage,
			bool isShowClipPlane, bool isShowCutPlane);

	void Handle();
	void RequestDraw();

	void CreateSectionPlane(Model* model, int direction, float percentage,
			SectionPlane* outPlane);

	Model* GetModel()
	{
		return m_Model;
	}
    
    static SectionOperator* Instance;
    int m_Direction;
    float m_fPercentage;
    
private:
	ModelView* m_CurModelView;
    bool m_IsShowClipPlane;
    bool m_IsShowCutPlane;
	
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
