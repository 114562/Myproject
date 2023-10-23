/**
 * @file main.cpp
 * @brief 整个项目主程序
 * @author 杨径骁
 * @version 1.0
 * @date 2023-10-22
 *
 * @copyright Copyright 电子科技大学
 */

#include <iostream>

#include "mymenu.h"
using namespace std;

int main() {
    Menu m;
    m.run();
    std::cout << "hello world" << std::endl;
    return 0;
}
