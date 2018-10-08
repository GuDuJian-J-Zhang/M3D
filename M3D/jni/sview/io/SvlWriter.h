//
//  SvlWriter.h
//  M3D
//
//  Created by administrator on 15/3/2.
//  Copyright (c) 2015年 hoteam. All rights reserved.
//

#ifndef  M3D_SVLWRITER_
#define  M3D_SVLWRITER_

#include "sview/io/Writer.h"

class Stk_Instance;
class Stk_ProtoType;
class Stk_DocumentManager;
class Stk_ProtoType;

namespace SVIEW
{
    class M3D_API SvlWriter: public Writer {
    	//遍历instance时的函数指针
        typedef void (SvlWriter::*P_InstanceFunc)(Stk_Instance* ins,M3D::Model* model);
    public:
        SvlWriter(bool isSVL);
        virtual ~SvlWriter();
//        virtual int SaveTo(Model* model,const string &targetPath);
        virtual int SaveTo(View* view,const string &targetPath);
        //保存到一个新的svl（数据从model获取，如stl格式）
        int SaveToNewSVL(Model* model,const string &targetPath);

        //递归创建proto数据（新建时使用）
        int SetProtoTypeDataRecursion(Model* curModel,Stk_ProtoType* curProto,Stk_DocumentManager * curDocMgr);

        //保存到旧的svl（如果是原本是svl）
        int SaveByModifyOldSVL();

    private:

//        //递归修改instance信息（修改时使用）
//        void SetInstanceDataRecursionForModify(Stk_Instance* ins,            //当前ins
//                                      P_InstanceFunc pInstanceFunc, //递归中调用的函数指针
//                                      string* pStrParentPlacePath   //父级的placePath
//                                      );
        void ModifyInstance(Stk_Instance* ins, M3D::Model* model);
        
        void ModifyProto(Stk_ProtoType* proto, M3D::Model* model);
        
        bool ApplyOperationToSVLFile(Operation* operation,Stk_DocumentManager * stkDocMgr);

        /**
         * 将颜色保存到SVL对应的实例
         * @param stkDocMgr
         * @return
         */
        bool SaveAnthorInfoToSVLFile();

        Stk_ProtoType* GetDefaultProto(Stk_DocumentManager * stkDocMgr);


        /**
         * 初始化内部数据
         * @param model
         */
        void init();
    private:
//        map<string,Model *> m_CurModelMap;//instanceID,Model
        vector<Model*> m_allModels;
        bool isSavingSVL;
        Stk_DocumentManager *m_stkDocMgr;
        Stk_ProtoType* m_topProtoType;
        View* m_view;
        string m_targetPath;
    };
}

#endif /* defined(__M3D__SvlWriter__) */
