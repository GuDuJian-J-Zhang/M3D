#include "m3d/extras/OperationHistoryManager.h"
#include "m3d/extras/Operation.h"

namespace M3D
{
OperationHistoryManager::OperationHistoryManager(void):Object()
{

}

OperationHistoryManager::~OperationHistoryManager()
{

}

const list<Operation*>& OperationHistoryManager::GetOperationList()
{
	return m_historyList;
}

bool OperationHistoryManager::Push(Operation* operation)
{
	bool addState = false;

	if(operation)
	{
		addState = true;
		operation->AddRef();
		m_historyList.push_back(operation);
	}

	return addState;
}

Operation*  OperationHistoryManager::Pop()
{
	Operation* topOperation =m_historyList.back();
	m_historyList.pop_back();

	return topOperation;
}

bool OperationHistoryManager::RemoveFromHistory(Operation* operation)
{
	bool removeState = false;

	if(operation)
	{
		m_historyList.remove(operation);
		removeState = true;
		operation->Release();
	}

	return removeState;
}

bool OperationHistoryManager::Clear()
{
	list<Operation*>::const_iterator it = m_historyList.begin();
	while (it != m_historyList.end())
	{
		Operation* operation = *it;
		operation->Release();
		it++;
	}
	m_historyList.clear();

	return true;
}
}

