#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
  ofBackground(0);
  ofSetFrameRate(30);
  words.setup(1920,1080);
  gui.setup();
  guiVisible = true;
  gui.add(*words.getLaserWordsParams());
}

//--------------------------------------------------------------
void ofApp::update()
{
  words.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
  words.draw();
  if(guiVisible)
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  if(key == 'g')
    guiVisible = !guiVisible;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
  
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
  
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
//  string url = "http";
//  if ( server.usingSSL() ){
//    url += "s";
//  }
//  url += "://localhost:" + ofToString( server.getPort() );
//  ofLaunchBrowser(url);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
  
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