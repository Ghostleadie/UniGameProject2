#pragma once
#include <string>
#include <glad/glad.h> 
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>
#include <iostream>
#include <glm/glm.hpp>
class ParticleSystemRenderer
{
public:

	struct Particle
	{
		float cameraSqDistance;
		// sort to ensure the particles further away are drawn first
		bool operator<(const Particle& that) const
		{
			return this->cameraSqDistance > that.cameraSqDistance;
		}

		// initialised to dead particle (-1.0f life)
		Particle() : lifeRemaining(-1.0f), cameraSqDistance(-1.0f) {}
		// variables that need passing to the shader
		glm::vec3 position;
		float size;
		glm::vec4 colour;
		// other variables
		glm::vec3 velocity;
		float lifeRemaining;
	};

	ParticleSystemRenderer(const unsigned int maxParticles = 10000);
	void setTexture(std::string textureFilepath);
	void initialise();
	void update(Particle* particleData, int numberOfLiveParticles);
	void render(unsigned int numberOfParticles, const unsigned int shaderProgram);
	static unsigned int TextureFromFile(const char* filepath, const std::string& directory, bool gamma = false);
	unsigned int VAO;
private:
	

	unsigned int m_textureIndex;
	unsigned int VBO_billboard; // stays the same for each particle
	unsigned int VBO_positionsAndColours; // changes every frame
	int m_maxParticles;
};
