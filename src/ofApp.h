#pragma once

#include "ofMain.h"
#include "Particle.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void resetParticles();
		void pauseParticles(); // Pauses the particles 
		void recordKey(int key); 
		void keyPressedReplay(int key); // This function will be use to replay the recorded keys
		void backGroundColor(); //Function to change background color

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		particleMode currentMode;
		string currentModeStr;

		bool isTrue = true; // This plays a major role on the ability to pause the particles
		bool pressR = false; // Boolean to see if 'R' key is active 
		bool pressP = false; // Boolean to see if 'P' key is active
		bool pressB = false; // Boolean to see if 'B' key is active

		int u = 0; //Plays a role in the time intervals of the replay
		int n = 0; //Variable to iterate trought wordRecorded vector
		int countR = 0; //Counts 'R' key to know when to stop the recording
		int c = 0; // This value will be of use when iterating throught pastelColor vector
		double normalRate;

		int vectorSize = 0;
		vector<int> wordRecorded; // This vector stores the keys while "Record key is on"
		vector<vector<int>> pastelColor {{224, 187, 228},{149, 125, 173},{210, 145, 188},{254, 200, 216},{255, 223, 211}};
		// pastelColor holds 5 pastelColors
		vector <Particle> p;
		vector <glm::vec3> attractPoints;
		vector <glm::vec3> attractPointsWithMovement;

};
