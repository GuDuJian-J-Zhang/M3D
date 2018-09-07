#ifndef M3D_LIGHT_MANAGER_H_
#define M3D_LIGHT_MANAGER_H_
#include "m3d/M3D.h"
#include "m3d/graphics/BaseLight.h"
namespace M3D
{
	class CameraNode;
	struct M3D_API LightUniforms
	{

	};
	/************************************************************************/
	/* 各种类型的灯光参数缓存结构                                                                     */
	/************************************************************************/
	struct DirectionalLightUniforms :public LightUniforms
	{
		DirectionalLightUniforms()
		{
			shadow = false;
			shadowBias = 0.0f;
			shadowRadius = 1.0;

		}
		Vector3 direction;
		Color color;

		int shadow ;
		float shadowBias;
		float shadowRadius;
		Vector2 shadowMapSize;
	};

	struct M3D_API PointLightUniforms :public LightUniforms
	{
		PointLightUniforms()
		{
			distance = 0;
			decay = 0;

			shadow = false;
			shadowBias = 0.0f;
			shadowRadius = 1.0;
			shadowCameraFar = 1;
			shadowCameraNear = 1000;
		}

		Vector3 position;		
		Color color;
		float distance;
		float decay;

		int shadow;
		float shadowBias;
		float shadowRadius;
		Vector2 shadowMapSize;
		float shadowCameraNear;
		float shadowCameraFar;
	};

	struct M3D_API SpotLightUniforms :public LightUniforms
	{
		SpotLightUniforms() 
		{
			distance = 0;
			coneCos = 0;
			penumbraCos = 0;
			decay = 0;

			shadow = false;
			shadowBias = 0.0f;
			shadowRadius = 1.0;
		}

		Vector3 position;
		Vector3 direction;
		Color color;
		float distance;
		float coneCos;
		float penumbraCos;
		float decay;

		int shadow;
		float shadowBias;
		float shadowRadius;
		Vector2 shadowMapSize;

	};

	struct M3D_API HemisphereLightUniforms :public LightUniforms
	{
		HemisphereLightUniforms()
		{
			direction = Vector3(0, 1, 0);
			skyColor = Color(1, 1, 1);
			groundColor = Color(1, 1, 1);
		}

		
		Vector3 direction;
		Color skyColor;
		Color groundColor;


	};

	struct M3D_API SceneLightState
	{
		string hash;
		vector<LightUniforms*> directional;
		vector<LightUniforms*> spot;
		vector<LightUniforms*> point;
		vector<LightUniforms*> hemisphere;
		Vector3 ambient;
		void Resize()
		{
			directional.clear();
			spot.clear();
			point.clear();
			hemisphere.clear();
			ambient.m_x = 0;
			ambient.m_y = 0;
			ambient.m_z = 0;
		}
		~SceneLightState()
		{

		}

	};
	/************************************************************************/
	/* 灯光管理类。用来管理灯管增删，更新灯光数据等                                                                     */
	/************************************************************************/
	class M3D_API LightManager
	{
	public:
		LightManager();
		~LightManager();
		//************************************
		// Method:    AddLight
		// FullName:  M3D::LightManager::AddLight
		// Access:    public 
		// Returns:   void
		// Qualifier: 添加指定灯光
		// Parameter: BaseLight * light
		//************************************
		void AddLight(BaseLight* light);
		//************************************
		// Method:    RemoveLight
		// FullName:  M3D::LightManager::RemoveLight
		// Access:    public 
		// Returns:   void
		// Qualifier: 移除指定灯光
		// Parameter: BaseLight * light
		//************************************
		void RemoveLight(BaseLight* light);
		//bool AddLightToScene(BaseLight*light);
		void SetSceneManager(SceneManager* sceneManager) { this->m_sceneManager = sceneManager; }
		SceneManager*GetSceneManager() { return this->m_sceneManager; }
		//************************************
		// Method:    SetUp
		// FullName:  M3D::LightManager::SetUp
		// Access:    public 
		// Returns:   void
		// Qualifier: 更新灯光数据
		// Parameter: CameraNode * camera
		//************************************
		void SetUp(CameraNode*camera);
		
		//************************************
		// Method:    DirectionalLightNumber
		// FullName:  M3D::LightManager::DirectionalLightNumber
		// Access:    public 
		// Returns:   int
		// Qualifier: 平行光个数
		//************************************
		int DirectionalLightNumber() const { return m_directionalLightNumber; }
		void DirectionalLightNumber(int val) { m_directionalLightNumber = val; }
		int PointLightNumber() const { return m_pointLightNumber; }
		//************************************
		// Method:    PointLightNumber
		// FullName:  M3D::LightManager::PointLightNumber
		// Access:    public 
		// Returns:   void
		// Qualifier: 点光源个数
		// Parameter: int val
		//************************************
		void PointLightNumber(int val) { m_pointLightNumber = val; }
		//************************************
		// Method:    SpotLightNumber
		// FullName:  M3D::LightManager::SpotLightNumber
		// Access:    public 
		// Returns:   int
		// Qualifier: 聚光灯个数
		//************************************
		int SpotLightNumber() const { return m_spotLightNumber; }
		void SpotLightNumber(int val) { m_spotLightNumber = val; }
		//************************************
		// Method:    GetState
		// FullName:  M3D::LightManager::GetState
		// Access:    public 
		// Returns:   SceneLightState&
		// Qualifier: 获取灯光状态
		//************************************
		SceneLightState& GetState();
		//************************************
		// Method:    GetAllLight
		// FullName:  M3D::LightManager::GetAllLight
		// Access:    public 
		// Returns:   vector<BaseLight*>&
		// Qualifier: 获取所有灯光
		//************************************
		vector<BaseLight*>& GetAllLight()  { return m_sceneLights; }
		//************************************
		// Method:    HemisphereLightNumber
		// FullName:  M3D::LightManager::HemisphereLightNumber
		// Access:    public 
		// Returns:   int
		// Qualifier: 半球光数量
		//************************************
		int HemisphereLightNumber() const { return m_hemisphereLightNumber; }
		void HemisphereLightNumber(int val) { m_hemisphereLightNumber = val; }
		void Clear();
		//void SceneLights(vector<BaseLight*> val) { m_sceneLights = val; }
	private:
		//************************************
		// Method:    GetLightUniform
		// FullName:  M3D::LightManager::GetLightUniform
		// Access:    private 
		// Returns:   LightUniforms*
		// Qualifier: 获取灯光的uniform变量
		// Parameter: BaseLight * light
		//************************************
		LightUniforms* GetLightUniform(BaseLight* light);
		//************************************
		// Method:    RemoveLightUniform
		// FullName:  M3D::LightManager::RemoveLightUniform
		// Access:    private 
		// Returns:   void
		// Qualifier: 移除灯光的uniform
		// Parameter: BaseLight * light
		//************************************
		void RemoveLightUniform(BaseLight* light);
	private:
		vector<BaseLight*> m_sceneLights;
		SceneManager* m_sceneManager;
		int m_directionalLightNumber;
		int m_pointLightNumber;
		int m_spotLightNumber;
		int m_hemisphereLightNumber;
		map<int, LightUniforms*> m_lightUniformCatch;
		SceneLightState m_state;

	};
}
#endif //M3D_LIGHT_MANAGER_H_


