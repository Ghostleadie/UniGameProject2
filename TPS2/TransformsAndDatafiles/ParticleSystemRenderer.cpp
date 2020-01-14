#include "ParticleSystemRenderer.h"

ParticleSystemRenderer::ParticleSystemRenderer(const unsigned int maxParticles)
{
	m_maxParticles = maxParticles;
}

void ParticleSystemRenderer::setTexture(std::string textureFilepath)
{
	m_textureIndex = TextureFromFile("smoke.png",textureFilepath);
}

void ParticleSystemRenderer::initialise()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// A quad representing the particle to be drawn
	// only need one, which will be instanced
	static const GLfloat vertexData[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};

	// load data into vertex buffers
	glGenBuffers(1, &VBO_billboard);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_billboard);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// VBO containing the positions of the particles
	glGenBuffers(1, &VBO_positionsAndColours);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positionsAndColours);
	// Initialise with empty (NULL) buffer
	// subsequently updated each frame (STREAM_DRAW).
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles *
		sizeof(Particle), NULL, GL_STREAM_DRAW);
}

void ParticleSystemRenderer::update(Particle* particleData, int numberOfLiveParticles) {
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positionsAndColours);
	// Set Buffer to NULL before overwrite with SubData
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(Particle), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numberOfLiveParticles * sizeof(Particle), particleData);
	// set the vertex attribute pointers
	// billboard vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_billboard);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// position and size
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positionsAndColours);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
	// colours
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, colour));
}

void ParticleSystemRenderer::render(unsigned int numberOfParticles, const unsigned int shaderProgram)
{
	glUseProgram(shaderProgram);
	glVertexAttribDivisor(0, 0); // billboard : always reuse 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // pos : one per particle -> 1
	glVertexAttribDivisor(2, 1); // colour : one per particle -> 1
								 // draw the instanced particles
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numberOfParticles);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

unsigned int ParticleSystemRenderer::TextureFromFile(const char * filepath, const std::string & directory, bool gamma)
{
	std::string filename = std::string(filepath);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* textureData = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (textureData)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		//glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(textureData);
	}
	else
	{
		std::cout << "Texture failed to load from: " << filepath << std::endl;
		stbi_image_free(textureData);
	}

	return textureID;
}
