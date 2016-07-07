#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)
{
	shaderProgs.resize(TOTAL_SHADERS);
	fbo.resize(TOTAL_FBO);
	fbo_tex.resize(TOTAL_TEX);

	shaderProgs[BASIC_SHADER] = new Shader(File_Locs::SHADER_DIR + "vertex_shader.glsl", File_Locs::SHADER_DIR + "fragment_shader.glsl");
	//Deffered lighting shaders
	shaderProgs[LIGHT_SHADER] = new Shader(File_Locs::SHADER_DIR + "DFR_vert_shader.glsl", File_Locs::SHADER_DIR + "DFR_frag_shader.glsl");
	//Combine scene shaders
	shaderProgs[COMBINE_SHADER] = new Shader(File_Locs::SHADER_DIR + "combine_vert_shader.glsl", File_Locs::SHADER_DIR + "combine_frag_shader.glsl");
	//Post-process gaussian blur shader
	shaderProgs[BLUR_SHADER] = new Shader(File_Locs::SHADER_DIR + "Blur_vert_shader.glsl", File_Locs::SHADER_DIR + "Blur_frag_shader.glsl");
	SceneNode::context = this;

	for (unsigned int i = 0; i < TOTAL_SHADERS; ++i)
	{
		if (!shaderProgs[i]->LinkProgram())
		{
			cout << "Initialisation failed...A shader program (" << shaderProgs[i]->GetProgramName() << ") failed to compile." << endl;
			system("pause");
			exit(1);
		}
	}
	
	screenQuad = Mesh::GenerateQuad();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearDepth(1.0);
	glClearColor(0, 0, 0, 1);
	glPolygonOffset(1.0, 4096.0);
	SwitchToPerspective();

	glGenFramebuffers(TOTAL_FBO, fbo.data());
	GLenum buffers[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };

	GenerateTexture(fbo_tex[BUF_DEPTH_TEX], true);
	GenerateTexture(fbo_tex[BUF_REFL_TEX]);
	GenerateTexture(fbo_tex[BUF_COLOUR_TEX]);
	GenerateTexture(fbo_tex[BUF_NRM_TEX]);
	GenerateTexture(fbo_tex[IRRADIANCE_TEX]);
	GenerateTexture(fbo_tex[LIGHT_EMISSIVE_TEX]);
	GenerateTexture(fbo_tex[LIGHT_SPECULAR_TEX]);
	GenerateTexture(fbo_tex[PB_SHADOW_TEX], false, true);
	GenerateTexture(fbo_tex[COMBINED_TEX]);
	GenerateTexture(fbo_tex[MOTION_BLUR_ONE]);
	GenerateTexture(fbo_tex[MOTION_BLUR_TWO]);

	GenerateShadowFBO(fbo[SHADOW_FBO], fbo_tex[PB_SHADOW_TEX]);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[BUFFER_FBO]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_tex[BUF_COLOUR_TEX], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fbo_tex[BUF_NRM_TEX], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, fbo_tex[IRRADIANCE_TEX], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, fbo_tex[BUF_REFL_TEX], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo_tex[BUF_DEPTH_TEX], 0);
	glDrawBuffers(4, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Initialisation failed...G-FBO was not created." << endl;
		system("pause");
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo[POINT_LIGHT_FBO]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_tex[LIGHT_EMISSIVE_TEX], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fbo_tex[LIGHT_SPECULAR_TEX], 0);
	glDrawBuffers(3, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Initialisation failed...Lighting FBO was not created." << endl;
		system("pause");
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo[COMBINE_FBO]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_tex[COMBINED_TEX], 0);
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Initialisation failed...Lighting FBO was not created." << endl;
		system("pause");
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo[POST_FBO]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_tex[MOTION_BLUR_ONE], 0);
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Initialisation failed...Post FBO was not created." << endl;
		system("pause");
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	init = true;
}
Renderer::~Renderer()
{
	delete screenQuad;
	screenQuad = nullptr; 	

	for (unsigned int i = 0; i < TOTAL_SHADERS; ++i)
	{
		delete shaderProgs[i];
		shaderProgs[i] = nullptr;
	}

	shaderProgs.clear();
	currentShader = nullptr;
	glDeleteFramebuffers(TOTAL_FBO, fbo.data());
	glDeleteTextures(TOTAL_TEX, fbo_tex.data());
}

