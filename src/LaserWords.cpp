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
  blur.setup(width, height);
}

void LaserWords::setupFont()
{
  font.load("leaguespartan.ttf", 160, true, true, true);
}

void LaserWords::setupFBO(int width, int height)
{
  fbo.allocate(width, height, GL_RGBA32F_ARB);
  fbo.begin();
  ofClear(0,255);
  fbo.end();
}

bool LaserWords::inInsideVector(string textToFind)
{
  int size = wordsToRemove.size();
  for(int a = 0; a < size; a++)
  {
    if(textToFind == wordsToRemove[a])
    {
      return true;
    }
  }
  return false;
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
  blur.update(fbo, ofVec2f(blurValue, blurValue));
  fbo.begin();
  if(newWordsToWrite.size() > 0)
  {
    for(int a = 0; a < newWordsToWrite.size(); a++)
    {
      if(!inInsideVector(newWordsToWrite[a]))
      {
        writeNewText(newWordsToWrite[a]);
        wordsToRemove.push_back(newWordsToWrite[a]);
      }
    }
    newWordsToWrite.clear();
  }
  
  checkWordToRemove();
  ofPushStyle();
  ofSetColor(0,blackSpeed);
  ofDrawRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
  ofPopStyle();
  fbo.end();
}

void LaserWords::checkWordToRemove()
{
  if(totSecondRemoveWordsFromList > 0)
  {
    if(ofGetFrameNum()%(totSecondRemoveWordsFromList*60) == 0)
    {
      if (wordsToRemove.size() > 0)
        wordsToRemove.erase(wordsToRemove.begin());
    }
  }
  else
  {
    if (wordsToRemove.size() > 0)
      wordsToRemove.erase(wordsToRemove.begin());
  }
  
}

void LaserWords::writeNewText(string text)
{
  ofPushStyle();
  ofSetColor(color);
  if(!fill)
  {
    ofNoFill();
    ofSetLineWidth(border);
  }
  int desiredWidth = ofRandom(100, ofGetWindowWidth());
  ofRectangle boundingBoxOriginal = font.getStringBoundingBox(text,0, 0);
  float prop = float(boundingBoxOriginal.width/boundingBoxOriginal.height);
  int actualSize = font.getSize();
  float newSize = float(desiredWidth/boundingBoxOriginal.width);
  ofVec2f newBoundingBoxSize;
  newBoundingBoxSize.x = boundingBoxOriginal.width * newSize;
  newBoundingBoxSize.y = boundingBoxOriginal.height * newSize;
  ofPushMatrix();
  ofVec2f newPos = getRandomPos(newBoundingBoxSize.x, newBoundingBoxSize.y);
  ofTranslate(newPos.x, newPos.y);
  ofScale(newSize, newSize);
  font.drawStringAsShapes(text, 0,boundingBoxOriginal.height);
  ofPopMatrix();
  ofPopStyle();
}

ofVec2f LaserWords::getRandomPos(int width, int height)
{
  ofVec2f newPos;
  newPos.x = ofRandom(0, ofGetWindowWidth()- width);
  newPos.y = ofRandom(0, ofGetWindowHeight()- height);
  return newPos;
}

void LaserWords::draw()
{
//  blur.draw();
  fbo.draw(0,0);
  if(bDrawDebug)
    drawDebug();
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
    newWordsToWrite = ofSplitString(ofToUpper(args.message), " ");
    words.push_back("New message: " + args.message + " from " + args.conn.getClientName() );
  }
  args.conn.send(args.message);
}

void LaserWords::onBroadcast(ofxLibwebsockets::Event& args)
{
  cout<<"got broadcast "<<args.message<<endl;
}

ofParameterGroup* LaserWords::getLaserWordsParams()
{
  if(!laserWordsParams)
  {
    laserWordsParams = new ofParameterGroup();
  }
  if(laserWordsParams->getName() == "")
  {
    laserWordsParams->setName("LaserWords");
    laserWordsParams->add(bDrawDebug.set("Draw debug", false));
    laserWordsParams->add(fill.set("Fill", false));
    laserWordsParams->add(border.set("Border", 0, 0, 255));
    laserWordsParams->add(blurValue.set("Blur", 0, 0, 2));
    laserWordsParams->add(blackSpeed.set("Black speed", 0, 0, 2));
    laserWordsParams->add(color.set("Color", ofColor(0), ofColor(0), ofColor(255)));
    laserWordsParams->add(totSecondRemoveWordsFromList.set("totSecondRemoveWordsFromList", 1, 0, 100));
    
  }
  return laserWordsParams;
}
