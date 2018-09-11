#include "m3d/graphics/PointLight.h"
#include "m3d/action/RenderAction.h"
#include "sview/views/Parameters.h"
#include "m3d/SceneManager.h"
#include "m3d/base/BoundingBox.h"

namespace M3D
{

	PointLight::PointLight():
		BaseLight()

	{
		m_intensity = 1.0;
		m_lightColor = Color::WHITE;
		m_ShowAllSign = false;
		m_showSimpleSign = true;
		m_lightType = LightType_Point;

	}

	PointLight::~PointLight()
	{

	}

	void PointLight::FindVisiableObject(RenderAction* renderAction)
	{
		if (m_needUpdataSign)
		{
			Matrix3x4 worldMatrix = this->GetWorldTransform();
			m_glworldMatrix = worldMatrix.ToMatrix4().Transpose();
			Vector3 newPos = Vector3::ZERO;
			//仅显示图片标识
			if (this->m_showSimpleSign)
			{
				if (!this->m_simpleSignModel)
				{
					this->m_simpleSignModel = new ImageModel();
					AddRefMe(this->m_simpleSignModel);

					string directLightImage = SVIEW::Parameters::Instance()->m_appWorkPath + "\\data\\pic\\" + string("point.png");
					m_simpleSignModel->SetImagePath(directLightImage);
                    Vector2 tempSize(0.5f, 0.5f);
					m_simpleSignModel->SetImageSize(newPos, tempSize);
					this->AddSubModel(this->m_simpleSignModel);
				}
				//m_simpleSignModel->SetImagePosition(newPos);
			}

			//显示包括线框的标识
			if (this->m_ShowAllSign)
			{
				if (!this->m_allSignModel)
				{
					this->m_allSignModel = new Model();

					AddRefMe(this->m_allSignModel);
					this->AddSubModel(this->m_allSignModel);
				}
				m_allSignModel->ClearLineData();
				float length = renderAction->GetScene()->GetSceneBox().Length();

				
				//构造圆
				vector<Vector3> circleXY;
				vector<Vector3> circleXZ;
				float size = m_distance;

				for (int i = 0,l = 32; i < l; i++)
				{	
					float theta = i / (l*1.0)*M3D::M_PI * 2;
					Vector3 tempPoint(cos(theta), sin(theta), 0.0);
					circleXY.push_back(tempPoint*size);
					
					circleXZ.push_back(Vector3(cos(theta), 0.0, sin(theta))*size);
				}
				circleXY.push_back(Vector3(cos(0.0), sin(0.0), 0.0)*size);
				circleXZ.push_back(Vector3(cos(0.0), 0.0, sin(0.0))*size);
				m_allSignModel->AddLineData(circleXY);
				m_allSignModel->AddLineData(circleXZ);
			}

			m_needUpdataSign = false;
		}

		//if(this->IsVisible())
		{
			this->SetRenderWorldMatrix(&m_glworldMatrix);
			//renderAction->AddLight(this);
		}

		Model::FindVisiableObject(renderAction);
	}

}