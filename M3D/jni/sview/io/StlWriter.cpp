//
//  StlWriter.cpp
//  M3D
//
//  Created by administrator on 15/3/2.
//  Copyright (c) 2015年 hoteam. All rights reserved.
//

#include "StlWriter.h"
#include "Writer.h"
#include "Utility.h"

#include "prebuilt/svl2/include/Stk_Define.h"
#include "prebuilt/svl2/include/Stk_platform.h"
#include "prebuilt/svl2/include/Stk_DocumentManager.h"
#include "prebuilt/svl2/include/Stk_Instance.h"
#include "prebuilt/svl2/include/Stk_ProtoType.h"
#include "prebuilt/svl2/include/Stk_GeoAttribute.h"
#include "prebuilt/svl2/include/Stk_CompositeText.h"
#include "prebuilt/svl2/include/Stk_LineSetEntity.h"

namespace SVIEW
{
    StlWriter::StlWriter(void)
    {
        
    }
    StlWriter::~StlWriter()
    {
        
    }

//    int StlWriter::SaveTo(Model* model,const string &targetPath)
//    {
//        //Writer::SaveTo(targetPath);
//        Stk_DocumentManager *m_stkDocMgr = new Stk_DocumentManager();
//        
//        wstring wsFile = Stk_Platform::StringToWString(m_SourcePath);
//        
//        STK_STATUS status = m_stkDocMgr->LoadDocument(wsFile);
//        if (status != STK_SUCCESS)
//        {
//            LOGE("SvlWriter::SaveTo Error!");
//            return -1;
//        }
//        
//        //save
//        wstring tarFile = Stk_Platform::StringToWString(targetPath);
//        m_stkDocMgr->SaveDocumentAs(tarFile, FILESAVE_TYPE_UNIQUE);
        
//        return 0;
//    }
    int StlWriter::SaveTo(View* view,const string &targetPath)
    {
    	 return 0;
    }
}
