//
//  Writer.cpp
//  M3D
//
//  Created by administrator on 15/3/2.
//  Copyright (c) 2015年 hoteam. All rights reserved.
//

#include "Writer.h"

#include "m3d/M3D.h"
#include "m3d/model/Model.h"
#include "StlWriter.h"
#include "SvlWriter.h"


namespace SVIEW
{
    Writer::Writer(void)
    {
    }
    
    Writer::~Writer(void)
    {

    }
    
    Writer*
    Writer::GetWriter(const string &sourcePath)
    {
        Writer* writer = NULL;
        unsigned long dotIndex = sourcePath.find_last_of('.');
        if (dotIndex == -1)
            return writer;
        
        string ext = sourcePath.substr(dotIndex + 1);
        if ("stl" == ext || "STL" == ext)
            writer = new SvlWriter(false);
        else if("svl" == ext || "SVL" == ext)
            writer = new SvlWriter(true);
        
        if (writer != NULL)
        {
            writer->m_SourcePath    = sourcePath;
        }
        
        return writer;
    }

    int Writer::SaveTo(View* view,const string &targetPath)
    {
    	return -1;
    }

//    int Writer::SaveTo(Model* model,const string &targetPath)
//    {
//        return -1;
//    }
}
