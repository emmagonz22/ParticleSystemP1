#include "ofApp.h"
//Be aware if you pause something could no be seen on the replay (Example: Velocity incress or decress)
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	int num = 1500; 
	p.assign(num, Particle());
	currentMode = PARTICLE_MODE_ATTRACT;

	currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";

	resetParticles();
}

//--------------------------------------------------------------

void ofApp::resetParticles(){

	//these are the attraction points used in the fourth demo
	attractPoints.clear();
	for(int i = 0; i < 4; i++){
		attractPoints.push_back( glm::vec3( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) , 0) );
	}

	attractPointsWithMovement = attractPoints;

	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressedReplay(int key) //This function will be use to replay the recorded keys
{

	if (key == '1') //Attract particle to mouse
	{
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
	}
	//-------------------Separate keys----------------------------------
	if (key == '2') //Repel particle from mouse
	{
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
	}
	//-------------------Separate keys----------------------------------
	if (key == '3') //Mode particle to the nearest circle created by a random number
	{

		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force";
	}
	//-------------------Separate keys----------------------------------
	if (key == '4') //Raining particle, It go crazy if you press T or t to many times (This is part of the fun)
	{
		currentMode = PARTICLE_MODE_NOISE;
		currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation";
		resetParticles();
	}
	//-------------------Separate keys----------------------------------
	if( key == '5') //Combination of repel and Noise, with square particle, It go crazy if you press T or t to many times (This is part of the fun)
	{
		currentMode = PARTICLE_MODE_CONFETTI;
		currentModeStr = "5 - PARTICLE_MODE_CONFETTI: PARTY!! move mouse to repel confetti";
	}
	//-------------------Separate keys----------------------------------
	if (key == ' ') //Reset particle
	{
		resetParticles();
	}
	//-------------------Separate keys----------------------------------
	if (key == 'D' || key == 'M' || key == 'd' || key == 'm')
	{
		for (int i = 0; i < p.size(); i++)
		{
			p[i].sizeChange(key); //Changes the size of the particle
		}
	}
	//-------------------Separate keys----------------------------------
	if (key == 'T' || key == 'S' || key == 't' || key == 's')
	{
		for (int i = 0; i < p.size(); i++)
		{
			p[i].velocityChange(key); // Changes the velocity of the particle
		}
	}
	if (key == 'A' || key == 'a') //Pause particle and his color when 'a' or 'A' is pressed
	{
		pauseParticles();
		for (unsigned int i = 0; i < p.size(); i++)
		{
			p[i].pauseColor(); // pauses the color of the particle
		}
	}
	if (key == 'B' || key == 'b') // If 'b' or 'B' key is pressed it will call backGroundColor().
	{
		backGroundColor();
	}
}
//--------------------------------------------------------------
void ofApp::recordKey(int key) 
{
	
	if(!pressR)
	{
		pressR = true;
		wordRecorded.clear(); // resets the vector
		if(countR >= 2)
		{
			countR = 0; // resets the count of 'R' keys
		}
		pressR = true;
		countR++;
	}else
	{
		countR++; // Adds 1 to countR
		pressR = false;
		vectorSize = wordRecorded.size();
	}
		
	

}
//--------------------------------------------------------------
void ofApp::pauseParticles() // This function pauses the particles
{
	if(isTrue == true)
	{
		isTrue = false;
	}else
	{
		isTrue = true;
	}
}
void ofApp::backGroundColor() //This function will change the background color
{
	if(!pressB)
	{
		pressB = true;
		c++; // Use to iterate pastelColor vector
		if (c == pastelColor.size())
		{
			c = 0;
		}
	}
	else
	{
		pressB = false;
		c++; // Use to iterate pastelColor vector
		if(c == pastelColor.size()) // if c equals to the size of pastelColor 'c' will reset
		{
			c = 0;
		}
	}
}
//--------------------------------------------------------------
void ofApp::update(){
	if(isTrue)//Pause if A was pressed
	{
			for(unsigned int i = 0; i < p.size(); i++)
		{
			p[i].setMode(currentMode);
			p[i].update();
		}

		//lets add a bit of movement to the attract points
		for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
			attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
			attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
		}
	}
	if(pressP && u < 120) // This creates the time intervals between each key replay
	{
		u++;//interval counter
	}else if(pressP && u == 120)
	{
		
		keyPressedReplay(wordRecorded[n]); // Runs the replay with each key that was recorded.
		n++;//index counter

		if(n == wordRecorded.size()) // The key limit was reached and the replay stops
		{
			pressP = false;
			wordRecorded.clear(); //Resets the vector
			n = 0; // Resets the iterating value 
		}
		u = 0; //interval counter reset
	}
}
//--------------------------------------------------------------
void ofApp::draw(){
	if (true) 
	{
		if (pressB) // If true the background color will change 
		{
			ofBackgroundGradient(ofColor(pastelColor[c][0], pastelColor[c][1], pastelColor[c][2]), ofColor(10, 10, 10));
		}
		else 
		{
			ofBackgroundGradient(ofColor(pastelColor[c][0], pastelColor[c][1], pastelColor[c][2]), ofColor(10, 10, 10));
		}
	}

	if(pressR) // Draws the "REC" circle that indicates that the recording key is ON. 
	{
		ofSetColor(255,0,0);
		ofNoFill();
		ofDrawCircle(ofGetWidth() / 2 * 1.8 , ofGetHeight() / 2 * .2, 30);
		ofFill();
		ofDrawCircle(ofGetWidth() / 2 * 1.8 , ofGetHeight() / 2 * .2, 20);
		ofSetColor(255,255,255);
		ofDrawBitmapString("REC", ofGetWidth() / 2 * 1.8 - 11, ofGetHeight() / 2 * .2 + 5);

	}else if(pressP) // Draws the "Replay symbol"
	{
		ofSetColor(255,0,0);
		ofDrawTriangle(ofGetWidth() / 2 * 1.73 , ofGetHeight() / 2 * .22,ofGetWidth() / 2 * 1.73, ofGetHeight() / 2 * .32, ofGetWidth() / 2 * 1.78 , ofGetHeight() / 2 * .27);
		ofDrawTriangle(ofGetWidth() / 2 * 1.68 , ofGetHeight() / 2 * .22,ofGetWidth() / 2 * 1.68, ofGetHeight() / 2 * .32, ofGetWidth() / 2 * 1.73 , ofGetHeight() / 2 * .27);

		ofSetColor(255,255,255);
		ofDrawTriangle(ofGetWidth() / 2 * 1.75 , ofGetHeight() / 2 * .2,ofGetWidth() / 2 * 1.75, ofGetHeight() / 2 * .3, ofGetWidth() / 2 * 1.8 , ofGetHeight() / 2 * .25);
		ofDrawTriangle(ofGetWidth() / 2 * 1.7 , ofGetHeight() / 2 * .2,ofGetWidth() / 2 * 1.7, ofGetHeight() / 2 * .3, ofGetWidth() / 2 * 1.75 , ofGetHeight() / 2 * .25);

	}
	for(unsigned int i = 0; i < p.size(); i++)
	{
		p[i].draw();
	}
	ofSetColor(190);

	if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
		for(unsigned int i = 0; i < attractPoints.size(); i++){
			ofNoFill();
			ofDrawCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofDrawCircle(attractPointsWithMovement[i], 4);
		}
	}

	ofSetColor(230);
	ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-5 to change mode.", 10, 20);
	if(countR == 2)
	{
		ofDrawBitmapString("Recorded keys: \n", 10, 100);
		for(int i = 0; i < vectorSize; i++)
		{
				ofDrawBitmapString((char)wordRecorded[i], 10, 111 + (i * 11)); // Displays the keys that were recorded

		}

	}
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(!pressP) //Check is P is active, if p is active cancel all the other
	{

		if ( key == '1'){ //Attract particle to the mouse
			currentMode = PARTICLE_MODE_ATTRACT;
			currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
		}
	//-------------------Separate keys----------------------------------
		if ( key == '2'){ //Repel particle from the mouse
			currentMode = PARTICLE_MODE_REPEL;
			currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
		}
	//-------------------Separate keys----------------------------------
		if( key == '3'){ //Move particle to the nearest circle created by a random number

			currentMode = PARTICLE_MODE_NEAREST_POINTS;
			currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force";
		}
	//-------------------Separate keys----------------------------------
		if( key == '4'){ //Raining particle, It go crazy if you press T or t to many times (This is part of the fun)
			currentMode = PARTICLE_MODE_NOISE;
			currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation";
			resetParticles();
		}
	//-------------------Separate keys----------------------------------
		if( key == '5')//Combination of repel and Noise, with square particle, It go crazy if you press T or t to many times (This is part of the fun)
		{
			currentMode = PARTICLE_MODE_CONFETTI;
			currentModeStr = "5 - PARTICLE_MODE_CONFETTI: PARTY!! move mouse to repel confetti";
			resetParticles();
		}
	//-------------------Separate keys----------------------------------
		if( key == ' ' ){ //ReSET PARTICLES
			resetParticles();
		}
	//-------------------Separate keys----------------------------------
		if( key == 'A' || key == 'a') //Pause particle and his color when 'a' or 'A' is pressed
		{
			pauseParticles();
			for(unsigned int i = 0; i < p.size(); i++)
		{
			p[i].pauseColor(); // pauses the color of the particle
		}

		}
	//-------------------Separate keys----------------------------------
		if( key == 'D' || key == 'M' || key == 'd' || key == 'm')
		{
			for(int i = 0; i < p.size(); i++)
			{
				p[i].sizeChange(key); //Changes the size of the particle
			}
		}
	//-------------------Separate keys----------------------------------
		if( key == 'R' || key == 'r' ) //Activate the record function
		{

			recordKey(key);		
		}
		if(key == 'A' || key == 'a' || key == 'T' || key == 't' || key == 'S' || key == 's' || key == 'D' || key == 'd' || key == 'M' || key == 'm' || key == 'B' || key == 'b' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == ' ')
		{
			if(pressR) //Limit what keys are going to register in the vector 
			{
				wordRecorded.push_back(key); // Save the last pressed key
			}
		}
		//-------------------Separate keys----------------------------------
		if(!pressR){
			if( key == 'P' || key == 'p' )
			{
			if(!wordRecorded.empty()) //Checks if the wordRecorded vector is not empty
                {
                    if(!pressP)
                    {
                        pressP = true;
                    }
                }
			}
		}
		if (key == 'T' || key == 'S' || key == 't' || key == 's')
		{
			for (int i = 0; i < p.size(); i++)
			{
				p[i].velocityChange(key); // Changes the velocity of the particle
			}
		}
		if (key == 'B' || key == 'b') // If 'b' or 'B' key is pressed it will call backGroundColor().
		{
			backGroundColor();
		}
	}
//-------------------Separate keys----------------------------------
	if(key == 'C' || key == 'c') //Cancel the replay
	{
		if(pressP)
		{
			wordRecorded.clear();
		}
		pressP = false;
	}




}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
