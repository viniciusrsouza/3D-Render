// DebugConsole.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <Vector3D.h>

using namespace MathLib;

int main()
{
    std::string str;
    int x, y, z;
    std::cout << "Inicialize o vetor." << std::endl << "x> ";
    std::cin >> x;
    std::cout << "y> ";
    std::cin >> y;
    std::cout << "z> ";
    std::cin >> z;

    Vector3D vector(x, y, z);

    std::cout << "Vetor> " << vector.str() << std::endl;

    std::cout << "Multiplicação por escalar: ";
    while (true) {
        int k;
        std::cin >> k;
        std::cout << vector.str() << " * " << k << "> ";
        vector = vector * k;
        std::cout << vector.str() << std::endl << std::endl << std::endl;
    }
}