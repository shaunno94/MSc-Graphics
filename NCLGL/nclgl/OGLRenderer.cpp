/*
Class:OGLRenderer
Author:Rich Davison
Description:Abstract base rendering class. Creates an OpenGL 
CORE PROFILE rendering context.
*/

#include "OGLRenderer.h"
#include "Shader.h"	
unsigned int	OGLRenderer::bytes_used = 0;

/*
Creates an OpenGL CORE PROFILE rendering context. Sets itself
as the current renderer of the passed 'parent' Window.
*/
OGLRenderer::OGLRenderer(Window &window)	
{
	init					= false;

	HWND windowHandle = window.GetHandle();

	// Did We Get A Device Context?
	if (!(deviceContext=GetDC(windowHandle)))		
	{					
		std::cout << "OGLRenderer::OGLRenderer(): Failed to create window!" << std::endl;
		return;
	}
	
	//A pixel format descriptor is a struct that tells the Windows OS what type of front / back buffers we want to create etc
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize			= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion		= 1; 
   	pfd.dwFlags			= PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   //It must be double buffered, it must support OGL(!), and it must allow us to draw to it...
   	pfd.iPixelType		= PFD_TYPE_RGBA;	//We want our front / back buffer to have 4 channels!
   	pfd.cColorBits		= 32;				//4 channels of 8 bits each!
   	pfd.cDepthBits		= 24;				//24 bit depth buffer
	pfd.cStencilBits	= 8;				//plus an 8 bit stencil buffer
   	pfd.iLayerType		= PFD_MAIN_PLANE;

	GLuint		PixelFormat;
	if (!(PixelFormat=ChoosePixelFormat(deviceContext,&pfd)))		
	{	// Did Windows Find A Matching Pixel Format for our PFD?
		std::cout << "OGLRenderer::OGLRenderer(): Failed to choose a pixel format!" << std::endl;
		return;
	}

	if(!SetPixelFormat(deviceContext,PixelFormat,&pfd))			
	{		// Are We Able To Set The Pixel Format?
		std::cout << "OGLRenderer::OGLRenderer(): Failed to set a pixel format!" << std::endl;
		return;
	}

	HGLRC		tempContext;		//We need a temporary OpenGL context to check for OpenGL 3.2 compatibility...stupid!!!
	if (!(tempContext=wglCreateContext(deviceContext)))				
	{	// Are We Able To get the temporary context?
		std::cout << "OGLRenderer::OGLRenderer(): Cannot create a temporary context!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}

	if(!wglMakeCurrent(deviceContext,tempContext))					
	{	// Try To Activate The Rendering Context
		std::cout << "OGLRenderer::OGLRenderer(): Cannot set temporary context!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}

	//Now we have a temporary context, we can find out if we support OGL 3.x
	char* ver = (char*)glGetString(GL_VERSION); // ver must equal "3.2.0" (or greater!)
	int major = ver[0] - '0';		//casts the 'correct' major version integer from our version string
	int minor = ver[2] - '0';		//casts the 'correct' minor version integer from our version string

	if(major < 3) {					//Graphics hardware does not support OGL 3! Erk...
		std::cout << "OGLRenderer::OGLRenderer(): Device does not support OpenGL 3.x!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}

	if(major == 3 && minor < 2) {	//Graphics hardware does not support ENOUGH of OGL 3! Erk...
		std::cout << "OGLRenderer::OGLRenderer(): Device does not support OpenGL 3.2!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}
	//We do support OGL 3! Let's set it up...

	int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, major,	//TODO: Maybe lock this to 3? We might actually get an OpenGL 4.x context...
        WGL_CONTEXT_MINOR_VERSION_ARB, minor, 
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 
#ifdef OPENGL_DEBUGGING 
		| WGL_CONTEXT_DEBUG_BIT_ARB
#endif		//No deprecated stuff!! DIE DIE DIE glBegin!!!!
		,WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,		//We want everything OpenGL 3.2 provides...
		0					//That's enough attributes...
    };

	//Everywhere else in the Renderers, we use function pointers provided by GLEW...but we can't initialise GLEW yet! So we have to use the 'Wiggle' API
	//to get a pointer to the function that will create our OpenGL 3.2 context...
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
	renderContext = wglCreateContextAttribsARB(deviceContext,0, attribs);

	// Check for the context, and try to make it the current rendering context
	if(!renderContext || !wglMakeCurrent(deviceContext,renderContext))		{			
		std::cout << "OGLRenderer::OGLRenderer(): Cannot set OpenGL 3 context!" << std::endl;	//It's all gone wrong!
		wglDeleteContext(renderContext);
		wglDeleteContext(tempContext);
		return;
	}

	wglDeleteContext(tempContext);	//We don't need the temporary context any more!

	glewExperimental = GL_TRUE;	//This forces GLEW to give us function pointers for everything (gets around GLEW using 'old fashioned' methods
								//for determining whether a OGL context supports a particular function or not
	
	if (glewInit() != GLEW_OK) {	//Try to initialise GLEW
		std::cout << "OGLRenderer::OGLRenderer(): Cannot initialise GLEW!" << std::endl;	//It's all gone wrong!
		return;
	}

	glClearColor(0.2f,0.2f,0.2f,1.0f);			//When we clear the screen, we want it to be dark grey
	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &AF_largest); //Get max amount of AF available on GPU

	currentShader = 0;							//0 is the 'null' object name for shader programs...

	window.SetRenderer(this);					//Tell our window about the new renderer! (Which will in turn resize the renderer window to fit...)

	pixelPitch = Vector2(1.0f / float(width), 1.0f / float(height));
	orthoMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	perspMatrix = Matrix4::Perspective(NEAR_PLANE, FAR_PLANE, float(width) / float(height), FOV);
}

/*
Destructor. Deletes the default shader, and the OpenGL rendering context.
*/
OGLRenderer::~OGLRenderer(void)	
{
	wglDeleteContext(renderContext);
}

/*
Returns TRUE if everything in the constructor has gone to plan.
Check this to end the application if necessary...
*/
bool OGLRenderer::HasInitialised() const
{
	return init;
}

/*
Resizes the rendering area. Should only be called by the Window class!
Does lower bounds checking on input values, so should be reasonably safe
to call.
*/
void OGLRenderer::Resize(int x, int y)	
{
	width	= max(x,1);	
	height	= max(y,1);
	glViewport(0,0,width,height);
}

/*
Swaps the buffers, ready for the next frame's rendering. Should be called
every frame, at the end of RenderScene(), or whereever appropriate for
your application.
*/
void OGLRenderer::SwapBuffers() 
{
	//We call the windows OS SwapBuffers on win32. Wrapping it in this 
	//function keeps all the tutorial code 100% cross-platform (kinda).
	::SwapBuffers(deviceContext);
}
/*
Used by some later tutorials when we want to have framerate-independent
updates on certain datatypes. Really, OGLRenderer should have its own
timer, so it can just sit and look after itself (potentially even in
another thread...), but it's fine for the tutorials.

STUDENTS: Don't put your entity update routine in this, or anything like
that - it's just asking for trouble! Strictly speaking, even the camera
shouldn't be in here...(I'm lazy)
*/
void OGLRenderer::UpdateScene(float msec)	{}

/*
Updates the uniform matrices of the current shader. Assumes that
the shader has uniform matrices called modelMatrix, viewMatrix,
projMatrix, and textureMatrix. Updates them with the relevant
matrix data. Sanity checks currentShader, so is always safe to
call.
*/
void OGLRenderer::UpdateShaderMatrices()	
{
	if(currentShader) 
	{
		MVP = projMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&modelMatrix);
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, (float*)&viewMatrix);
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "MVP"), 1, false, (float*)&MVP);
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, (float*)&textureMatrix);
	}
}

void OGLRenderer::SetCurrentShader(Shader*s) 
{
	currentShader = s;
	glUseProgram(s->GetProgram());
}

void OGLRenderer::SetTextureRepeating(GLuint target, bool repeating )	
{
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OGLRenderer::SetTextureFiltering(GLuint target, bool filtering, bool AF)
{
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, AF ? 16 : 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OGLRenderer::GenerateMipMaps(GLuint target)
{
	glBindTexture(GL_TEXTURE_2D, target);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OGLRenderer::SetCubeMapParams(GLuint target)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, target);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void OGLRenderer::SwitchToOrtho()
{
	//projMatrix = Matrix4::Orthographic(-NEAR_PLANE, FAR_PLANE, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
	projMatrix = orthoMatrix;
}

void OGLRenderer::SwitchToPerspective()
{
	projMatrix = perspMatrix;
}

void OGLRenderer::SetShaderLight(Light* l) 
{
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "lightPos"), 1,(float*)&l->getPos());
	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "lightColour"),1 ,(float*)&l->getColour());
	glUniform1f(glGetUniformLocation(currentShader->GetProgram() , "lightRadius"), l->getRadius());
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "intensity"), l->getIntensity());
}