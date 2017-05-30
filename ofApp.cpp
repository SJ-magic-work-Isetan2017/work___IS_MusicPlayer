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
, State(STATE__BOOTCHECK_PLAY)
, counter(10) // 十分大きな数. IsTimeToClose()に最初に入った時、余計な処理をしないように.
, b_ScheduleMode(false)
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
	ofSetFrameRate(30);
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
	if(State == STATE__BOOTCHECK_PLAY){
		if(b_ScheduleMode){
			if(FirstState == STATE__STOP){
				counter = 0;
				Process_OpenToClose();
				
				/* if(FirstState == STATE__PLAY):just keep playing. */
			}
			State = FirstState;
			
		}else{
			UpdateSound_SendMusicTime();
		}
	}else{
		if(State == STATE__PLAY){
			if(Transition_ifTimeToClose()){
				/* 遷移時の処理も関数の向こう側で完了してある. */
			}else{
				UpdateSound_SendMusicTime();
			}
			
		}else if(State == STATE__STOP){
			if(Transition_ifTimeToOpen()){
				/* 遷移時の処理も関数の向こう側で完了してある. */
			}
		}
	}
}

/******************************
******************************/
void ofApp::UpdateSound_SendMusicTime()
{
	ofxOscMessage m;
	m.setAddress("/MusicTime");
		
	if(sound.isPlaying()){
		ofSoundUpdate();
		now_ms = sound.getPositionMS();
		
		m.addIntArg(now_ms);
				
	}else{
		now_ms = 0;
		m.addIntArg(-1);
	}
	
	OscVj.OscSend.sendMessage(m);
	OscDmx.OscSend.sendMessage(m);
}

/******************************
******************************/
bool ofApp::Transition_ifTimeToClose()
{
	int s = ofGetSeconds();
	int m = ofGetMinutes();
	int h = ofGetHours();
	
	if( (h == 20) && (m == 5) ){
		State = STATE__STOP;
		counter = 0;
		Process_OpenToClose();
		
		return true;
		
	}else{
		if(counter < 3){
			counter++;
			Process_CloseToOpen();
		}
		return false;
	}
}

/******************************
******************************/
bool ofApp::Transition_ifTimeToOpen()
{
    int s = ofGetSeconds();
    int m = ofGetMinutes();
    int h = ofGetHours();
	
	// if( (h == 10) && (m == 25) ){
	if( (h == 15) && (m == 43) ){
		State = STATE__PLAY;
		counter = 0;
		Process_CloseToOpen();
		
		return true;
		
	}else{
		if(counter < 3){
			counter++;
			Process_OpenToClose();
		}
		return false;
	}
}

/******************************
******************************/
void ofApp::Process_OpenToClose()
{
	/********************
	********************/
	ofxOscMessage m;
	
	m.setAddress("/Quit");
	m.addIntArg(1);
	
	OscVj.OscSend.sendMessage(m);
	OscDmx.OscSend.sendMessage(m);
	
	/********************
	********************/
	if(sound.isPlaying()) sound.stop();
}

/******************************
******************************/
void ofApp::Process_CloseToOpen()
{
	if(!sound.isPlaying()) sound.play();
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

/******************************
******************************/
void ofApp::draw_NowStopping()
{
	/********************
	********************/
	ofSetColor(255, 255, 255);
	
	font.drawString("Now Stopping", 10, 50);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(30);
	
	if( (State == STATE__PLAY) || (State == STATE__BOOTCHECK_PLAY))	draw_time();
	else															draw_NowStopping();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case '0':
			b_ScheduleMode = true;
			FirstState = STATE__STOP;
			break;
			
		case '1':
			b_ScheduleMode = true;
			FirstState = STATE__PLAY;
			break;
		
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
