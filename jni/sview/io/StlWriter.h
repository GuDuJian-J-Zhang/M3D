//
//  StlWriter.h
//  M3D
//
//  Created by administrator on 15/3/2.
//  Copyright (c) 2015年 hoteam. All rights reserved.
//

#ifndef __M3D__StlWriter__
#define __M3D__StlWriter__
#include "m3d/M3D.h"
#include "Writer.h"


namespace SVIEW
{
    class M3D_API StlWriter: public Writer {
    public:
        StlWriter(void);
        virtual ~StlWriter();
//        virtual int SaveTo(Model* model,const string &targetPath);
        virtual int SaveTo(View* view,const string &targetPath);
    };
}

#endif /* defined(__M3D__StlWriter__) */
