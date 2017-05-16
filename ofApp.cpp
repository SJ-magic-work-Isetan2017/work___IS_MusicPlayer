/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp()
: b_PauseSound(false)
, OscVj("127.0.0.1", 12350, 12347)
, OscDmx("127.0.0.1", 12352, 12348)
, OscAnalyzer("127.0.0.1", 12353, 12354)
{
}

/******************************
******************************/
ofApp::~ofApp()
{
}

/******************************
******************************/
void ofApp::exit()
{
	/********************
	********************/
	ofxOscMessage m;
	m.setAddress("/Quit");
	m.addIntArg(1);
	
	OscVj.OscSend.sendMessage(m);
	OscDmx.OscSend.sendMessage(m);
	OscAnalyzer.OscSend.sendMessage(m);
	
	/********************
	********************/
	printf("\n> Good-bye\n");
}

//--------------------------------------------------------------
void ofApp::setup(){
	/********************
	各種基本設定
	********************/
	ofSetWindowTitle("Music Player");
	ofSetFrameRate(60);
	ofSetWindowShape(WIDTH, HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	font.loadFont("RictyDiminished-Regular.ttf", 15);
	
	/********************
	********************/
	sound.loadSound("music.mp3");
	if(!sound.isLoaded())	{ ERROR_MSG(); std::exit(1); }
	
	sound.setLoop(true);
	sound.setMultiPlay( true );
	// sound.setSpeed( 1.0 );
	sound.setVolume(1.0);
	
	sound.play();
}

//--------------------------------------------------------------
void ofApp::update(){
	ofxOscMessage m;
	m.setAddress("/MusicTime");
		
	if(sound.isPlaying()){
		/********************
		********************/
		ofSoundUpdate();
		now_ms = sound.getPositionMS();
		
		/********************
		********************/
		m.addIntArg(now_ms);
		
		
	}else{
		/********************
		********************/
		now_ms = 0;
		
		/********************
		********************/
		m.addIntArg(-1);
	}
	
	OscVj.OscSend.sendMessage(m);
	OscDmx.OscSend.sendMessage(m);
}

/******************************
******************************/
void ofApp::draw_time()
{
	/********************
	********************/
	char buf[BUF_SIZE];
	
	int min	= now_ms / 1000 / 60;
	int sec	= now_ms / 1000 - min * 60;
	int ms	= now_ms % 1000;
	
	sprintf(buf, "%6d:%6d:%6d", min, sec, ms);
	
	
	/********************
	********************/
	ofSetColor(255, 255, 255);
	
	font.drawString(buf, 10, 50);
	
	/*
	float w = font.stringWidth(buf);
	float h = font.stringHeight(buf);
	float x = ofGetWidth() / 2 - w / 2;
	float y = ofGetHeight() / 2 + h / 2;
	
	font.drawString(buf, x, y);
	*/
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(30);
	
	draw_time();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case 'a':
			b_PauseSound = !b_PauseSound;
			sound.setPaused(b_PauseSound);
			break;
			
		case 'p':
			b_PauseSound = false;
			sound.setPaused(b_PauseSound);
			
			if(!sound.isPlaying()){
				sound.play();
			}
			break;
			
		case 's':
			b_PauseSound = false;
			sound.setPaused(b_PauseSound);
			
			if(sound.isPlaying()){
				sound.stop();
			}
			break;
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
