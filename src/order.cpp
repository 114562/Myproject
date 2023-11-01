#include "order.h"

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>

#include <cstdlib>
#include <cstring>
#include <fstream>

const std::string prefix_path = "../resources/";
const std::string data_prefix = "../DataBase/";

void Order::open_and_print(const std::string path) {
    // 打开文件
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

void Order::clear() {
    // std::system("clear");
    command.clear();
    m_command_type = Unknown;
}

void Order::set_command(const std::string& command) {
    this->command = command;
}

void Order::run() {
    // 判断命令类型
    command_t type = _getCommandType();
    switch (type) {
    case Show:
        _deal_show();
        break;
    case Tree:
        _deal_tree();
        break;
    case Clear:
        _deal_clear();
        break;
    case Quit:
        _deal_quit();
        break;
    case Create_Database:
        _deal_createDB();
        break;
    case Drop_Database:
        _deal_dropDB();
        break;
    case Use:
        _deal_changeDB();
        break;
    case Create_Table:
        _deal_createTable();
        break;
    case Delete:
        _deal_deleteTable();
        break;
    case Insert:
        _deal_insertTable();
        break;
    case Select:
        _deal_selectTable();
        break;
    case Update:
        _deal_updateTable();
        break;
    case Unknown:
        _deal_unknown();
        break;
    }
}

Order::command_t Order::_getCommandType() {
    if (command == "q" || command == "quit") {
        return command_t::Quit;
    } else if (command == "show") {
        return command_t::Show;
    } else if (command == "tree" || strncmp(command.c_str(), "tree", 4) == 0) {
        return command_t::Tree;
    } else if (command == "clear") {
        return command_t::Clear;
    } else if (strncmp(command.c_str(), "create database", 15) == 0) {
        return command_t::Create_Database;
    } else if (strncmp(command.c_str(), "drop database", 13) == 0) {
        return command_t::Drop_Database;
    } else if (strncmp(command.c_str(), "use", 3) == 0) {
        return command_t::Use;
    } else if (strncmp(command.c_str(), "create table", 12) == 0) {
        return command_t::Create_Table;
    } else if (strncmp(command.c_str(), "delete from", 11) == 0) {
        return command_t::Delete;
    } else if (strncmp(command.c_str(), "insert into", 11) == 0) {
        return command_t::Insert;
    } else if (strncmp(command.c_str(), "select", 6) == 0) {
        return command_t::Select;
    } else if (strncmp(command.c_str(), "update", 6) == 0) {
        return command_t::Update;
    } else {
        return command_t::Unknown;
    }
}

void Order::_deal_show() {
    open_and_print(prefix_path + "menu_start.txt");
}

void Order::_deal_clear() {
    system("clear");
}

void Order::_deal_quit() {
    // 退出
    open_and_print(prefix_path + "menu_end.txt");
    exit(0);
}

void Order::_deal_tree() {
    // std::cout << "数据库的树形目录结构:" << std::endl;
    // 先完成一个简单的搜索
    // 记录数据库的名字
    std::string dbName = _deal_findName(4);
    if (dbName == "") {
        if (m_db_name == "") {
            // 默认使用 0921
            dbName = "0921";
        } else {
            dbName = m_db_name;
        }
    }

    // 找到数据库开始位置的前一位
    // size_t pos = command.find(' ');  // 使用STL封装的find
    // if (std::string::npos != pos) {
    //     // 找到空格 说明有数据库名
    //     dbName = command.substr(pos + 1, command.size() - pos);
    // }
    // std::string path = data_prefix + dbName;
    // if (access(path.c_str(), F_OK) == -1) {
    //     std::cout << "数据库 " << dbName << "不存在" << std::endl;
    // } else {
    //     std::cout << dbName << "数据库目录架构如图所示:" << std::endl;
    //     execlp("tree", "tree", path.c_str(), nullptr);
    // }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        // 子进程
        std::string path = data_prefix + dbName;
        if (access(path.c_str(), F_OK) == -1) {
            // 不存在这个路径
            std::cout << "数据库 " << dbName << "不存在" << std::endl;
        } else {
            std::cout << dbName << "数据库目录架构如图所示:" << std::endl;
            execlp("tree", "tree", path.c_str(), nullptr);
        }
    } else if (pid > 0) {
        // 父进程
        int ret = waitpid(-1, nullptr, 0);
        if (ret == -1) {
            perror("waitpid");
            exit(1);
        }
    }
}

