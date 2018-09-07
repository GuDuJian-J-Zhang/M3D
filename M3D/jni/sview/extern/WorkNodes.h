/**
 *@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */

#ifndef    SELECTEDNODES_H_
#define    SELECTEDNODES_H_

#include "m3d/M3D.h"
#include "m3d/scene/SceneNode.h"

namespace M3D
{
/**
 * @brief  TODO
 * 用于缓存当前需要操作的节点，ASView中有摄像机，坐标轴，和模型顶级实例
 * @class
 */
class M3D_API WorkNodes
{
public:
	void AddNode(const string& name, SceneNode* node);
	void RemoveNode(const string& name);

	int Size();

	void SetPosition(const Vector3& mov);
	void SetZoom(const float scale);

	void GetMVMatrix(ControlInfo& mvMatrix, ControlInfo& oldmvMatrix);
	void SetRotationAroundCenter(const Quaternion& rotation);
	void SetRotation(const Quaternion& rotation);
	void GetRotaton(Quaternion& rotation);
	void RotateAroundCenter(const Quaternion& rotation);
	void Rotate(const Quaternion& rotation);
	void Translate(const Vector3& vector, const Vector3& curPos);
	void Zoom(float scale);

private:
	map<string, SceneNode*> m_workNodesMap;

};

}

#endif // SELECTEDNODES_H_
