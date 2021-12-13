#pragma once
#include "ofMain.h"


enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_NOISE,
	PARTICLE_MODE_CONFETTI
};

class Particle{

	public:
		Particle();
		
		void setMode(particleMode newMode);	
		void setAttractPoints( vector <glm::vec3> * attract );
		void attractToPoint(int, int);
		void repelFromPoint(int, int);
		void confetti(int, int);
		void reset();
		void update();
		void draw();

		void pauseColor();// Pause the color at the same time as pauseParticle (Included in the ofApp.h)
		void sizeChange(int key); //Double or Half the size of the Particles
		void velocityChange(int key); // Changes the velocity of the particles


		glm::vec3 pos;
		glm::vec3 vel;
		glm::vec3 frc;
		
		float drag; 
		float uniqueVal;
		float scale;
		bool ifPause = true; // Plays a role in pauseColor function
		int random_R = (rand() % 255) + 1; //Takes a random color
		int random_G = (rand() % 255) + 1;
		int random_B = (rand() % 255) + 1;
		particleMode mode;

		int isFigure = 0; //Check the actual mode and the figure it use 0 == circle 1 == rectangle 2 == triangle
		float particleSize = 4.0; //Size of the particle it can double its size or half with d and h
		double sVelocity = ofRandom(-3.9,3.9); // Velocity variable, this will change the speed with a factor
		
		vector <glm::vec3> * attractPoints; 
};