void Renderer::GenerateTexture(GLuint& target, bool depth, bool shadow, bool clamp)
{
	glGenTextures(1, &target);
	if (shadow)
	{
		glBindTexture(GL_TEXTURE_2D, target);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
		Renderer::bytes_used += sizeof(float)* (SHADOW_SIZE * SHADOW_SIZE);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, target);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, depth ? GL_DEPTH_COMPONENT32 : GL_RGBA32F, width, height, 0,
				depth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_FLOAT, NULL);
		depth ? Renderer::bytes_used += sizeof(float) * (width * height) : Renderer::bytes_used += (sizeof(float) * 4) * (width * height);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::GenerateShadowFBO(GLuint FBO, GLuint target)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, target, 0);
	glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Initialisation failed...Shadow FBO was not created." << endl;
		system("pause");
		exit(1);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::RenderScene(float msec)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	if (currentScene)
	{
		UpdateScene(msec);	

		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		FillBuffers();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		DrawLights();
		DrawCombinedScene();
		DrawPostProcess();
	}		

	glUseProgram(0);
	SwapBuffers();
}

void Renderer::UpdateScene(float msec)
{
	currentScene->UpdateScene(msec);
}

void Renderer::FillBuffers()
{
	DrawShadowScene();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[BUFFER_FBO]);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	currentScene->DrawScene(false);

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawLights()
{
	SetCurrentShader(shaderProgs[LIGHT_SHADER]);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[POINT_LIGHT_FBO]);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_ONE, GL_ONE);

	glUniform1i(glGetUniformLocation(shaderProgs[LIGHT_SHADER]->GetProgram(), "depthTex"), 5);
	glUniform1i(glGetUniformLocation(shaderProgs[LIGHT_SHADER]->GetProgram(), "normTex"), 6);
	glUniform1i(glGetUniformLocation(shaderProgs[LIGHT_SHADER]->GetProgram(), "shadowTex"), 7);
	glUniform3fv(glGetUniformLocation(shaderProgs[LIGHT_SHADER]->GetProgram(), "cameraPos"), 1, (float*)&currentScene->GetSceneCamera()->GetPosition());
	glUniform2f(glGetUniformLocation(shaderProgs[LIGHT_SHADER]->GetProgram(), "pixelSize"), pixelPitch.x, pixelPitch.y);
	glUniform1i(glGetUniformLocation(shaderProgs[LIGHT_SHADER]->GetProgram(), "irradianceTex"), 13);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgs[LIGHT_SHADER]->GetProgram(), "invPV"), 1, false, (float*)&Matrix4::Inverse(projMatrix * viewMatrix));

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, fbo_tex[BUF_DEPTH_TEX]);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, fbo_tex[BUF_NRM_TEX]);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, fbo_tex[PB_SHADOW_TEX]);
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, fbo_tex[IRRADIANCE_TEX]);
	
	DrawPointLights();
	currentScene->DrawScene(false, true);
		
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void Renderer::DrawPointLights()
{
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "dir"), 0);
	for (unsigned int i = 0; i < currentScene->GetNumberOfPointLights(); ++i)
	{
		Light* l = currentScene->GetPointLight(i);
		float radius = l->getRadius();
		Vector3 offset = l->getOffset();
		if (!l->getIsStatic())
		{
			modelMatrix = Matrix4::Translation(offset) * Matrix4::Rotation(1.0f, Vector3(0, 1, 0)) * Matrix4::Translation(-offset) *
				Matrix4::Translation(l->getPos()) * Matrix4::Scale(Vector3(radius, radius, radius));
			l->setPos(modelMatrix.GetPositionVector());
		}
		else
		{ 
			modelMatrix = Matrix4::Translation(l->getPos()) * Matrix4::Scale(Vector3(radius, radius, radius));
		}
		if ((l->getPos() - currentScene->GetSceneCamera()->GetPosition()).Length() < radius)
		{
			glCullFace(GL_FRONT);
		}
		else
		{
			glCullFace(GL_BACK);
		}
		SetShaderLight(l);
		UpdateShaderMatrices();
		l->Draw();
	}
}

