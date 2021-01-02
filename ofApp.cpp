#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofEnableDepthTest();

	this->noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

	for (int x = -500; x <= 500; x += 50) {

		for (int y = -500; y <= 500; y += 50) {

			this->location_list.push_back(glm::vec3(x, y, 0));
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int span = 180;
	this->mesh.clear();

	if (ofGetFrameNum() % span == 0) {

		for (int i = 0; i < this->location_list.size(); i += 1) {

			vector<glm::vec3> log;
			log.push_back(this->location_list[i]);
			this->log_list.push_back(log);
			this->color_list.push_back(ofColor(ofRandom(39, 139)));
			this->life_list.push_back(span);
			glm::vec3 param;
			int p = ofRandom(3);
			if (p < 1) param = glm::vec3(15, 0, 0);
			if (p >= 1 && p < 2) param = glm::vec3(0, 15, 0);
			if (p >= 2 && p < 3) param = glm::vec3(0, 0, 15);
			this->param_list.push_back(param);
		}
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);
			this->param_list.erase(this->param_list.begin() + i);

			continue;
		}

		auto x = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.001, this->noise_seed.x + ofGetFrameNum() * 0.008)), 0, 1, -100, 100);
		auto y = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.001, this->noise_seed.y + ofGetFrameNum() * 0.008)), 0, 1, -100, 100);
		auto z = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.001, this->noise_seed.z + ofGetFrameNum() * 0.008)), 0, 1, 0, 20);
		this->log_list[i].push_back(this->log_list[i].back() + glm::vec3(x, y, z));
	}

	for (int i = 0; i < this->log_list.size(); i++) {

		//auto alpha = this->life_list[i] > 60 ? 255 : ofMap(this->life_list[i], 0, 60, 0, 255);
		for (int k = 0; k < this->log_list[i].size(); k++) {

			int index = this->mesh.getNumVertices();

			this->mesh.addVertex(this->log_list[i][k] + this->param_list[i]);
			this->mesh.addVertex(this->log_list[i][k] - this->param_list[i]);
			this->mesh.addColor(ofColor(this->color_list[i]));
			this->mesh.addColor(ofColor(this->color_list[i]));

			if (k > 0) {

				this->mesh.addIndex(index + 0); this->mesh.addIndex(index - 1); this->mesh.addIndex(index - 2);
				this->mesh.addIndex(index + 0); this->mesh.addIndex(index + 1); this->mesh.addIndex(index - 1);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateZ(ofGetFrameNum() * 0.5);

	this->mesh.drawFaces();

	ofDrawBox(glm::vec3(0, 0, -150), 1000, 1000, 300);

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}