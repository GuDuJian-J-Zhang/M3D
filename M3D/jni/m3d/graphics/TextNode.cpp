/*
 * TextNode.cpp
 *
 *  Created on: 2013-9-28
 *      Author: bing
 */

#include "m3d/graphics/TextNode.h"
#ifdef __MOBILE__
#include <FTGL/ftgles.h>
#endif
namespace M3D
{



TextNode::TextNode()
{
	m_pText = NULL;
	m_IsFixedSize = false;
	m_IsFrontDisplay = false;
	m_IsFaceToCamera = false;
	m_IsFixedLocation = false;

	m_color.SetColor(1, 0, 0, 1);

	m_Pos.ToZero();
}

TextNode::~TextNode()
{
	if (m_pText != NULL)
	{
		free(m_pText);
	}
}

void TextNode::SetText(char * pText)
{
	if (m_pText)
		free(m_pText);
	m_pText = (char*) malloc(strlen(pText) + 1);
	strcpy(m_pText, pText);
}

//Note that the order of release
//void TextNode::FinalRelease(void)
//{
//	if (m_pText)
//		free(m_pText);
//	Object::FinalRelease();
//}

int TextNode::GetType(void)
{
	return TEXT_NODE;
}

}
