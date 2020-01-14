#pragma once
#include "Component.h"

#include <glad/glad.h> 
//#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ParticleSystemRenderer.h"

#include <string>
#include <vector>
class ParticleComponent : public Component {
public:

//	
//	struct Particle
//	{
//		float cameraSqDistance;
//		// sort to ensure the particles further away are drawn first
//		bool operator<(const Particle& that) const
//		{
//			return this->cameraSqDistance > that.cameraSqDistance;
//		}
//
//		// initialised to dead particle (-1.0f life)
//		Particle() : life(-1.0f), cameraSqDistance(-1.0f) {}
//		// variables that need passing to the shader
//		glm::vec3 position;
//		float size;
//		glm::vec4 colour;
//		// other variables
//		glm::vec3 velocity;
//		float life;
//	};
//
//
//	int MaxParticles;
//	Particle ParticlesContainer[500];
//	int LastUsedParticle = 0;
//	ParticleSystemRenderer* renderer;
//
//	GLuint particles_position_buffer;
//	GLuint billboard_vertex_buffer;
//	GLuint particles_color_buffer;
//	GLfloat g_particule_position_size_data[2000];
//	GLubyte g_particule_color_data[2000];
//	int currentCount;
//	int ParticlesCount;
//	float m_timeSinceLastParticleGen;
//
//	bool active = false;
//	
//	void start() {
//		active = true;
//	}
//	void stop() {
//		active = false;
//	}
//
//
//	ParticleComponent
//	(const unsigned int maxParticles = 500) 
//	//m_particleRenderer(maxParticles), m_maxParticles(maxParticles)
//	{
//		renderer = new ParticleSystemRenderer(maxParticles);
//		MaxParticles = maxParticles;
//		//ParticlesContainer = new Particle[maxParticles];
//		LastUsedParticle = 0;
//		ParticlesCount = 0;
//		m_timeSinceLastParticleGen = 0;
//	}
//
//	int FindUnusedParticle() {
//		//check next particle in array
//		for (int i = LastUsedParticle; i<MaxParticles; i++) {
//			if (ParticlesContainer[i].life < 0) {
//				LastUsedParticle = i;
//				return i;
//			}
//		}
//		//check for dead particles prior to current particle
//		for (int i = 0; i<LastUsedParticle; i++) {
//			if (ParticlesContainer[i].life < 0) {
//				LastUsedParticle = i;
//				return i;
//			}
//		}
//
//		return 0; // All particles are taken, override the first one
//	}
//
//	void OnUpdate(float dt)
//	{
//		m_timeSinceLastParticleGen += dt;
//		if (m_timeSinceLastParticleGen > 1)
//		{
//			int particleIndex = FindUnusedParticle();
//			ParticlesContainer[particleIndex].life = 500.0f;
//			float randomXStart = ((rand() % RAND_MAX) / (float)RAND_MAX) * 50000.0f - 25000.0f;
//			float randomZStart = ((rand() % RAND_MAX) / (float)RAND_MAX) * 50000.0f - 25000.0f;
//			ParticlesContainer[particleIndex].position = glm::vec3(randomXStart, 5000, randomZStart);
//			ParticlesContainer[particleIndex].velocity = glm::vec3(0, 0, 0);
//			ParticlesContainer[particleIndex].colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//			ParticlesContainer[particleIndex].size = 100;
//			m_timeSinceLastParticleGen = 0;
//		}
//
//		std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
//
//	}
//
//	
//}

	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a; // Color
		float size, angle;
		float life; // Remaining life of the particle. if < 0 : dead and unused.
		float cameradistance;

		// initialised to dead particle (-1.0f life)
		Particle() : life(-1.0f), cameradistance(-1.0f) {}
		// variables that need passing to the shader
//		glm::vec3 position;
//		float size;
//		glm::vec4 colour;
		// other variables
//		glm::vec3 velocity;
//		float life;

