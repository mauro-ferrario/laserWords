//
//  LaserWords.cpp
//  laserWords
//
//  Created by Mauro Ferrario on 13/02/16.
//
//

#include "LaserWords.h"

LaserWords::LaserWords()
{
  blackSpeed = 2;
}

void LaserWords::setup(int width, int height)
{
  setupWebSockets();
  setupFont();
  setupFBO(width, height);
}

void LaserWords::setupFont()
{
  font.load("Batang.ttf", 160, true, true, true);
}

void LaserWords::setupFBO(int width, int height)
{
  fbo.allocate(width, height, GL_RGBA32F_ARB);
  fbo.begin();
  ofClear(0,255);
  fbo.end();
}

void LaserWords::setupWebSockets()
{
  ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
  options.port = 9092;
  options.bUseSSL = false;
  bSetup = server.setup( options );
  server.addListener(this);
}

void LaserWords::update()
{
  fbo.begin();
  if(textToWrite != "")
  {
    writeNewText(textToWrite);
    textToWrite = "";
  }
  ofPushStyle();
  ofSetColor(0,blackSpeed);
  ofDrawRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
  ofPopStyle();
  fbo.end();
}

void LaserWords::writeNewText(string text)
{
  ofPushStyle();
  ofSetColor(255);
  ofNoFill();
  font.drawStringAsShapes(textToWrite, ofRandom(600), ofRandom(550));
  ofPopStyle();
}

void LaserWords::draw()
{
  fbo.draw(0,0);
}

void LaserWords::drawDebug()
{
  ofPushStyle();
  if(bSetup)
  {
    ofDrawBitmapString("WebSocket server setup at "+ofToString( server.getPort() ) + ( server.usingSSL() ? " with SSL" : " without SSL"), 20, 20);
    ofSetColor(150);
    ofDrawBitmapString("Click anywhere to open up client example", 20, 40);
  } else
  {
    ofDrawBitmapString("WebSocket setup failed :(", 20,20);
  }
  int x = 20;
  int y = 100;
  ofSetColor(0,150,0);
  ofDrawBitmapString("Console", x, 80);
  ofSetColor(255);
  for (int i = words.size() -1; i >= 0; i-- )
  {
    ofDrawBitmapString( words[i], x, y );
    y += 20;
  }
  if (words.size() > NUM_MESSAGES) words.erase( words.begin() );
  ofSetColor(150,0,0);
  ofDrawBitmapString("Type a message, hit [RETURN] to send:", x, ofGetHeight()-60);
  ofSetColor(255);
  ofPopStyle();
}

void LaserWords::onConnect(ofxLibwebsockets::Event& args)
{
  cout<<"on connected"<<endl;
}

void LaserWords::onOpen(ofxLibwebsockets::Event& args)
{
  cout<<"new connection open"<<endl;
  words.push_back("New connection from " + args.conn.getClientIP() + ", " + args.conn.getClientName() );
}

void LaserWords::onClose(ofxLibwebsockets::Event& args)
{
  cout<<"on close"<<endl;
  words.push_back("Connection closed");
}

void LaserWords::onIdle(ofxLibwebsockets::Event& args)
{
  cout<<"on idle"<<endl;
}

void LaserWords::onMessage(ofxLibwebsockets::Event& args)
{
  cout<<"got message "<<args.message<<endl;
  if ( !args.json.isNull() ){
    words.push_back("New message: " + args.json.toStyledString() + " from " + args.conn.getClientName() );
  } else {
    textToWrite = args.message;
    words.push_back("New message: " + args.message + " from " + args.conn.getClientName() );
  }
  args.conn.send(args.message);
}

void LaserWords::onBroadcast(ofxLibwebsockets::Event& args)
{
  cout<<"got broadcast "<<args.message<<endl;
}
