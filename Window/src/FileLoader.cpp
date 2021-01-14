#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include "FileLoader.h"
#include "camera/Camera.h"
#include <Vector3D.h>

#define LOG true

void LoadFile(std::string filename, void(*callback)(int, MathLib::Vector3D*, int, int**, Camera)) {
	std::cout << "Loading: " << filename << std::endl;
	std::thread thread(_LoadFileAsync, filename, callback);
	thread.detach();
}

void swap(int* arr, int i, int j) {
	int aux = arr[i];
	arr[i] = arr[j];
	arr[j] = aux;
}

void sort(MathLib::Vector3D* vertex, int i, int j, int k, int* triangle) {
	if (vertex[triangle[2]].y > vertex[triangle[1]].y) {
		swap(triangle, 2, 1);
	}
	if (vertex[triangle[1]].y > vertex[triangle[0]].y) {
		swap(triangle, 1, 0);
	}
	if (vertex[triangle[2]].y > vertex[triangle[1]].y) {
		swap(triangle, 2, 1);
	}
}

void _LoadFileAsync(std::string filename, void(*callback)(int, MathLib::Vector3D*, int, int**, Camera)) {
	std::ifstream file(filename);
	int vertex_count, triangle_count;

	file >> vertex_count >> triangle_count;

	// carregando vertices
	MathLib::Vector3D* list_of_vertex = new MathLib::Vector3D[vertex_count];

	float x, y, z;
	for (int i = 0; i < vertex_count; i++) {
		file >> x >> y >> z;
		list_of_vertex[i] = MathLib::Vector3D(x, y, z);

		#if LOG
		std::cout << i << ": " << list_of_vertex[i].str() <<std::endl;
		#endif
	}

	// carregando triangulos
	std::cout << triangle_count << std::endl;
	int** triangles = new int*[triangle_count];
	for (int i = 0; i < triangle_count; i++) {
		int* triangle = new int[3];
		file >> triangle[0] >> triangle[1] >> triangle[2];
		triangle[0]--;
		triangle[1]--;
		triangle[2]--;
		sort(list_of_vertex, triangle[0], triangle[1], triangle[2], triangle);
		triangles[i] = triangle;

		#if LOG
		std::cout << i << ": " << triangle[0] << " " << triangle[1] << " " << triangle[2] << std::endl;
		#endif
	}
	file.close();
	
	file = std::ifstream("../input/camera.in");
	// carregando camera
	Camera c;
	file >> x >> y >> z;
	c.N = MathLib::Vector3D(x, y, z);

	file >> x >> y >> z;
	c.V = MathLib::Vector3D(x, y, z);

	file
		>> c.d
		>> c.hx
		>> c.hy;

	file >> x >> y >> z;
	c.C = MathLib::Vector3D(x, y, z);

	std::cout << c.str() << std::endl;

	std::cout << "Finished file loading" << std::endl;

	callback(vertex_count, list_of_vertex, triangle_count, triangles, c);
}