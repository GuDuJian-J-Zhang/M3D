// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*			
**************************************************************************/
#ifndef _SVLLIB_STK_UNITTEST_H_
#define _SVLLIB_STK_UNITTEST_H_
#include "Stk_Object.h"
#include <string>
// #include <vector>
// #include <map>

namespace HoteamSoft {
	namespace SVLLib {

 
class Stk_Document;
class STK_TOOLKIT_EXPORT Stk_UnitTest
{
public:
	Stk_UnitTest(void);
	virtual ~Stk_UnitTest(void);

	static void WriteMeshToBinFile();
	static void ReadMeshFromBinFile();

	static void ReadAssemblyTest(wstring& filePath,bool readMesh);
	static void ReadMeshTest(bool readMesh);

	static void FillAssembly(bool readMesh,void* pstk_NodePtr);

	static void FillPart(bool readMesh, void* pstk_ModelPtr);

};

	}//svllib
}//hoteamsoft

#endif