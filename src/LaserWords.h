//
//  LaserWords.h
//  laserWords
//
//  Created by Mauro Ferrario on 13/02/16.
//
//

#ifndef __laserWords__LaserWords__
#define __laserWords__LaserWords__

#include  "ofMain.h"
#include "ofxLibwebsockets.h"

#define NUM_MESSAGES 30 // how many past messages we want to keep

class LaserWords
{
public:
                            LaserWords();
  void                      setup(int width, int height);
  void                      update();
  void                      draw();
  void                      drawDebug();
  void                      onConnect(ofxLibwebsockets::Event& args);
  void                      onOpen(ofxLibwebsockets::Event& args);
  void                      onClose(ofxLibwebsockets::Event& args);
  void                      onIdle(ofxLibwebsockets::Event& args);
  void                      onMessage(ofxLibwebsockets::Event& args);
  void                      onBroadcast(ofxLibwebsockets::Event& args);
  
private:
  void                      writeNewText(string text);
  void                      setupWebSockets();
  void                      setupFont();
  void                      setupFBO(int width, int height);
  ofxLibwebsockets::Server  server;
  bool                      bSetup;
  vector<string>            words;
  ofFbo                     fbo;
  ofTrueTypeFont            font;
  string                    textToWrite;
  ofParameter<float>        blackSpeed;
};

#endif /* defined(__laserWords__LaserWords__) */
