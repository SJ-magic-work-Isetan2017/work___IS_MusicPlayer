/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxNetwork.h"
#include "ofxGui.h"


/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

enum{
	BUF_SIZE = 2000,
};


/************************************************************
************************************************************/


/**************************************************
**************************************************/
class OSC_SEND{
private:
	char IP[BUF_SIZE];
	int Port;

	ofxOscSender sender;
	
public:
	OSC_SEND()
	: Port(-1)
	{
	}
	OSC_SEND(const char* _IP, int _Port)
	{
		if(_Port != -1){
			sprintf(IP, "%s", _IP);
			Port = _Port;
			
			sender.setup(IP, Port);
		}
	}
	
	void setup(const char* _IP, int _Port)
	{
		if(_Port != -1){
			sprintf(IP, "%s", _IP);
			Port = _Port;
			
			sender.setup(IP, Port);
		}
	}
	void sendMessage(ofxOscMessage &message)
	{
		if(Port != -1){
			sender.sendMessage(message);
		}
	}
};

class OSC_RECEIVE{
private:
	int Port;
	ofxOscReceiver	receiver;
	
public:
	OSC_RECEIVE()
	: Port(-1)
	{
	}
	OSC_RECEIVE(int _Port)
	{
		if(_Port != -1){
			Port = _Port;
			receiver.setup(Port);
		}
	}
	
	void setup(int _Port)
	{
		if(_Port != -1){
			Port = _Port;
			receiver.setup(Port);
		}
	}
	
	bool hasWaitingMessages()
	{
		if(Port == -1){
			return false;
		}else{
			return receiver.hasWaitingMessages();
		}
	}
	
	bool getNextMessage(ofxOscMessage *msg)
	{
		if(Port == -1){
			return false;
		}else{
			return receiver.getNextMessage(msg);
		}
	}
};

class OSC_TARGET{
private:
public:
	OSC_SEND	OscSend;
	OSC_RECEIVE	OscReceive;
	
	OSC_TARGET()
	{
	}
	
	OSC_TARGET(const char* _SendTo_IP, int _SendTo_Port, int _Receive_Port)
	: OscSend(_SendTo_IP, _SendTo_Port), OscReceive(_Receive_Port)
	{
	}
	
	void setup(const char* _SendTo_IP, int _SendTo_Port, int _Receive_Port)
	{
		OscSend.setup(_SendTo_IP, _SendTo_Port);
		OscReceive.setup(_Receive_Port);
	}
};

/**************************************************
**************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
		WIDTH = 300,
		HEIGHT = 100,
	};
	
	enum STATE{
		STATE__BOOTCHECK_PLAY,
		
		STATE__PLAY,
		STATE__STOP,
	};
	
	enum BOOTMODE{
		BOOTMODE_INSTALLATION,
		BOOTMODE_DEMO,
	};
	
	/****************************************
	****************************************/
	STATE State;
	int counter;
	
	ofTrueTypeFont font;
	
	ofSoundPlayer sound;
	int now_ms;
	bool b_PauseSound;
	
	bool b_ScheduleMode;
	STATE FirstState;
	
	/********************
	********************/
	OSC_TARGET OscVj;
	OSC_TARGET OscDmx;
	OSC_TARGET OscAnalyzer;
	
	/****************************************
	****************************************/
	void draw_time();
	void draw_NowStopping();
	
	bool Transition_ifTimeToClose();
	bool Transition_ifTimeToOpen();
	void Process_OpenToClose();
	void Process_CloseToOpen();
	
	void UpdateSound_SendMusicTime();

public:
	/****************************************
	****************************************/
	ofApp();
	~ofApp();
	void exit();
	
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
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
	
};
