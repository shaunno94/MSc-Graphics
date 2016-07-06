#include <nclgl\Scene.h>

class DemoScene : public Scene
{
public:
	DemoScene();
	virtual ~DemoScene();

	virtual void UpdateScene(float msec) override;
	virtual void DrawScene(bool shadowPass) override;
	virtual void LateDraw() override;

	inline void SetPosition(Vector3& p) { static_cast<PoliceBox*>(sceneObjects[policeBox_index])->updatePos(p); }
	inline Vector3 getStartingPoint() { return sceneObjects[policeBox_index]->GetTransform().GetPositionVector(); }

	inline void incDirLight() {
		sceneLightRot += 2.0f;
		static_cast<SkyBoxNode*>(sceneObjects[skybox_index])->incAmbient();
	}

	inline void decDirLight() {
		sceneLightRot -= 2.0f;
		static_cast<SkyBoxNode*>(sceneObjects[skybox_index])->decAmbient();
	}

protected:

private:

	void InitialiseLights();

	const unsigned int LIGHTNUM = 8;

	Light* pb_light;
	//Light Volume
	OBJMesh* lightVol; 
	Vector3 heightMap_center;
	float sceneLightRot = 0.0f;

	unsigned int heightmap_index;
	unsigned int policeBox_index;
	unsigned int envLight_index;
	unsigned int skybox_index;

	unsigned int heightmap_shader_index;
	unsigned int policeBox_shader_index;
	unsigned int envLight_shader_index;
	unsigned int skybox_shader_index;
	unsigned int lake_shader_index;
};