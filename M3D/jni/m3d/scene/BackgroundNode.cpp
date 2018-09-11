#include "m3d/M3D.h"
#include "m3d/model/Shape.h"
#include "m3d/scene/BackgroundNode.h"

#include "m3d/ResourceManager.h"

#include "m3d/model/Image.h"
#include "m3d/graphics/Texture.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/action/RenderAction.h"
namespace M3D
{

BackgroundNode::BackgroundNode():SceneNode()
{
	Initial();
}

BackgroundNode::~BackgroundNode(void)
{
    this->m_skyBoxTexture.clear();
}

void BackgroundNode::Initial()
{
	m_iViewX = 0;
	m_iViewY = 0;
	m_iWidth = 1600;
	m_iHeight = 1000;

	UpdateTopColor();
	UpdateBottomColor();

	m_ProArray[0] = -1;
	m_ProArray[1] = 1;
	m_ProArray[2] = -1;

	m_ProArray[3] = 1;
	m_ProArray[4] = -1;
	m_ProArray[5] = 1;

	m_fAspectRatio = 1;

	m_BackPnt[0] = -1;
	m_BackPnt[1] = 1;
	m_BackPnt[2] = 1;

	m_BackPnt[3] = -1;
	m_BackPnt[4] = -1;
	m_BackPnt[5] = 1;

	m_BackPnt[6] = 1;
	m_BackPnt[7] = -1;
	m_BackPnt[8] = 1;

	m_BackPnt[9] = -1;
	m_BackPnt[10] = 1;
	m_BackPnt[11] = 1;

	m_BackPnt[12] = 1;
	m_BackPnt[13] = -1;
	m_BackPnt[14] = 1;

	m_BackPnt[15] = 1;
	m_BackPnt[16] = 1;
	m_BackPnt[17] = 1;

	m_isUseImage = false;
	m_isUseSkyBox = false;
	m_isUseColor = true;
	m_needRestoreState = false;
	m_originTexture = nullptr;
	m_imagePath = "";
	m_origimagePath = "";
	m_texture = NULL;

	points.clear();
	textCoords.clear();
}

//void BackgroundNode::FinalRelease(void)
//{
//	Object::FinalRelease();
//}

void BackgroundNode::SetBackgroundSize(int width, int height)
{
	MutexLock lock(m_mutex);

	this->m_iWidth = width;
	this->m_iHeight = height;

//	size_ = Vector2(this->m_iWidth,this->m_iHeight);
}

void BackgroundNode::SetTopColor(const Color& topColor)
{
	MutexLock lock(m_mutex);
	if (this->m_topColor != topColor)
	{
		this->m_topColor = topColor;
		UpdateTopColor();
	}
}

void BackgroundNode::SetBottomColor(const Color& bottomColor)
{
	MutexLock lock(m_mutex);
	if (this->m_bottomColor != bottomColor)
	{
		this->m_bottomColor = bottomColor;
		UpdateBottomColor();
	}
}

void BackgroundNode::GetTopColor(Color& topColor)
{
	topColor.m_r = m_BackColor[0];
	topColor.m_g = m_BackColor[1];
	topColor.m_b = m_BackColor[2];
	topColor.m_a = m_BackColor[3];
	topColor.m_r = m_BackColor[12];
	topColor.m_g = m_BackColor[13];
	topColor.m_b = m_BackColor[14];
	topColor.m_a = m_BackColor[15];
	topColor.m_r = m_BackColor[20];
	topColor.m_g = m_BackColor[21];
	topColor.m_b = m_BackColor[22];
	topColor.m_a = m_BackColor[23];
}

void BackgroundNode::GetBottomColor(Color& bottomColor)
{
	bottomColor.m_r = m_BackColor[4];
	bottomColor.m_g = m_BackColor[5];
	bottomColor.m_b = m_BackColor[6];
	bottomColor.m_a = m_BackColor[7];
	bottomColor.m_r = m_BackColor[8];
	bottomColor.m_g = m_BackColor[9];
	bottomColor.m_b = m_BackColor[10];
	bottomColor.m_a = m_BackColor[11];
	bottomColor.m_r = m_BackColor[16];
	bottomColor.m_g = m_BackColor[17];
	bottomColor.m_b = m_BackColor[18];
	bottomColor.m_a = m_BackColor[19];
}

Vector3* BackgroundNode::GetVertexs()
{
	if (points.size() == 0)
	{
		MutexLock lock(m_mutex);
		Vector2 m_size_ = Vector2(2,2);
		Vector3 m_position_;
		float x = m_size_.m_x/2;
		float y = m_size_.m_y/2;

		//顺时针方向构建三角形
		// 左下角
		points.push_back(
				Vector3(m_position_.m_x - x, m_position_.m_y - y, m_position_.m_z));
		//右下角
		points.push_back(
				Vector3(m_position_.m_x + x, m_position_.m_y - y, m_position_.m_z));

		//左上角
		points.push_back(
				Vector3(m_position_.m_x - x, m_position_.m_y + y, m_position_.m_z));

		//左上角
		points.push_back(
				Vector3(m_position_.m_x - x, m_position_.m_y + y, m_position_.m_z));
		//右下角
		points.push_back(
				Vector3(m_position_.m_x + x, m_position_.m_y - y, m_position_.m_z));

		//右上角
		points.push_back(
				Vector3(m_position_.m_x + x, m_position_.m_y + y, m_position_.m_z));
	}
	return points.data();
}

Vector2* BackgroundNode::GetTextureCoords()
{
	if (textCoords.size() == 0)
	{
		MutexLock lock(m_mutex);
		Rect m_uv  = Rect(Vector2(0,0),Vector2(1,1)) ;
		Vector2 min = m_uv.m_min;
		Vector2 max = m_uv.m_max;
		//左下角
		textCoords.push_back(Vector2(min.m_x, min.m_y));
		//右下角
		textCoords.push_back(Vector2(max.m_x, min.m_y));
		//左上角
		textCoords.push_back(Vector2(min.m_x, max.m_y));
		//左上角
		textCoords.push_back(Vector2(min.m_x, max.m_y));
		//右下角
		textCoords.push_back(Vector2(max.m_x, min.m_y));
		//右上角
		textCoords.push_back(Vector2(max.m_x, max.m_y));
	}

	return textCoords.data();
}

void BackgroundNode::UpdateTopColor(void)
{
	m_BackColor[0] = this->m_topColor.m_r;
	m_BackColor[1] = this->m_topColor.m_g;
	m_BackColor[2] = this->m_topColor.m_b;
	m_BackColor[3] = this->m_topColor.m_a;

	m_BackColor[12] = this->m_topColor.m_r;
	m_BackColor[13] = this->m_topColor.m_g;
	m_BackColor[14] = this->m_topColor.m_b;
	m_BackColor[15] = this->m_topColor.m_a;

	m_BackColor[20] = this->m_topColor.m_r;
	m_BackColor[21] = this->m_topColor.m_g;
	m_BackColor[22] = this->m_topColor.m_b;
	m_BackColor[23] = this->m_topColor.m_a;
}

void BackgroundNode::UpdateBottomColor(void)
{
	m_BackColor[4] = this->m_bottomColor.m_r;
	m_BackColor[5] = this->m_bottomColor.m_g;
	m_BackColor[6] = this->m_bottomColor.m_b;
	m_BackColor[7] = this->m_bottomColor.m_a;

	m_BackColor[8] = this->m_bottomColor.m_r;
	m_BackColor[9] = this->m_bottomColor.m_g;
	m_BackColor[10] = this->m_bottomColor.m_b;
	m_BackColor[11] = this->m_bottomColor.m_a;

	m_BackColor[16] = this->m_bottomColor.m_r;
	m_BackColor[17] = this->m_bottomColor.m_g;
	m_BackColor[18] = this->m_bottomColor.m_b;
	m_BackColor[19] = this->m_bottomColor.m_a;
}

int BackgroundNode::GetType(void)
{
	return BACKGROUNDCOLOR_NODE;
}

void BackgroundNode::FindVisiableObject(RenderAction* renderAction)
{
	if (!this->IsVisible())
	{
		renderAction->SetBackGroundNode(NULL);
	}
	else
	{
		renderAction->SetBackGroundNode(this);
	}
}

void BackgroundNode::SetImage(const string& filePath,int mappingStyle)
{
	MutexLock lock(m_mutex);
	//先从资源管理器中删除原有的图片资源
	if(filePath.length() > 0)
	{
		m_imagePath = filePath;
//		LOGI("backgroundImage %s", m_imagePath.c_str());
		
		this->m_isImageDirty = true;
	}
}

void BackgroundNode::SetUseImage(bool useImage)
{
	MutexLock lock(m_mutex);

	this->m_isUseImage = useImage;
	if (useImage)
	{
		m_isUseColor = false;
		m_isUseSkyBox = false;
	}
//	LOGI("BackgroundNode::SetUseImage %d", this->m_isUseImage);
}

bool BackgroundNode::IsUseImage()
{
	return m_isUseImage;
}

void BackgroundNode::SetUseSkyBox(bool useSkyBox)
{
	MutexLock lock(m_mutex);
	m_isUseSkyBox = useSkyBox;
	if (useSkyBox)
	{
		m_isUseImage = false;
		m_isUseColor = false;
	}
}

bool BackgroundNode::IsUseSkyBox()
{
	return m_isUseSkyBox;
}

Texture* BackgroundNode::GetTexture()
{
	 if(this->m_isImageDirty || this->m_texture == NULL)
	 {
		 MutexLock lock(m_mutex);

		 if(this->m_texture)
		 {
			 delete this->m_texture;
			 this->m_texture = NULL;
		 }

		Rect m_uv  = Rect(Vector2(0,0),Vector2(1,1)) ;

		this->m_texture = new Texture2D(m_imagePath,TEXTURE_LOAD_RGBA,TEXTURE_FLAG_MIPMAPS|TEXTURE_FLAG_INVERT_Y);
         m_texture->SetResourceManager(this->m_resMgr);
		m_originTexture = m_texture;
		m_originTexture->AddRef();
		LOGI("BackgroundNode create Texture3D");
		this->m_isImageDirty = false;
	 }

	 return this->m_texture;
}

void BackgroundNode::SetTexture(Texture * texture)
{
	if (texture && this->m_texture != texture)
	{
		MutexLock lock(m_mutex);

		if (this->m_texture)
		{
//			this->m_texture->Release();
		//	this->m_texture = NULL;
		}

		this->m_texture = texture;
		this->m_texture->AddRef();
		this->m_isImageDirty = false;
	}
}

Texture * BackgroundNode::GetSkyBoxTexture(string name)
{
	Texture * ret = nullptr;
	map<string,Texture*>::iterator it =  m_skyBoxTexture.find(name);
	if (it != m_skyBoxTexture.end())
	{
		ret = it->second;
	}
	return nullptr;
}

Texture * BackgroundNode::GetSkyBoxTexture()
{
	Texture * ret = nullptr;
	if (this->m_skyBoxTexture.size()>0)
	{
		map<string, Texture*>::iterator it = this->m_skyBoxTexture.begin();
		ret = it->second;
	}
	
	return ret;
}

void BackgroundNode::AddSkyBoxTexture(string name,Texture* skyBox)
{
	this->m_skyBoxTexture[name]= skyBox;
}
void BackgroundNode::ClearSkyBoxTexture()
{
    if (this->m_skyBoxTexture.size()>0)
    {
       this->m_skyBoxTexture.clear();
    }
}
bool BackgroundNode::IsUseColor()
{
	return m_isUseColor;
}

void BackgroundNode::SetUseColor(bool useColor)
{
	MutexLock lock(m_mutex);
	m_isUseColor = useColor;
	if (useColor)
	{
		m_isUseImage = false;
		m_isUseSkyBox = false;
	}
}

void BackgroundNode::KeepBackgroundState()
{
	MutexLock lock(m_mutex);
	m_originTopColor = this->m_topColor;
	m_originBottomColor = this->m_bottomColor;
	m_originUseColor = m_isUseColor;
	m_originUseImage = m_isUseImage;
	m_originUseSkyBox = m_isUseSkyBox;
	m_origimagePath = m_imagePath;
	m_needRestoreState = true;
}

void BackgroundNode::RestoreBackgroundState()
{
	MutexLock lock(m_mutex);
	if (!m_needRestoreState)
	{
		return;
	}
	SetTopColor(m_originTopColor);
	SetBottomColor(m_originBottomColor);
	m_texture = m_originTexture;
	m_isUseColor = m_originUseColor;
	m_isUseImage = m_originUseImage;
	m_isUseSkyBox = m_originUseSkyBox;
	m_imagePath = m_origimagePath;
	m_needRestoreState = false;
}
void BackgroundNode::SetResourceManager(ResourceManager * resMgr)
{this->m_resMgr = resMgr;}
}
