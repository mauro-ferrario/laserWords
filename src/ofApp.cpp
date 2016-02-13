#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
  ofBackground(0);
  ofSetFrameRate(60);
  words.setup(1024,768);
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
  words.drawDebug();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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