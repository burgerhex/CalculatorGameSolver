#include <iostream>
#include "Solver.h"

int main() {
    Solver s("level.txt");
    for (Operation* o : s.solve()) {
        std::cout << o->to_string() << std::endl;
    }
}
