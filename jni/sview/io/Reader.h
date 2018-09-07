/**@file
 *@brief	文件读取器基类
 *@author	billy
 *@date		2013-5-31
 *@version	1.0
 *
 *文件读取器基类
 *
 */

#ifndef READER_H_
#define READER_H_
#include "sview/SView.h"
#include "m3d/base/Mutex.h"

namespace M3D {
class Object;
class Model;
class IVertexSet;
class VertexSet;
class Mesh;
class IDCreator;
class BaseMaterial;
class Color;
}
using M3D::Object;
using M3D::Model;
using M3D::IVertexSet;
using M3D::VertexSet;
using M3D::Mesh;
using M3D::IDCreator;
using M3D::BaseMaterial;
using M3D::Color;

#define SHOW_READER_LOG false
#define  READER_LOGI(...)  if(SHOW_READER_LOG) LOGI(__VA_ARGS__)
#define  READER_LOGE(...)  if(SHOW_READER_LOG) LOGE(__VA_ARGS__)

#ifdef __MOBILE__

#define INDEX_MAX_COUNT 65535
#endif

#ifdef _WIN32
#define INDEX_MAX_COUNT 4294967295
#endif

namespace SVIEW
{
class View;
class Reader;
class M3D_API ReaderListener :public M3D::Object{
public:
	ReaderListener();
	virtual ~ReaderListener();
	virtual void OnProcessPercent(Reader* reader,float percent);
	virtual void OnBegin(Reader* reader);
	virtual void OnEnd(Reader* reader);
};

/**@class Reader
 *@brief  文件读取器基类
 *
 *提供读取器的基本方法。
 *
 */
class M3D_API Reader {
public:
	Reader(void);
	virtual ~Reader(void);

	/**
	 * @brief 根据文件扩展名获取具体Reader
	 * @param file 要读取的文件的路径
	 * @note 为stl时返回stlreader，为svl时返回Svlreader
	 * */
	static Reader* GetReader(const string & file);

	/**
	 * 返回当前文件路径
	 * @return 文件路径
	 */
	string GetFile();

	/**
	 * 设置当前文件路径
	 * @param file 文件路径
	 */
	void SetFile(string file);

	/**
	 * 设置要填充的View
	 * @param view
	 */
	void SetView(View* view);

	/**
	 * 获取顶级model
	 * @return
	 */
	virtual Model* GetModel();

	/**
	 * 得到所有的资源文件
	 * @return
	 */
	set<string>& GetSourceFiles();

	/**
	 * 添加资源路径到文件中
	 * @param filePath
	 * @return
	 */
	bool AddSourceFile(const string& filePath);

	/**
	 * 根据id获取model
	 * @param id
	 * @return Model指针
	 */
	virtual Model* GetModel(int id);

	/**
	 *根据Model id获取MeshData
	 * @param id
	 * @return
	 */
	virtual IVertexSet* GetMeshData(int id);

	/**
	 * 返回是否取消状态
	 * @return
	 */
	bool IsCancel();

	/**
	 * 设置取消
	 */
	virtual void Cancel();

	/**
	 * 清除缓存内容
	 */
	virtual void Clear();

	/**
	 * 设置id生成器
	 * @param creator
	 */
	void SetIDCreator(IDCreator* creator) {
		this->m_IDCreator = creator;
	}

	/**
	 * 获取新model的id
	 * @return
	 */
	virtual int GetNewID();

	/**
	 * 初始化id
	 * @return
	 */
	virtual int InitializeID();

	virtual const string& GetXMLAnimationData();
	virtual void SetXMLAnimationData(const string& xmlData);


	/**
	 * 设置读取文件百分比
	 * @param percent
	 */
	void SetPercent(float percent);

	/**
	 * 获取文件处理百分比
	 * @return
	 */
	float GetPercent();

	void SetListener(ReaderListener* listener);
	ReaderListener* GetListener();

	bool IsAsynMode();
	void SetAsynMode(bool asynMode);

	/**
	* 是否使用索引方式进行数据存储
	* @return
	*/
	bool IsUseIndex();

	/**
	* 设置以索引方式进行数据存储
	* @param useIndex
	*/
	void SetUseIndex(bool useIndex);

	BaseMaterial * CovertColorToMaterial(Color& srcColor);

	/**
	* 通过索引个数和自身是否允许使用索引条件，判断能够使用索引
	* @param indexNum
	* @return
	*/
	bool CanUseIndex(int indexNum);
//    /**
//     * 得到读取过程中发送的消息
//     * @return
//     */
//    string& GetReadLogs();

protected:
	//int newId;
	string m_File;
	bool m_isCancel;
	bool m_isNativeDisplay;
	IDCreator* m_IDCreator;
    //string m_readLogs;
    set<string> m_srcFiles;

    float m_readPercent;//!< 当前读取的百分比

    ReaderListener* m_readerListener;
	mutable M3D::Mutex m_mutex;//!<线程锁
    View* m_view;
    static string defaultAnimationData;

	bool m_isAnsyMode;//是否采用异步方式读取

	bool m_isUseIndex; //!<是否使用索引方式进行数据存储
};

}

#endif /* READER_H_ */
