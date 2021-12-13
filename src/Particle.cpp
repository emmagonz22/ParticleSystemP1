#include "Particle.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//------------------------------------------------------------------
Particle::Particle()
{
	attractPoints = NULL;
}

//------------------------------------------------------------------
void Particle::setMode(particleMode newMode)
{
	mode = newMode;
}

//------------------------------------------------------------------
void Particle::setAttractPoints(vector<glm::vec3> *attract)
{
	attractPoints = attract;
}

//------------------------------------------------------------------
void Particle::reset()
{
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);

	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
	pos.z = 0;

	vel.x = sVelocity;
	vel.y = sVelocity;
	vel.z = 0;

	frc = glm::vec3(0, 0, 0);

	scale = ofRandom(0.5, 1.0);

	if (mode == PARTICLE_MODE_NOISE)
	{
		drag = ofRandom(0.97, 0.99);
		vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
	}
	else
	{
		drag = ofRandom(0.95, 0.998);
	}
}
//------------------------------------------------------------------
void Particle::pauseColor() //This function pauses the color of the particle when pauseParticles is called
{
	if(ifPause)
	{
		ifPause = false;
	}else
	{
		ifPause = true;
	}
	
}
//------------------------------------------------------------------
void Particle::confetti(int x, int y) //Copy of Repel function with minor changes
{
	glm::vec3 attractPt(ofGetMouseX(), ofGetMouseY(), 0);
	frc = attractPt - pos;

	//let get the distance and only repel points close to the mouse
	float dist = glm::length(frc);
	frc = glm::normalize(frc);

	vel *= drag;
	if (dist < 175)
	{
		vel += -frc * 0.8; //notice the frc is negative
	}
	else
	{
		//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy.
		frc.x = ofSignedNoise(uniqueVal, pos.y * 0.02, ofGetElapsedTimef() * 0.3);
		frc.y = ofSignedNoise(uniqueVal, pos.x * 0.02, ofGetElapsedTimef() * 0.3);
		vel += frc * 0.06;
	}
}
//------------------------------------------------------------------
void Particle::sizeChange(int key) //Changes the size of the particle
{
	
	if(key == 'D' || key == 'd') // Doubles the size of the particle
	{
		particleSize *= 2.0;
	}else if(key == 'M' || key == 'm') // Shrinks the size of the particle
	{
		particleSize /= 2.0;
	}
	
}
//------------------------------------------------------------------
void Particle::velocityChange(int key) // Changes the velocity of the particle
{ /*Remember the velocity is going to be crazy if you increment it to much 
Also velocity act diferent on some modes, and it can be descreased over time if you stay to long in a mode*/
	if( key == 'T' || key == 't'){ // Doubles the velocity of the particle
		sVelocity *= 2.0; 
		vel *= 2.0; 
	}
	else if (key == 'S' || key == 's'){ // Reduces the velocity of the particle
		sVelocity /= 2.0;
		vel /= 2.0; 
	}
}
//------------------------------------------------------------------
void Particle::attractToPoint(int x, int y)
{
	glm::vec3 attractPt(x, y, 0);
	frc = attractPt - pos;	   // we get the attraction force/vector by looking at the mouse pos relative to our pos
	frc = glm::normalize(frc); //by normalizing we disregard how close the particle is to the attraction point

	vel *= drag;	  //apply drag
	vel += frc * 0.6; //apply force
}
//------------------------------------------------------------------
void Particle::repelFromPoint(int x, int y)
{
	glm::vec3 attractPt(ofGetMouseX(), ofGetMouseY(), 0);
	frc = attractPt - pos;

	//let get the distance and only repel points close to the mouse
	float dist = glm::length(frc);
	frc = glm::normalize(frc);

	vel *= drag;
	if (dist < 150)
	{
		vel += -frc * 0.6; //notice the frc is negative
	}
	else
	{
		//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy.
		frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef() * 0.2);
		frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef() * 0.2);
		vel += frc * 0.04;
	}
}
//------------------------------------------------------------------
void Particle::update()
{

	// APPLY THE FORCES BASED ON WHICH MODE WE ARE IN

	if (mode == PARTICLE_MODE_ATTRACT)
	{
		attractToPoint(ofGetMouseX(), ofGetMouseY());
	}
	else if (mode == PARTICLE_MODE_REPEL)
	{
		repelFromPoint(ofGetMouseX(), ofGetMouseY());
	}
	else if (mode == PARTICLE_MODE_NOISE)
	{
		//lets simulate falling snow
		//the fake wind is meant to add a shift to the particles based on where in x they are
		//we add pos.y as an arg so to prevent obvious vertical banding around x values - try removing the pos.y * 0.006 to see the banding
		float fakeWindX = ofSignedNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);

		frc.x = fakeWindX * 0.25 + ofSignedNoise(uniqueVal, pos.y * 0.04) * 0.6;
		frc.y = ofSignedNoise(uniqueVal, pos.x * 0.006, ofGetElapsedTimef() * 0.2) * 0.09 + 0.18;

		vel *= drag;
		vel += frc * 0.4;

		//we do this so as to skip the bounds check for the bottom and make the particles go back to the top of the screen
		if (pos.y + vel.y > ofGetHeight())
		{
			pos.y -= ofGetHeight();
		}
	}
	else if (mode == PARTICLE_MODE_NEAREST_POINTS)
	{

		if (attractPoints)
		{

			// find closest attractPoint
			glm::vec3 closestPt;
			int closest = -1;
			float closestDist = 9999999;

			for (unsigned int i = 0; i < attractPoints->size(); i++)
			{
				float lenSq = glm::length2(attractPoints->at(i) - pos);
				if (lenSq < closestDist)
				{
					closestDist = lenSq;
					closest = i;
				}
			}

			//if we have a closest point - lets calcuate the force towards it
			if (closest != -1)
			{
				closestPt = attractPoints->at(closest);
				float dist = sqrt(closestDist);

				//in this case we don't normalize as we want to have the force proportional to distance
				attractToPoint(closestPt.x, closestPt.y);

				//lets also limit our attraction to a certain distance and don't apply if 'f' key is pressed
				if (dist < 300 && dist > 40 && !ofGetKeyPressed('f'))
				{
					vel += frc * 0.003;
				}
				else
				{
					//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy.
					frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef() * 0.2);
					frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef() * 0.2);
					vel += frc * 0.4;
				}
			}
		}
	}else if(mode == PARTICLE_MODE_CONFETTI)
	{ 	//MODIFY version of PARTICLE_MODE_NOISE and REPEL (Union with both mode, minor change in the numbers)
		//lets simulate falling snow
		//the fake wind is meant to add a shift to the particles based on where in x they are
		//we add pos.y as an arg so to prevent obvious vertical banding around x values - try removing the pos.y * 0.006 to see the banding
		confetti(ofGetMouseX(), ofGetMouseY());
		float fakeWindX = ofSignedNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6); 

		frc.x = fakeWindX * 0.25 + ofSignedNoise(uniqueVal, pos.y * 0.04) * 0.6;
		frc.y = ofSignedNoise(uniqueVal, pos.x * 0.006, ofGetElapsedTimef() * 0.2) * 0.09 + 0.18;

		vel *= drag;
		vel += frc * 0.4;

		//we do this so as to skip the bounds check for the bottom and make the particles go back to the top of the screen
		if (pos.y + vel.y > ofGetHeight())
		{
			pos.y -= ofGetHeight();
		}
	}

	// UPDATE OUR POSITION

	pos += vel;

	// LIMIT THE PARTICLES TO STAY ON SCREEN
	//we could also pass in bounds to check - or alternatively do this at the ofApp level
	if (pos.x > ofGetWidth())
	{
		pos.x = ofGetWidth();
		vel.x *= -1.0;
	}
	else if (pos.x < 0)
	{
		pos.x = 0;
		vel.x *= -1.0;
	}
	if (pos.y > ofGetHeight())
	{
		pos.y = ofGetHeight();
		vel.y *= -1.0;
	}
	else if (pos.y < 0)
	{
		pos.y = 0;
		vel.y *= -1.0;
	}
}

