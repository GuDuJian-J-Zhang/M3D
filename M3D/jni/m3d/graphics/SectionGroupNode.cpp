/*
 * SectionGroupNode.cpp
 *
 *  Created on: 2016-6-24
 *      Author: CDJ
 */

#include "m3d/graphics/SectionGroupNode.h"

namespace M3D
{

SectionGroupNode::SectionGroupNode()
{
	m_sceneManager = NULL;
}


SectionGroupNode::~SectionGroupNode()
{

}

void SectionGroupNode::SetScene(SceneManager* scenemanager)
{
	m_sceneManager = scenemanager;
}

SceneManager* SectionGroupNode::GetScene()
{
	return m_sceneManager;
}

} /* namespace M3D */
