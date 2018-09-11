#include "m3d/graphics/SpotLight.h"
#include "m3d/action/RenderAction.h"
#include "sview/views/Parameters.h"
#include "m3d/SceneManager.h"
#include "m3d/base/BoundingBox.h"
namespace M3D
{

	SpotLight::SpotLight() :
		BaseLight()

	{
		m_intensity = 1.0;
		m_lightColor = Color::WHITE;
		m_ShowAllSign = false;
		m_showSimpleSign = true;
		m_lightType = LightType_Spot;

	}

	SpotLight::~SpotLight()
	{

	}

	void SpotLight::FindVisiableObject(RenderAction* renderAction)
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

					string directLightImage = SVIEW::Parameters::Instance()->m_appWorkPath + "\\data\\pic\\" + string("spot.png");
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
				float length = m_distance;
				float angle = m_angle;
				float radius = M3D::Tan(angle)*length;
				//构造方向线
				vector<Vector3> lines;

				//Vector3 target = GetTargetPosition();
				Vector3 position = newPos;
				Vector3 direction = m_direction;
				direction.Normalize();			
				lines.push_back(position - direction*length);
				lines.push_back(position);
				m_allSignModel->AddLineData(lines);
				//构造四条线
				vector<Vector3> line1;
				line1.push_back(Vector3(0.0,0.0,0.0));
				line1.push_back(Vector3(cos(0.0)*radius, sin(0.0)*radius, -length));
				m_allSignModel->AddLineData(line1);
				vector<Vector3> line2;
				line2.push_back(Vector3(0.0, 0.0, 0.0));
				line2.push_back(Vector3(cos(M3D::M_HALF_PI)*radius, sin(M3D::M_HALF_PI)*radius,-length));
				m_allSignModel->AddLineData(line2);
				vector<Vector3> line3;
				line3.push_back(Vector3(0.0, 0.0, 0.0));
				line3.push_back(Vector3(cos(M3D::M_PI)*radius, sin(M3D::M_PI)*radius, -length));
				m_allSignModel->AddLineData(line3);
				vector<Vector3> line4;
				line4.push_back(Vector3(0.0, 0.0, 0.0));
				line4.push_back(Vector3(cos(M3D::M_HALF_PI*3)*radius, sin(M3D::M_HALF_PI*3)*radius, -length));
				m_allSignModel->AddLineData(line4);

				//构造圆
				vector<Vector3> circleXY;				
				

				for (int i = 0, l = 32; i < l; i++)
				{
					float theta = i / (l*1.0)*M3D::M_PI * 2;
					
					circleXY.push_back(Vector3(cos(theta)*radius, sin(theta)*radius, -length));
				}
				circleXY.push_back(Vector3(cos(0.0)*radius, sin(0.0)*radius, -length));
			
				m_allSignModel->AddLineData(circleXY);
			
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