/**@file
 *@brief	Selector类头文件
 *@author
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *提供Selector类的声明
 *
 */


#ifndef M3D_SELECTOR_H_
#define M3D_SELECTOR_H_

#include "sview/SView.h"
#include "m3d/model/Shape.h"

namespace M3D {
    class SceneManager;
}

using namespace M3D;

namespace SVIEW {

/**@class Selector
 * @brief Selector类   提供Selector类的声明
 *
 * Selector类
 *
 */
class M3D_API Selector
{
public:
	Selector(SceneManager* scene);
	virtual ~Selector();


	/**
	 * 设置图形
	 * @param shape
	 */
	void Set(IShape* shape);


	/**
	 * 增加图形
	 * @param shape
	 */
	void Add(IShape* shape);


	/**
	 * 删除图形
	 * @param shape
	 */
	void Remove(IShape* shape);


	/**
	 * @brief 清除
	 */
	void Clear();


	/**
	 * @brief 	获取
	 * @return
	 */
	IShape* Get();


	/**
	 *  获取所有
	 * @return
	 */
	vector<IShape*> GetAll();


	/**
	 * 	数量
	 * @return
	 */
	int Count();

    /**
     *  是否存在
     * @param shape
     * @return
     */
    bool Exist(IShape* shape);
    
private:
	vector<IShape*> m_selectList; //!<选择列表
	SceneManager* m_scene;
};

}




#endif /*M3D_SELECTOR_H_*/
