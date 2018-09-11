#include "m3d/Handler/HandlerNode.h"
#include "m3d/Handler/Handler.h"
#include "m3d/scene/ShapeNode.h"

namespace M3D
{
HandlerNode::HandlerNode() :
		GroupNode()
{
	m_bRotate = true;
	m_bTranslate = true;
	m_bScaleState = false;
	m_bInTop = true;
}

HandlerNode::~HandlerNode()
{

}


bool HandlerNode::InTop()
{
	return this->m_bInTop;
}

bool HandlerNode::IsRotate()
{
	return this->m_bRotate;
}
bool HandlerNode::IsTranslate()
{
	return this->m_bTranslate;
}
bool HandlerNode::IsScale()
{
	return this->m_bScaleState;
}

void HandlerNode::AllowRotate(bool state)
{
	this->m_bRotate = state;
}
void HandlerNode::AllowTranslate(bool state)
{
	this->m_bTranslate = state;
}
void HandlerNode::AllowScale(bool state)
{
	this->m_bScaleState = state;
}

int HandlerNode::GetType( void )
{
	return HANDLER_NODE;
}

void HandlerNode::SetActiveHandler(Handler* activeHandler)
{
	this->m_ActiveHandler = activeHandler;
}

Handler* HandlerNode::GetActiveHandler()
{
	if(this->m_HandlerDirty)
	{
		this->m_HandlerDirty = false;
		UpdataActiveHandler();
	}
	return this->m_ActiveHandler;
}

void HandlerNode::UpdataActiveHandler()
{
	this->m_ActiveHandler = NULL;

	for (int i = 0; i < m_pChildrenList->Size(); i++)
	{
		SceneNode *node = m_pChildrenList->GetSceneNode(i);
		;
		if (node != NULL && node->GetType() == SHAPE_NODE)
		{
			ShapeNode* shapeNode = (ShapeNode*) node;
			Handler* handler = (Handler*) shapeNode->GetShape();
			if (handler)
			{
				if (handler->IsSelected())
				{
					this->m_ActiveHandler = handler;

					break;
				}
			}
		}
	}
}


}