void Order::_deal_unknown() {
    m_db_name.clear();
    m_command_type = command_t::Unknown;
    std::cout << "命令不存在,请重新输入!" << std::endl;
}

void Order::_deal_createDB() {
    // 创建database的实质就是在根目录下创建一个文件夹
    // std::cout << "创建数据库" << std::endl;

    // 找出数据库的名字 形式为:create database xxx
    // std::string dbName = command.substr(15, command.size() - 15);
    // int pos = 0;
    // int n = dbName.size();
    // while (pos < n) {
    //     if (dbName[pos] != ' ') {
    //         break;
    //     }
    //     pos++;
    // }
    // int idx1 = pos;
    // pos = n - 1;
    // while (pos >= 0) {
    //     if (dbName[pos] != ' ') {
    //         break;
    //     }
    //     pos--;
    // }
    // int idx2 = pos;
    // dbName = dbName.substr(idx1, idx2 - idx1 + 1);
    std::string dbName = _deal_findName(15);
    // 创建数据库
    std::string path = data_prefix + dbName;

    // 设置文件权， 这里设置为所有权限
    int status = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    if (status == 0) {
        std::cout << "数据库" << dbName << "创建成功" << std::endl;
    } else {
        perror("数据库创建失败");
        exit(1);
    }
}

void Order::_deal_dropDB() {
    // std::cout << "删除数据库" << std::endl;
    // 找出数据库的名字 形式为:frop database xxx
    // std::string dbName = command.substr(13, command.size() - 13);
    // int pos = 0;
    // int n = dbName.size();
    // while (pos < n) {
    //     if (dbName[pos] != ' ') {
    //         break;
    //     }
    //     pos++;
    // }
    // int idx1 = pos;
    // pos = n - 1;
    // while (pos >= 0) {
    //     if (dbName[pos] != ' ') {
    //         break;
    //     }
    //     pos--;
    // }
    // int idx2 = pos;
    // dbName = dbName.substr(idx1, idx2 - idx1 + 1);
    std::string dbName = _deal_findName(13);
    std::string path = data_prefix + dbName;
    int ret = access(path.c_str(), F_OK);
    if (ret == 0) {
        int status = rmdir(path.c_str());
        if (status != 0) {
            perror("删除数据库失败");
            exit(1);
        } else {
            std::cout << "数据库" << dbName << "被删除!" << std::endl;
        }
    } else {
        std::cout << "没有对应数据库,删除失败!" << std::endl;
    }
}

void Order::_deal_changeDB() {
    std::string dbName = _deal_findName(3);
    m_db_name = dbName;
    std::string path = data_prefix + dbName;
    int ret = access(path.c_str(), F_OK);
    if (ret != 0) {
        std::cout << "数据库不存在,请重新输入!" << std::endl;
    } else {
        std::cout << "数据库切换成功,当前使用数据库名称:" << dbName << std::endl;
    }
}

void Order::_deal_createTable() {
    dml.deal_createTable_command(command, m_db_name);
}

void Order::_deal_deleteTable() {
    dml.deal_deleteTable(command, m_db_name);
}

void Order::_deal_insertTable() {
    dml.deal_InsertTable_command(command, m_db_name);
}

void Order::_deal_selectTable() {
    dml.deal_selectTable(command, m_db_name);
}

std::string Order::_deal_findName(int n) {
    std::string dbName = command.substr(n, command.size() - n);
    int pos = 0;
    int size = dbName.size();
    while (pos < size) {
        if (dbName[pos] != ' ') {
            break;
        }
        pos++;
    }
    int idx1 = pos;
    pos = size - 1;
    while (pos >= 0) {
        if (dbName[pos] != ' ') {
            break;
        }
        pos--;
    }
    int idx2 = pos;
    return dbName.substr(idx1, idx2 - idx1 + 1);
}

void Order::_deal_updateTable() {
    dml.deal_updateTable(command, m_db_name);
}