#include "dml.h"

#include <dirent.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

const std::string table_prefix = "../DataBase/";

void DML::deal_createTable(const std::string command, const std::string dbName) {
    // std::cout << command << "创建表" << std::endl;
    // 处理信息
    // Table table;
    // table.name = command;
    Table table;
    _deal_command(command, table);
    // 得到数据之后,创建数据库
    // 增加表的后缀
    std::string table_name = table.name + ".dat";
    std::string fileName = table_prefix + dbName + "/" + table_name;

    std::ofstream outputFile(fileName, std::ios::binary);
    if (!outputFile.is_open()) {
        perror("ifstream");
        exit(1);
    }
    int numOfColumns = table.columns.size();
    // 写入列信息
    for (const Column& column : table.columns) {
        outputFile << column.name << "(" << column.type << ')' << '\t';
    }
    outputFile << '\n';

    // 关闭数据流文件并给出提示信息
    std::cout << table.name << "表创建成功" << std::endl;
    outputFile.close();
}

void DML::_deal_command(std::string command, Table& table) {
    // 清理输入，去除额外的空格和换行符
    command = std::regex_replace(command, std::regex("\\s+"), " ");

    // 正则表达式来匹配表名和列信息
    // create table student(name(string),age(int))
    /*
        create table：字面匹配 "create table"。
    ([a-zA-Z_]+)：这部分匹配表名，表名由一个或多个大小写字母（a-z, A-Z）或下划线（_）字符组成。圆括号表示捕获组，用于提取匹配到的表名。
    \\(：这部分匹配左圆括号 "("，因为 "(" 是正则表达式的特殊字符，所以需要用 "\" 转义。
    ([^]+)：这部分匹配列信息，包括列名和列类型。[^]+ 表示匹配一个或多个字符。圆括号表示捕获组，用于提取匹配到的列信息。
    \\)：这部分匹配右圆括号 ")"，同样需要用 "\" 转义
    */
    std::regex tablePattern("create table ([a-zA-Z_]+)\\(([^]+)\\)");
    std::smatch matches;

    if (std::regex_search(command, matches, tablePattern)) {
        // 提取表名
        std::string tableName = matches[1];
        // 检查是否获取正确表名
        //  std::cout << "Table Name: " << tableName << std::endl;
        table.name = tableName;

        std::string columns = matches[2];
        std::regex columnPattern("([a-zA-Z_]+)\\(([^)]+)\\)");
        std::smatch columnMatches;
        // 逐行提取信息
        while (std::regex_search(columns, columnMatches, columnPattern)) {
            std::string columnName = columnMatches[1];
            std::string columnType = columnMatches[2];
            Column col;
            col.name = columnName;
            col.type = columnType;
            table.columns.push_back(col);
            // 移到下一个列
            columns = columnMatches.suffix().str();
        }

        // 检测是否正确读取
        // for (const auto& info : table.columns) {
        //     std::cout << "Column Name: " << info.name << ", Column Type: " << info.type << std::endl;
        // }
    } else {
        std::cout << "字符串格式错误,请参考 create table <table-name> (<column> <type> [primary],...)" << std::endl;
    }
}

void DML::deal_deleteTable(const std::string command, const std::string dbName) {
    if (dbName == "") {
        std::cout << "未指定数据库!" << std::endl;
        return;
    }
    // 使用正则表达式取出表名 delete from student where name = "yjx";

    size_t wherePos = command.find("where");
    if (wherePos == std::string::npos) {
        // 没有where限制符,删除整个表
        std::string path = table_prefix + dbName + "/";
        // 获取表名
        std::string table_name = command.substr(12, command.size() - 12);
        int pos = 0;
        while (pos < table_name.size()) {
            if (table_name[pos] != ' ') {
                break;
            }
            pos++;
        }
        int idx1 = pos;
        pos = table_name.size() - 1;
        while (pos) {
            if (table_name[pos] != ' ') {
                break;
            }
            pos--;
        }
        int idx2 = pos;
        table_name = table_name.substr(idx1, idx2 - idx1 + 1);
        path = path + table_name + ".dat";
        // std::cout << path << std::endl;
        int ret = remove(path.c_str());
        if (ret == 0) {
            std::cout << table_name << "删除成功" << std::endl;
        } else {
            perror("删除表失败");
        }
    } else {
        // 找到where,说明不是删除整个表 delete from student where id = 1;
    }
}

void DML::deal_InsertTable(std::string command, const std::string dbName) {
    if (dbName == "") {
        std::cout << "未指定数据库!" << std::endl;
        return;
    }
    // 清理输入，去除额外的空格和换行符
    command = std::regex_replace(command, std::regex("\\s+"), " ");

    // insert into student(1,"peter",18)
    std::regex insertPattern("insert into ([a-zA-Z_]+)\\(([^)]+)\\)");
    std::smatch matches;
    if (std::regex_search(command, matches, insertPattern)) {
        std::string tableName = matches[1];
        // 判断表是否存在
        std::string path = table_prefix + dbName + "/" + tableName + ".dat";
        if (access(path.c_str(), F_OK) != 0) {
            std::cout << dbName << "数据库中没有表" << tableName << std::endl;
            return;
        }
        std::string values = matches[2];
        // 拆分值
        std::vector<std::string> value_Tokens;
        std::istringstream tokenStream(values);
        std::string token;
        while (getline(tokenStream, token, ',')) {
            value_Tokens.push_back(token);
        }
        // 插入数据
        Table table;
        table.data.push_back(value_Tokens);
        _saveTableToFile(table, path);
        std::cout << tableName << "插入数据成功" << std::endl;
    } else {
        // 格式错误
        std::cout << "字符串格式错误,请参考 insert into <table-name>(<column>...)" << std::endl;
    }
}

void DML::_saveTableToFile(Table& table, std::string path) {
    std::ofstream outputFile(path, std::ios::app);
    if (!outputFile.is_open()) {
        perror("ofstream");
        return;
    }
    // 写入插入信息
    for (auto row : table.data) {
        for (auto col : row) {
            outputFile << col << '\t';
        }
        outputFile << '\n';
    }
    outputFile.close();
}