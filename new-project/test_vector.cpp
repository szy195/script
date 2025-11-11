#include "Utils/Vector.h"
#include <iostream>

int main() {
    Vector2 v1(1.0f, 2.0f);
    Vector2 v2(3.0f, 4.0f);
    Vector2 result = v1 + v2;
    std::cout << "Vector test: " << result.ToString() << std::endl;
    return 0;
}