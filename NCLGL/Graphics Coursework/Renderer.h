/*
Author: Shaun Heald
This class extends OGLRenderer, this class provides functionality to update and draw an arbitrary scene.
This class holds frame buffer objects, textures that are used by the FBO's and shader programs required by this class.
Rendering pipeline ==> Fill shadow map, fill G-buffer, compute lighting, combine the previous stages, compute any post-processing effects and present the final image.
*/

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
	//Updates and renders the currently active scene.
	virtual void RenderScene(float msec);

	/* Public utility functions which allows some state changes to the renderer instance. */
	inline void ToggleWireFrame() { wireframe = !wireframe; }
	inline void ToggleBlur()	{ blur = !blur; }
	inline void SetCurrentScene(Scene* scene) { currentScene = scene; }

protected:
	virtual void UpdateScene(float msec);
	//Draw objects into the shadow map (a depth buffer).
	void DrawShadowScene(); 
	//Setup the rendering pipeline to draw any point or directional lights (fills emissive and specular textures).
	void DrawLights();
	//First render pass, fills the G-Buffer (diffuse, normal and depth texture).
	void FillBuffers();
	//Draw any point lights in the current scene.
	void DrawPointLights(); 
	//Renders a full screen quad which combines previously rendered textures.
	void DrawCombinedScene(); 
	//Do any post processing and present the final image to screen (renders full screen quad).
	void DrawPostProcess(); 

	/* Utility functions which sets up the classes state. */
	void GenerateTexture(GLuint& target, bool depth = false, bool shadow = false, bool clamp = true);
	void GenerateShadowFBO(GLuint FBO, GLuint target);
	void InitShaderUniformLocations();

	//A pointer to the scene which will be drawn.
	Scene* currentScene = nullptr;

	//Size of the shadow map (4k * 4k).
	const unsigned int SHADOW_SIZE = 4096;

	//Full screen quad used by the combine & post-processing draw functions.
	Mesh* screenQuad;
	//Collection of shader programs used by the renderer class.
	vector<Shader*> shaderProgs; 
	//Collection of handles to FBO objects.
	vector<GLuint> fbo; 
	//Collection of handles to texture objects that the FBO's write/read.
	vector<GLuint> fbo_tex; 

	bool wireframe = false;
	bool blur = false;
	//How many passes the post-processing blur effect will do.
	const unsigned int blurSamples = 4;

	/* Shader Uniform Handles. */
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