		bool operator<(Particle& that) {
			// Sort in reverse order : far particles drawn first.
			return this->cameradistance > that.cameradistance;
		}
	};

	int MaxParticles = 100000;
	Particle ParticlesContainer[100000];
	int LastUsedParticle = 0;

	GLuint particles_position_buffer;
	GLuint billboard_vertex_buffer;
	GLuint particles_color_buffer;
	GLfloat g_particule_position_size_data[2000];
	GLubyte g_particule_color_data[2000];
	int currentCount;
	int ParticlesCount = 0;
	bool active = false;

	void start() {
		active = true;
	}
	void stop() {
		active = false;
	}

	ParticleComponent(){
		// The VBO containing the 4 vertices of the particles.
		// Thanks to instancing, they will be shared by all particles.
		static const GLfloat g_vertex_buffer_data[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
		};
		
		glGenBuffers(1, &billboard_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		// The VBO containing the positions and sizes of the particles
		glGenBuffers(1, &particles_position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		// The VBO containing the colors of the particles	
		glGenBuffers(1, &particles_color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);


		for (int i = 0; i < MaxParticles; i++)
		{
			ParticlesContainer[i].a = rand() % 50 + 25;
			int colour = rand() % 100 + 100;
			ParticlesContainer[i].r = colour;
			ParticlesContainer[i].g = colour;
			ParticlesContainer[i].b = colour;
			ParticlesContainer[i].size = 1;
			ParticlesContainer[i].life = 0;
			ParticlesContainer[i].speed.x = 0;
			ParticlesContainer[i].speed.y = -1;
			ParticlesContainer[i].speed.z = 0;
		}
	}
	void OnUpdate(float dt) override {}
	void OnMessage(const std::string m) override {}

	void OnUpdate(float dt, glm::vec3 camPos, glm::vec3 partPos) {

		ParticlesCount = 0;
		
		for (int i = 0; i<MaxParticles; i++) {

			Particle& p = ParticlesContainer[i]; // shortcut

			if (p.life > 0.0f) {

				// Decrease life
				p.life -= dt;
				if (p.life > 0.0f) {

					// Simulate simple physics : gravity only, no collisions
					p.speed += glm::vec3(0.0f, 0.0f, 0.0f) * (float)dt * 0.5f;
					p.pos += p.speed * (float)dt;
					p.cameradistance = glm::length(p.pos - camPos);
					//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;
					int a = 4 * ParticlesCount;
					int b = 4 * ParticlesCount + 1;
					int c = 4 * ParticlesCount + 2;
					int d = 4 * ParticlesCount + 3;

					// Fill the GPU buffer
					g_particule_position_size_data[a] = p.pos.x;
					g_particule_position_size_data[b] = p.pos.y;
					g_particule_position_size_data[c] = p.pos.z;

					g_particule_position_size_data[d] = p.size;

					g_particule_color_data[a] = p.r;
					g_particule_color_data[b] = p.g;
					g_particule_color_data[c] = p.b;
					g_particule_color_data[d] = p.a;

				}
				else {
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}
				
				ParticlesCount++;

			}
			else {
				p.pos = partPos;
				p.life = -1;
			}
		}

		if (active)
		{
			for (int i = 0; i < rand() % 5 + 1; i++)
			{
				ParticlesContainer[FindUnusedParticle()].life = 2;
			}
		}
		std::cout << ParticlesCount << " currentParticles\n";

		sortParticles();
	
		//glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		//glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		//glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

		//glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		//glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		//glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);
	}
	void sortParticles() {
		std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
	}
	void draw() {
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0, // attribute. No particular reason for 0, but must match the layout in the shader.
			3, // size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
			);

		// 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glVertexAttribPointer(
			1, // attribute. No particular reason for 1, but must match the layout in the shader.
			4, // size : x + y + z + size => 4
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
			);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glVertexAttribPointer(
			2, // attribute. No particular reason for 2, but must match the layout in the shader.
			4, // size : r + g + b + a => 4
			GL_UNSIGNED_BYTE, // type
			GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0, // stride
			(void*)0 // array buffer offset
			);

		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad -> 1

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);
	}

	int FindUnusedParticle() {
		//check next particle in array
		for (int i = LastUsedParticle; i<MaxParticles; i++) {
			if (ParticlesContainer[i].life < 0) {
				LastUsedParticle = i;
				return i;
			}
		}
		//check for dead particles prior to current particle
		for (int i = 0; i<LastUsedParticle; i++) {
			if (ParticlesContainer[i].life < 0) {
				LastUsedParticle = i;
				return i;
			}
		}

		return 0; // All particles are taken, override the first one
	}
	//ParticleComponent()
};