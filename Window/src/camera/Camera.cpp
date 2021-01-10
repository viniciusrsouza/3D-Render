#include "Camera.h"
#include <iostream>
#include <sstream>

Camera::Camera(): hx(0), hy(0), d(0) {}

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