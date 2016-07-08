#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/TextMesh.h"
#include <nclgl\Scene.h>

//Shader program enums.
enum SHADER_PROGS
{
	LIGHT_SHADER, COMBINE_SHADER, BLUR_SHADER, TOTAL_SHADERS
};

//Frame buffer enums.
enum FBO
{
	BUFFER_FBO, POINT_LIGHT_FBO, SHADOW_FBO, COMBINE_FBO, POST_FBO, TOTAL_FBO
};

//FBO attatchment enums.
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

	inline void toggleWireFrame() { wireframe = !wireframe; }
	inline void toggleBlur()	{ blur = !blur; }
	inline void SetCurrentScene(Scene* scene) { currentScene = scene; }

protected:
	virtual void UpdateScene(float msec);
	void DrawShadowScene(); //Create shadow map.
	void DrawLights();
	void FillBuffers(); //First render pass, fills G-Buffer.
	void DrawPointLights(); //Lighting Render Pass.
	void DrawCombinedScene(); //Combination Render Pass.
	void DrawPostProcess(); //Do any post processing.

	void GenerateTexture(GLuint& target, bool depth = false, bool shadow = false, bool clamp = true);
	void GenerateShadowFBO(GLuint FBO, GLuint target);
	void InitShaderUniformLocations();

	Scene* currentScene = nullptr;

	const unsigned int SHADOW_SIZE = 4096;
	const unsigned int LIGHTNUM = 8;

	Mesh* screenQuad;
	vector<Shader*> shaderProgs; //Collection of shader programs.
	vector<GLuint> fbo; //Handles to FBO objects.
	vector<GLuint> fbo_tex; //Handles to texture objects that the FBO's write into.

	bool wireframe = false;
	bool blur = false;
	unsigned int blurSamples = 5;

	unsigned int light_depthTex_loc;
	unsigned int light_normalTex_loc;
	unsigned int light_shadowTex_loc;
	unsigned int light_irradianceTex_loc;
	unsigned int light_camera_loc;
	unsigned int light_pixelSize_loc;
	unsigned int light_invProjView_loc;
	unsigned int light_directional_loc;

	unsigned int combine_projMatrix_loc;
	unsigned int combine_diffuseTex_loc;
	unsigned int combine_emissiveTex_loc;
	unsigned int combine_specularTex_loc;
	unsigned int combine_irradianceTex_loc;
	unsigned int combine_reflTex_loc;
	unsigned int combine_finalRender_loc;
	unsigned int combine_blurTex_loc;

	unsigned int blur_projMatrix_loc;
	unsigned int blur_pixelSize_loc;
	unsigned int blur_isVertical_loc;
	unsigned int blur_diffuseTex_loc;
};