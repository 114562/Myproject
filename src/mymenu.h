/**
 * @file menu.h
 * @author 杨径骁 (2372256530@qq.com)
 * @brief 菜单类的头文件
 * @version 1.0
 * @date {2023.10.22}
 * @copyright Copyright (c) 2023 电子科技大学
 */

#ifndef _MENU_H
#define _MENU_H
#include <cstring>
#include <exception>
#include <iostream>
#include <string>

#include "order.h"

/**
 * @brief 菜单类
 */
class Menu {
public:
    /**
     * @brief 默认构造函数
     */
    Menu() = default;

    /**
     * @brief 默认析构函数
     */
    virtual ~Menu() = default;

public:
    /**
     * @brief 菜单创建之后执行
     */
    void run();

    /**
     * @brief 打印信息至屏幕
     * @param path 要打开的文件路径
     */
    void open_and_print(const std::string path);

private:
    /**
     * @brief  让用户输入命令，并存储在类内
     * @return 返回一个bool值来表示用户是否输入了命令的结束,程序需要退出
     */
    bool _command_input();

private:
    /**
     * @brief 维护一个命令管理order对象，以后的命令都是由这个函数进行操作
     *
     */
    Order order;
};
#endif