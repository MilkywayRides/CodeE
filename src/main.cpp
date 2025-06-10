#include "editor.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        Editor editor;
        editor.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}