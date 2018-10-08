/*
 * AssembleManager.cpp
 *
 *  Created on: 2015-12-24
 *      Author: CDJ
 */

#include "m3d/model/GeoAttribute.h"
#include "m3d/SceneManager.h"
#include "m3d/utils/M3DTools.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/ResourceManager.h"
#include "m3d/SceneManager.h"
#include "m3d/scenemanager.h"
#include "sview/utils/ViewHelper.h"
#include "m3d/extras/modelmanager/ModelManager.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/scene/ModelNode.h"
#include "m3d/Utils/FileHelper.h"
#include "sview/io/Reader.h"
#include "sview/io/Writer.h"
#include "m3d/model/Body.h"
#include "m3d/model/ShapeSet.h"
#include "m3d/extras/OperationHistoryManager.h"
#include "m3d/extras/modelmanager/RemoveModelOperation.h"
#include "m3d/extras/modelmanager/MoveModelOperation.h"
#include "m3d/extras/modelmanager/CopyModelOperation.h"
#include "m3d/extras/modelmanager/AddFileOperation.h"
#include "m3d/extras/modelmanager/RenameOperation.h"

using namespace M3D;
using namespace SVIEW;
namespace M3D
{

ModelManager::ModelManager(View * view):Object()
{
	m_commandHistoryManager = new OperationHistoryManager();
	m_view = view;
}

ModelManager::~ModelManager()
{
	// TODO Auto-generated destructor stub
}


bool ModelManager::Delete(Model* model)
{
	LOGI("ModelManager::Delete BEGIN");
	bool msg = false;

	RemoveModelOperation * removeModelCmd = new RemoveModelOperation(m_view,model);
	msg = removeModelCmd->execute();
	m_commandHistoryManager->Push(removeModelCmd);

	LOGI("ModelManager::Delete END");
	return msg;
}

bool ModelManager::MoveTo( Model* model,
		Model* parentModel)
{
	LOGI("ModelManager::MoveTo BEGIN)");
	bool msg = false;

	MoveModelOperation * moveModelCmd = new MoveModelOperation(m_view,model,parentModel);

	msg = moveModelCmd->execute();

	m_commandHistoryManager->Push(moveModelCmd);

	LOGI("ModelManager::MoveTo END)");
	return msg;

}

bool ModelManager::CopyTo( Model* model,
		Model* parentModel, Model* &newModel)
{
	LOGI(" ModelManager::CopyTo BEGIN");
	bool msg = false;

	CopyModelOperation * copyModelCmd = new CopyModelOperation(m_view,model,parentModel);

	msg = copyModelCmd->execute();

	newModel = copyModelCmd->GetNewMode();

	m_commandHistoryManager->Push(copyModelCmd);

	LOGI(" ModelManager::CopyTo END");
	return msg;
}

bool ModelManager::AddTo( string& fileName,
		Model* parentModel,Model*& retModel)
{
	LOGI(" ModelManager::AddTo BEGIN");
	bool msg = false;

	AddFileOperation * addFileCmd = new AddFileOperation(m_view,fileName,parentModel);

	msg = addFileCmd->execute();

	retModel = addFileCmd->GetNewModel();

	m_commandHistoryManager->Push(addFileCmd);

	LOGI(" ModelManager::AddTo END");
	return msg;

}

OperationHistoryManager* ModelManager::GetCommandHistoryManager()
{
	return m_commandHistoryManager;
}


bool ModelManager::ReName(Model * model,string& name)
{
	LOGI("ModelManager::ReName BEGIN");
	bool ret = false;

	RenameOperation * renameOperation = new RenameOperation(m_view,model,name);

	ret = renameOperation->execute();

	m_commandHistoryManager->Push(renameOperation);

	LOGI("ModelManager::ReName END");

	return ret;

}

}
































 /* namespace M3D */
