/**@file
 *@brief	文件修改器基类
 *@author	liubing
 *@date		2015-3-2
 *@version	1.0
 *
 *文件修改器基类
 *
 */
#ifndef  M3D__Writer__
#define  M3D__Writer__
#include "M3D/M3DExport.h"
#include "sview/SView.h"

namespace M3D {
    class Model;
	class Model;
    class TriMeshData;
    class IDCreator;
    class Operation;
}
using M3D::Model;
using M3D::Model;
using M3D::TriMeshData;
using M3D::IDCreator;
using M3D::Operation;

namespace SVIEW
{
	class View;

    /**@class Reader
     *@brief  文件修改器基类
     *
     *提供修改器的基本方法。
     *
     */
    class M3D_API Writer {
    public:
        Writer(void);
        virtual ~Writer(void);
        
        /**
         * @brief 根据文件扩展名获取具体Reader
         * @param file 要读取的文件的路径
         * @note 为stl时返回stlreader，为svl时返回Svlreader
         * */
        static Writer* GetWriter(const string &sourcePath);

        virtual int SaveTo(View* view,const string &targetPath);

    protected:

        string m_SourcePath;
    private:
        
        
    };
}

#endif /* defined(__M3D__Writer__) */