//------------------------------------------------------------------
void Particle::draw()
{
	if(isFigure != 1) //Check if the boton pressed is 5
	{
		srand(time(NULL));
	}

	if(ifPause) //If true the particle will have a random color each time. If the particles are paused this condition will equal to false and the particle color will be paused 
	{
	random_R = (rand() % 255) + 1;
	random_G = (rand() % 255) + 1;
	random_B = (rand() % 255) + 1;
	}
	if (mode == PARTICLE_MODE_ATTRACT) //isFigure check what figure is going to use isFigure == 0 Circle isFigure == 1 Rectangle, There can be more and its going to be easier to implement more figure with this var
	{
		ofSetColor(random_R, random_G, random_B);
		isFigure = 0;
	}
	else if (mode == PARTICLE_MODE_REPEL)
	{
		ofSetColor(random_R, random_G, random_B);
		isFigure = 0;
	}
	else if (mode == PARTICLE_MODE_NOISE)
	{
		ofSetColor(random_R, random_G, random_B);
		isFigure = 0;
	}
	else if (mode == PARTICLE_MODE_NEAREST_POINTS)
	{
		ofSetColor(random_R, random_G, random_B);
		isFigure = 0;
	}
	else if (mode == PARTICLE_MODE_CONFETTI) //New mode of Confetti, this is a union between Repel and Noise
	{
		ofSetColor(random_R,random_G,random_B, 255);				
		isFigure = 1;
	}

	switch (isFigure) //Switch case in case we want to implement new figures or changes in the figures easier
	{
	case 1:
		ofDrawRectangle(pos.x + ofRandom(1, 10), pos.y + ofRandom(1, 10), scale * particleSize + 2, scale * particleSize * ofRandom(1,2));
		break;
	default:
		ofDrawCircle(pos.x, pos.y, scale * particleSize);
		break;
	}
}
