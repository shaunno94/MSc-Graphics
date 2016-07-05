#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/TextMesh.h"
#include "PoliceBox.h"
#include "HeightMapNode.h"
#include "SkyBoxNode.h"
#include "LakeNode.h"
#include "EnvLight.h"
#include "ParticleEmitter.h"
#include "UI.h"

enum SCENE_NODES
{
	HEIGHTMAP, PB, SKYBOX, LAKE, ENV_LIGHT, TOTAL_NODES
};

enum SHADER_PROGS
{
	BASIC_SHADER, LIGHT_SHADER, COMBINE_SHADER, BLUR_SHADER, TOTAL_SHADERS
};
//Frame buffer objects.
enum FBO
{
	BUFFER_FBO, POINT_LIGHT_FBO, SHADOW_FBO, COMBINE_FBO, POST_FBO, TOTAL_FBO
};
//FBO attatchments
enum TEXTURES
{
	BUF_COLOUR_TEX, BUF_NRM_TEX, BUF_DEPTH_TEX, BUF_REFL_TEX, LIGHT_EMISSIVE_TEX, 
	LIGHT_SPECULAR_TEX, PB_SHADOW_TEX, IRRADIANCE_TEX, COMBINED_TEX, MOTION_BLUR_ONE, MOTION_BLUR_TWO, TOTAL_TEX
};

class Renderer : public OGLRenderer
{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);
	virtual void RenderScene(float msec);

	//inline void SetScale(float s) { scale = s; }
	//inline void SetRotation(float r) { rotation = r; }
	inline void SetPosition(Vector3& p) { static_cast<PoliceBox*>(sceneNodes[PB])->updatePos(p); }
	inline Vector3 getStartingPoint(){ return sceneNodes[PB]->GetTransform().GetPositionVector(); }
	inline void incDirLight() { 
		sceneLightRot += 2.0f;
		static_cast<SkyBoxNode*>(sceneNodes[SKYBOX])->incAmbient(); }
	inline void decDirLight() { 
		sceneLightRot -= 2.0f;
		static_cast<SkyBoxNode*>(sceneNodes[SKYBOX])->decAmbient(); }

	inline void switchViewPoint() { viewLight = !viewLight; }
	inline void toggleWireFrame() { wireframe = !wireframe; }
	inline void toggleBlur()	{ blur = !blur; }

protected:
	virtual void UpdateScene(float msec);
	void DrawPBShadows();
	void DrawShadowScene(); //Create shadow map.
	void DrawLights();
	void FillBuffers(); //First render pass, fills G-Buffer.
	void DrawPointLights(); //Lighting Render Pass.
	void DrawCombinedScene(); //Combination Render Pass.
	void DrawPostProcess(); //Do any post processing
	void DrawUI(const float size);

	void GenerateTexture(GLuint& target, bool depth = false, bool shadow = false, bool clamp = true);
	void GenerateShadowFBO(GLuint FBO, GLuint target);
	void initLights();

	Camera* camera;
	OBJMesh* lightVol; //Light Volume
	float sceneLightRot = 0.0f;
	Light* pb_light;
	Vector3 heightMap_center;
	ParticleEmitter* emitter;
	Vector2 pixSize;
	UI* HUD;

	Matrix4 lightVM;
	Matrix4 prevView;

	const unsigned int SHADOW_SIZE = 4096;
	const unsigned int LIGHTNUM = 8;

	float ambientLight;

	Mesh* screenQuad;
	vector<SceneNode*> sceneNodes;
	vector<Shader*> shaderProgs; //Collection of shader programs.
	vector<GLuint> fbo; //Handles to FBO objects.
	vector<GLuint> fbo_tex; //Handles to texture objects that the FBO's write into.
	vector<Light*> pointLights; //Array of lighting data.

	const char* LIGHT_VOLUME = "..\\Meshes\\ico.obj";
	
	const char* BASIC_VERT_SHADER = "..\\Shaders\\vertex_shader.glsl";
	const char* BASIC_FRAG_SHADER = "..\\Shaders\\fragment_shader.glsl";
	//Deffered lighting shaders
	const char* DFR_VERT_SHADER = "..\\Shaders\\DFR_vert_shader.glsl";
	const char* DFR_FRAG_SHADER = "..\\Shaders\\DFR_frag_shader.glsl";
	//Combine scene shaders
	const char* COMBINE_VERT_SHADER = "..\\Shaders\\combine_vert_shader.glsl";
	const char* COMBINE_FRAG_SHADER = "..\\Shaders\\combine_frag_shader.glsl";

	//Lens flare post process shaders
	const char* LF1_VERT_SHADER = "..\\Shaders\\LF1_vert_shader.glsl";
	const char* LF1_FRAG_SHADER = "..\\Shaders\\LF1_frag_shader.glsl";
	const char* LF2_FRAG_SHADER = "..\\Shaders\\LF2_frag_shader.glsl";

	const char* BLUR_VERT_SHADER = "..\\Shaders\\Blur_vert_shader.glsl";
	const char* BLUR_FRAG_SHADER = "..\\Shaders\\Blur_frag_shader.glsl";

	bool viewLight = false;
	bool wireframe = false;
	bool blur = false;
	unsigned int blurSamples = 5;

	static unsigned int bytes_used;
};