#include <nclgl\Scene.h>
#include "PoliceBox.h"
#include "SkyBoxNode.h"
#include "ParticleEmitter.h"
#include "UI.h"

class DemoScene : public Scene
{
public:
	DemoScene();
	virtual ~DemoScene();

	virtual void UpdateScene(float msec) override;
	virtual void DrawScene(bool shadowPass, bool lightPass = false) override;
	virtual void LateDraw() override;

	inline void SetPosition(Vector3& p) { static_cast<PoliceBox*>(sceneObjects[policeBox_index])->updatePos(p); }
	inline Vector3 getStartingPoint() { return sceneObjects[policeBox_index]->GetTransform().GetPositionVector(); }
	inline void SwitchViewPoint() { viewLight = !viewLight; }
	inline void toggleBlur() { blur = !blur; }

	inline void incDirLight(float dt) {
		sceneLightRot += 2.0f;
		static_cast<SkyBoxNode*>(sceneObjects[skybox_index])->incAmbient();
	}

	inline void decDirLight(float dt) {
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

	Matrix4 lightVM;
	Matrix4 shadowPersp;
	bool viewLight = false;

	UI* HUD;
	ParticleEmitter* emitter;
	bool blur = false;

	unsigned int heightmap_index;
	unsigned int policeBox_index;
	unsigned int envLight_index;
	unsigned int skybox_index;

	unsigned int heightmap_shader_index;
	unsigned int policeBox_shader_index;
	unsigned int envLight_shader_index;
	unsigned int skybox_shader_index;
	unsigned int lake_shader_index;
	unsigned int UI_shader_index;
	unsigned int particle_shader_index;
};