#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include "FileLoader.h"
#include <Vector3D.h>

#define LOG true

#ifdef _DEBUG

void LoadFile(std::string filename, void* callback) {
	std::cout << "Loading: " << filename << std::endl;
	std::thread thread(_LoadFileAsync, filename, callback);
	thread.detach();
}

void _LoadFileAsync(std::string filename, void* callback) {
	std::ifstream file(filename);
	int vertex_count, triangle_count;

	file >> vertex_count >> triangle_count;

	// carregando vertices
	MathLib::Vector3D* list_of_vertex = new MathLib::Vector3D[vertex_count];

	double x, y, z;
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
		triangles[i] = triangle;

		#if LOG
		std::cout << i << ": " << triangle[0] << " " << triangle[1] << " " << triangle[2] << std::endl;
		#endif
	}



	std::cout << "Finished file loading" << std::endl;
}

#endif