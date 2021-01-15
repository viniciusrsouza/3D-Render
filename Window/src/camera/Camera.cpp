#include "Camera.h"
#include <iostream>
#include <sstream>

Camera::Camera(): hx(0), hy(0), d(0) {}

float** Camera::getChangeBasisMatrix() {
	float** x = new float* [3];
	x[0] = new float[3]{ U.x, U.y, U.z };
	x[1] = new float[3]{ V.x, V.y, V.z };
	x[2] = new float[3]{ N.x, N.y, N.z };
	return x;
}

std::string Camera::str() const {
	std::stringstream sstm;
	sstm <<
		"Camera: " << std::endl <<
		"C: " << this->C.str() << std::endl <<
		"N: " << this->N.str() << std::endl <<
		"V: " << this->V.str() << std::endl <<
		"U: " << this->U.str() << std::endl <<
		"hx: " << this->hx << std::endl <<
		"hy: " << this->hy << std::endl <<
		"d: " << this->d << std::endl
		<< "---";
	return sstm.str();
}