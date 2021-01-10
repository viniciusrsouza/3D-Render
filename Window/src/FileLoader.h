#ifndef __FILE_LOADER_H__
#define __FILE_LOADER_H__

#include <Vector3D.h>
#include "camera/Camera.h"

void LoadFile(std::string, void(*f)(int, MathLib::Vector3D*, int, int**, Camera));
void _LoadFileAsync(std::string, void(*f)(int, MathLib::Vector3D*, int, int**, Camera));
#endif