void Renderer::DrawShadowScene()
{
	glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[SHADOW_FBO]);
	glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glClear(GL_DEPTH_BUFFER_BIT);

	currentScene->DrawScene(true);

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

void Renderer::DrawCombinedScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[COMBINE_FBO]);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(shaderProgs[COMBINE_SHADER]);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgs[COMBINE_SHADER]->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);
	glUniform1i(glGetUniformLocation(shaderProgs[COMBINE_SHADER]->GetProgram(), "diffuseTex"), 2);
	glUniform1i(glGetUniformLocation(shaderProgs[COMBINE_SHADER]->GetProgram(), "emissiveTex"), 3);
	glUniform1i(glGetUniformLocation(shaderProgs[COMBINE_SHADER]->GetProgram(), "specularTex"), 4);
	glUniform1i(glGetUniformLocation(shaderProgs[COMBINE_SHADER]->GetProgram(), "irradianceTex"), 13);
	glUniform1i(glGetUniformLocation(shaderProgs[COMBINE_SHADER]->GetProgram(), "reflTex"), 14);
	glUniform1i(glGetUniformLocation(shaderProgs[COMBINE_SHADER]->GetProgram(), "finalRender"), 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, fbo_tex[BUF_COLOUR_TEX]);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, fbo_tex[LIGHT_EMISSIVE_TEX]);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, fbo_tex[LIGHT_SPECULAR_TEX]);

	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, fbo_tex[IRRADIANCE_TEX]);

	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, fbo_tex[BUF_REFL_TEX]);

	screenQuad->Draw();
	SwitchToPerspective();
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawPostProcess()
{
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);	
	if (blur)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo[POST_FBO]);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		SetCurrentShader(shaderProgs[BLUR_SHADER]);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_tex[MOTION_BLUR_TWO], 0);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgs[BLUR_SHADER]->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);
		glUniform2f(glGetUniformLocation(shaderProgs[BLUR_SHADER]->GetProgram(), "pixelSize"), pixelPitch.x, pixelPitch.y);
		glDisable(GL_DEPTH_TEST);

		for (int i = 0; i < 4; ++i)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_tex[MOTION_BLUR_TWO], 0);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical "), 0);
			glUniform1i(glGetUniformLocation(shaderProgs[BLUR_SHADER]->GetProgram(), "diffuseTex"), 22);

			if (i == 0)
			{
				glActiveTexture(GL_TEXTURE22);
				glBindTexture(GL_TEXTURE_2D, fbo_tex[COMBINED_TEX]);
			}
			else
			{
				glActiveTexture(GL_TEXTURE22);
				glBindTexture(GL_TEXTURE_2D, fbo_tex[MOTION_BLUR_ONE]);
			}
			screenQuad->Draw();

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), " isVertical "), 1);
			glUniform1i(glGetUniformLocation(shaderProgs[BLUR_SHADER]->GetProgram(), "diffuseTex"), 23);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_tex[MOTION_BLUR_ONE], 0);
			glActiveTexture(GL_TEXTURE23);
			glBindTexture(GL_TEXTURE_2D, fbo_tex[MOTION_BLUR_TWO]);
			screenQuad->Draw();
		}
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}

	SetCurrentShader(shaderProgs[COMBINE_SHADER]);
	glUniform1i(glGetUniformLocation(shaderProgs[COMBINE_SHADER]->GetProgram(), "finalRender"), 1);
	glUniform1i(glGetUniformLocation(shaderProgs[COMBINE_SHADER]->GetProgram(), "diffuseTex"), 2);
	glUniform1i(glGetUniformLocation(shaderProgs[COMBINE_SHADER]->GetProgram(), "blurTex"), 26);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgs[COMBINE_SHADER]->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);
	glActiveTexture(GL_TEXTURE26);

	if (blur)
	{
		glBindTexture(GL_TEXTURE_2D, fbo_tex[MOTION_BLUR_ONE]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, fbo_tex[COMBINED_TEX]);
	}
		
	screenQuad->Draw();
	currentScene->LateDraw();

	SwitchToPerspective();
	glUseProgram(0);
}