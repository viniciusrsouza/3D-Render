// DebugConsole.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <Vector2D.h>

using namespace MathLib;

int main()
{
    std::string str;
    int x, y;
    std::cout << "Inicialize o vetor." << std::endl << "x> ";
    std::cin >> x;
    std::cout << "y> ";
    std::cin >> y;

    Vector2D vector(x, y);

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