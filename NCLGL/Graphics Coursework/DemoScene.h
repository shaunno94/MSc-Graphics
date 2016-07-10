/*
Author: Shaun Heald
This class extends the base scene class and holds all of the elements
required to create a simple 'demo' scene to show various rendering techniques.
*/
#pragma once
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

	//Switch between the camera and light view point (was mainly for debugging)
	inline void SwitchViewPoint() { viewLight = !viewLight; }
	inline void toggleBlur() { blur = !blur; }

	void IncrementDirLight(float dt);
	void DecrementDirLight(float dt);

private:

	void InitialiseLights();

	const unsigned int LIGHTNUM = 8;

	Light* pb_light;
	//Point light volume (simple ico sphere)
	OBJMesh* lightVol; 
	Vector3 heightMap_center;
	float sceneLightRot = 0.0f;

	Matrix4 lightVM;
	Matrix4 shadowPersp;
	bool viewLight = false;

	UI* HUD;
	ParticleEmitter* emitter;
	bool blur = false;

	//Indices of the various scene objects used by this scene
	unsigned int heightmap_index;
	unsigned int policeBox_index;
	unsigned int envLight_index;
	unsigned int skybox_index;

	//Indices of the various shaders stored by the scene
	unsigned int heightmap_shader_index;
	unsigned int policeBox_shader_index;
	unsigned int envLight_shader_index;
	unsigned int skybox_shader_index;
	unsigned int lake_shader_index;
	unsigned int UI_shader_index;
	unsigned int particle_shader_index;
};