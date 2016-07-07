#include "ParticleEmitter.h"
#include <string>
#include <io.h>
#include <stdio.h>

//Cheap random number generator, will generate
//numbers between 0.0 and 1.0 to 2 DP
#define RAND() ((rand()%101)/100.0f)

ParticleEmitter::ParticleEmitter(Shader* shader)	
{
	particleRate = 100.0f;
	particleLifetime = 2000.0f;
	particleSize = 4.0f;
	particleVariance = 1.0f;
	nextParticleTime = 0.0f;
	particleSpeed = 0.002f;
	numLaunchParticles = 150;
	largestSize = 0;

	nodeShader = shader;

	diffuseTex_loc = glGetUniformLocation(nodeShader->GetProgram(), "diffuseTex");
	pSize_loc = glGetUniformLocation(nodeShader->GetProgram(), "particleSize");
	
	/*Each particle is a white dot, which has an alpha fade on it,
	so the edges fade to 0.0 alpha.*/
	texture = SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + ("particle.png")).c_str(), SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_COMPRESS_TO_DXT);

	if (!texture)
	{
		cout << "Initialisation failed...Emitter texture failed to load." << endl;
		system("pause");
		exit(1);
	}
}

ParticleEmitter::~ParticleEmitter(void)	
{
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		delete particles[i];
	}
	for (unsigned int i = 0; i < freeList.size(); i++)
	{
		delete freeList[i];
	}

	particles.clear();
	freeList.clear();
}

/*
Updates all particles, if it has no 'life' left,
it is removed from the particle list. If it's time to generate some new particles, we do
that in here, too. Finally, this function resizes our VBOs if necessary.
*/
void ParticleEmitter::Update(float msec)	
{
	nextParticleTime -= msec;	//some time has passed!

	//Enough time might have passed that more than 1 'launch' of particles is necessary...
	while (nextParticleTime <= 0) 
	{
		nextParticleTime += particleRate;
		//Add a number of particles to the vector, obtained from the free list.
		for (int i = 0; i < numLaunchParticles; ++i)
		{
			particles.push_back(GetFreeParticle());
		}
	}

	itr = particles.begin();
	while(itr != particles.end())
	{
		Particle *p = (*itr);

		//We're keeping the particles 'life' in the alpha channel of its colour. 
		//This means it'll also fade out as it loses energy.
		p->colour.w -= (msec / particleLifetime);

		//If this particle has ran out of life, remove it from the 'active' list,
		//and put it on the 'free' list for reuse later.
		if (p->colour.w <= 0.0f) 
		{
			freeList.push_back(p);
			itr = particles.erase(itr);
		}
		else
		{
			//Otherwise, this particle must be still 'alive'. Update its
			//position by multiplying its normalised direction by the
			//particle speed, and adding the result to the position.
			p->position += p->direction* (msec * particleSpeed);
			++itr;
		}
	}

	//If we now have more particles than we have graphics memory for, we
	//must allocate some new space for them, using ResizeArrays.
	if (particles.size() > largestSize) 
	{
		ResizeArrays();
	}
	
	//Get 2 contiguous sections of memory full of our particle info
	for (unsigned int i = 0; i < particles.size(); ++i)
	{
		vertices[i] = particles[i]->position;
		colours[i] = particles[i]->colour;
	}
}

/*This function gets a pointer to an unused particle - either from the freelist,
or from a newly created particle on the heap.*/
Particle* ParticleEmitter::GetFreeParticle()	
{
	Particle * p = nullptr;

	//If we have a spare particle on the freelist, pop it off
	if (freeList.size() > 0) 
	{
		p = freeList.back();
		freeList.pop_back();
	}
	else
	{
		//no spare particles, create a new one
		p = new Particle();
	}

	//Reset its values
	p->colour = Vector4(RAND(), RAND(), RAND(), 1.0);
	p->direction = initialDirection;
	p->direction.x += ((RAND() - RAND()) * particleVariance);
	p->direction.y += ((RAND() - RAND()) * particleVariance);
	p->direction.z += ((RAND() - RAND()) * particleVariance);

	p->direction.Normalise();
	p->position.ToZero();

	return p;
}

/*
If we end up with more particles than space in graphics memory, we must
allocate some new graphics memory!
*/
void ParticleEmitter::ResizeArrays() 
{
	//Delete the system memory, as it is of the wrong size now...
	//yes, this does mean we're keeping particle info in THREE places...
	delete[] vertices;
	delete[] colours;

	//Tell OpenGL that we can delete our old VBOs
	glDeleteBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glDeleteBuffers(1, &bufferObject[COLOUR_BUFFER]);

	//Make some new system memory
	vertices = new Vector3[particles.size()];
	colours = new Vector4[particles.size()];

	//Make some new VBO space in memory. Note how we don't actually upload any
	//data to the graphics card here! We just 'new' the memory for now.
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Vector3), 0, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Vector4), 0, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);	

	largestSize = particles.size();		//Update our largest particles size
}

/*
to send using the OpenGL command glBufferSubData
*/
void ParticleEmitter::Draw()	
{
	glBindVertexArray(arrayObject);

	//Bind and update the data
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(Vector3), vertices);
	glEnableVertexAttribArray(VERTEX_BUFFER);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(Vector4), colours);
	glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
	glEnableVertexAttribArray(COLOUR_BUFFER);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glUseProgram(nodeShader->GetProgram());
	glUniform1i(diffuseTex_loc, 0);
	glUniform1f(pSize_loc, particleSize);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	SceneNode::context->UpdateShaderMatrices();

	glDrawArrays(GL_POINTS, 0, particles.size());
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(0);
	glUseProgram(0);
};