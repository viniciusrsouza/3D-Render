#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__
#ifdef _DEBUG

#include <Vector3D.h>

class Camera {
public:
	double hx, hy, d;
	MathLib::Vector3D U, V, N, C;

	Camera();

	std::string str() const;
};
#endif
#endif