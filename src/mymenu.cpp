/**
 * @file mymenu.cpp
 * @author 杨径骁 (2372256530@qq.com)
 * @brief 菜单类逻辑实现
 * @version 1.0
 * @date {2023.10.22}
 *
 * @copyright Copyright (c) 2023 电子科技大学
 *
 */

#include <mymenu.h>

#include <fstream>
/**
 * @brief 对头文件中定义的函数实现
 */
const std::string prefix_path = "../resources/";

void Menu::run() {
    // 在../resources目录下存放着，menu.txt,里面的内容就是数据库系统进入之后的提示内容
    open_and_print(prefix_path + "menu_start.txt");

    // 读入命令
    while (1) {
        if (_command_input()) {
            // 输入命令结束
            break;
        }
    }
    // 结束 打印结束消息
    open_and_print(prefix_path + "menu_end.txt");
}

void Menu::open_and_print(const std::string path) {
    // 打开文件
    // FILE* file = fopen(path.c_str(), "r");
    // if (nullptr == file) {
    //     perror("fopen");
    //     exit(-1);
    // }

    // // 读取内容
    // char read_buf[BUFSIZ] = {0};  // 数组容量用系统默认给的缓冲区大小 8192
    // while (1) {
    //     bzero(read_buf, BUFSIZ);
    //     size_t len = fread(read_buf, 1, BUFSIZ - 1, file);
    //     // 返回0表明可能出错或者读到末尾了
    //     if (0 == len) {
    //         if (ferror(file)) {
    //             perror("fread");
    //             exit(-1);
    //         }
    //         if (feof(file))
    //             break;
    //     }

    //     // 打印到屏幕上
    //     fwrite(read_buf, 1, len, stdout);
    // }

    // // 关闭
    // fclose(file);

    std::ifstream file;
    file.open(path, std::ios::out);
    if (!file.is_open()) {
        perror("ifstream");
        exit(-1);
    }
    // 将内容输出至stdout
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}

bool Menu::_command_input() {
    puts("请输入命令: ");  // puts()自带换行
    // 一个一个读入字符，如果遇到 ; 则结束，将得到的命令存储在command中,等待命令输入结束后在Order中进行设置
    order.clear();

    // 存取命令行输入的命令
    std::string command;

    while (1) {
        int ch = fgetc(stdin);
        // 不允许缩进'\t'和回车'\n'出现，若出现替换为' '
        if (ch == '\t' || ch == '\n') {
            ch = ' ';
        }
        // 当字符串为空且输入为空时被忽略
        if (command.empty() && ch == ' ') {
            continue;
        }
        if (ch == ';') {
            // 分号代表输入结束
            break;
        }
        command += ch;
    }
    // 需要判断命令的最后一个字符是否为空格，若是则需要去掉
    if (command.back() == ' ') {
        command.pop_back();
    }
    // 命令结束之后将Order中的命令字符串修改
    order.set_command(command);

    // 执行命令
    order.run();
    puts("");
    // 这里的true和false是针对前面退出程序的判断来的，没有实际的逻辑含义
    return false;
}
