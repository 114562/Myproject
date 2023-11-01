#include "dml.h"

#include <dirent.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

const std::string table_prefix = "../DataBase/";

void DML::deal_createTable_command(const std::string command, const std::string dbName) {
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
        perror("ofstream");
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

void DML::deal_createTable_table(const Table table, const std::string dbName) {
    // 增加表的后缀
    std::string table_name = table.name + ".dat";
    std::string fileName = table_prefix + dbName + "/" + table_name;

    std::ofstream outputFile(fileName, std::ios::binary);
    if (!outputFile.is_open()) {
        perror("ofstream");
        exit(1);
    }
    int numOfColumns = table.columns.size();
    // 写入列信息
    for (const Column& column : table.columns) {
        outputFile << column.name << "(" << column.type << ')' << '\t';
    }
    outputFile << '\n';

    // 关闭数据流文件并给出提示信息
    // std::cout << table.name << "表创建成功" << std::endl;
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
    std::cin.clear();
    if (std::regex_search(command, matches, tablePattern)) {
        // 提取表名
        std::string tableName = matches[1];
        // 检查是否获取正确表名
        //  std::cout << "Table Name: " << tableName << std::endl;
        table.name = tableName;

        std::string columns = matches[2];
        std::regex columnPattern("([a-zA-Z_]+)\\(([^)]+)\\)");
        std::smatch columnMatches;
        std::cin.clear();
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
    std::string path;
    path = table_prefix + dbName + "/";
    // 获取表名
    std::string table_name;
    // int pos = 0;
    // while (pos < table_name.size()) {
    //     if (table_name[pos] != ' ') {
    //         break;
    //     }
    //     pos++;
    // }
    // int idx1 = pos;
    // pos = table_name.size() - 1;
    // while (pos) {
    //     if (table_name[pos] != ' ') {
    //         break;
    //     }
    //     pos--;
    // }
    // int idx2 = pos;
    // table_name = table_name.substr(idx1, idx2 - idx1 + 1);

    if (wherePos == std::string::npos) {
        // delete from student;
        table_name = command.substr(12, command.size() - 12);
        table_name = _deal_whitespace(table_name);
        path = path + table_name + ".dat";
        // 没有where限制符,删除整个表
        // std::cout << path << std::endl;
        int ret = remove(path.c_str());
        if (ret == 0) {
            std::cout << table_name << "删除成功" << std::endl;
        } else {
            perror("删除表失败");
        }
    } else {
        // 找到where,说明不是删除整个表 delete from student where id = 1;
        table_name = command.substr(12, wherePos - 12);
        table_name = _deal_whitespace(table_name);
        path = path + table_name + ".dat";
        Table table;
        // 存储列名的下标
        std::map<std::string, int> column_idx;
        // 将数据存储到table中
        _loadTableFromFile(table, path, column_idx);
        // 记录满足条件的下标
        std::vector<int> cond_idx;
        // 存储条件,键存储的是列名 值存储的是要求的值
        std::map<std::string, std::string> cond;
        _afterWhere(command, cond, cond_idx, column_idx);
        // 遍历table删除对应的行数据
        for (auto iter = table.data.begin(); iter != table.data.end();) {
            // 判断这一行是否删除
            bool is_valid = true;
            // 遍历满足的条件
            for (auto idx : cond_idx) {  // 得到有要求列的序号
                // 对应列名
                std::string col_name = table.columns[idx].name;
                std::string excpected_val = cond[col_name];
                // 判断table中的这一行的值是否相等,也就是表中的值是否相等
                if ((*iter)[idx] != excpected_val) {
                    is_valid = false;
                    break;
                }
            }
            // 注意:这样写是为了防止迭代器出现越界异常
            if (is_valid) {
                // 删除该行数据
                iter = table.data.erase(iter);
                std::cout << "成功删除指定行数据" << std::endl;
            } else {
                iter++;
            }
        }
        // 删除表再重新写入一次表
        std::string delete_table = "delete from " + table_name;
        deal_deleteTable(delete_table, dbName);
        // 在表中写入表名,表创建时候需要
        table.name = table_name;
        // 创建表
        deal_createTable_table(table, dbName);
        // 重写数据
        deal_InsertTable_table(table, dbName);
    }
}

void DML::deal_InsertTable_command(std::string command, const std::string dbName) {
    if (dbName == "") {
        std::cout << "未指定数据库!" << std::endl;
        return;
    }
    // 清理输入，去除额外的空格和换行符
    command = std::regex_replace(command, std::regex("\\s+"), " ");

    // insert into student(1,"peter",18)
    std::regex insertPattern("insert into ([a-zA-Z_]+)\\(([^)]+)\\)");
    std::smatch matches;
    // 清空输入缓冲区
    std::cin.clear();
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

void DML::deal_InsertTable_table(Table table, const std::string dbName) {
    if (dbName == "") {
        std::cout << "未指定数据库!" << std::endl;
        return;
    }
    std::string path = table_prefix + dbName + "/" + table.name + ".dat";
    if (access(path.c_str(), F_OK) != 0) {
        std::cout << dbName << "数据库中没有表" << table.name << std::endl;
        return;
    }
    // 将table中的表数据存储到文件中
    _saveTableToFile(table, path);
    std::cout << table.name << "插入数据成功" << std::endl;
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

void DML::deal_selectTable(std::string command, const std::string dbName) {
    if (dbName == "") {
        std::cout << "未指定数据库" << std::endl;
        return;
    }
    // 清理输入，去除额外的空格和换行符
    command = std::regex_replace(command, std::regex("\\s+"), " ");
    // 格式命令 select * from table;    select id from table where id = 1;

    // 判断是否有where
    size_t wherePos = command.find("where");
    std::string path;

    if (wherePos == std::string::npos) {
        // 没有where,查询整个表
        size_t fromPos = command.find("from");

        std::string table_name = command.substr(fromPos + 4, command.size() - fromPos + 4);
        // 获取表名
        table_name = _deal_whitespace(table_name);
        path = table_prefix + dbName + "/" + table_name + ".dat";
        std::ifstream inputFile(path, std::ios::binary);
        if (!inputFile.is_open()) {
            perror("ifstream");
            exit(1);
        }
        if (command.find('*') != std::string::npos) {
            // 查询所有列的所有行
            std::string line;
            while (getline(inputFile, line)) {
                std::cout << line << std::endl;
            }
        } else {
            // 查询特定列的所有行
        }

    } else {
        // 有选择性的输出 select * from student where id = 1
        size_t fromPos = command.find("from");
        // 表名在from -- where中间
        std::string table_name = command.substr(fromPos + 4, wherePos - fromPos - 4);
        std::cout << "test: " << table_name << std::endl;
        // 获取表名
        table_name = _deal_whitespace(table_name);
        path = table_prefix + dbName + "/" + table_name + ".dat";
        std::ifstream input(path, std::ios::binary);
        if (!input.is_open()) {
            perror("ifstream");
            exit(1);
        }
        Table table;
        if (command.find('*') != std::string::npos) {
            // 查询所有列的特定行 select * from student where id = 1

            // 存储列名的下标
            std::map<std::string, int> column_idx;
            // 将数据存储到table中
            _loadTableFromFile(table, path, column_idx);
            // 记录需要判断的列名的下标
            std::vector<int> cond_idx;
            // 存储条件,键存储的是列名 值存储的是要求的值
            std::map<std::string, std::string> cond;
            _afterWhere(command, cond, cond_idx, column_idx);

            // 先输出表第一行,即表的列名
            std::string line;
            getline(input, line);
            std::cout << line << std::endl;

            // 遍历,输出所有满足条件的行
            for (auto iter = table.data.begin(); iter != table.data.end(); iter++) {
                bool is_valid = true;
                for (auto idx : cond_idx) {
                    // 遍历限制条件
                    std::string col_name = table.columns[idx].name;
                    std::string excpected_val = cond[col_name];
                    if ((*iter)[idx] != excpected_val) {
                        is_valid = false;
                        break;
                    }
                }
                if (is_valid) {
                    // 输出该行数据
                    for (std::string s : (*iter)) {
                        std::cout << s << '\t';
                    }
                    std::cout << std::endl;
                }
            }
        } else {
            // 查询特定列的特定行
        }
    }
}

void DML::deal_updateTable(std::string command, const std::string dbName) {
    if (dbName == "") {
        std::cout << "未指定数据库" << std::endl;
        return;
    }
    // update student set name = 杨径骁 where id = 1;
    // 获取表名
    size_t updatePos = command.find("update");
    size_t setPos = command.find("set");
    if (setPos == std::string::npos) {
        std::cout << "update 格式错误,请重新输入" << std::endl;
        return;
    }
    // 获取表名
    std::string table_name = command.substr(updatePos + 6, setPos - updatePos - 6);
    table_name = _deal_whitespace(table_name);

    // 获取表路径
    std::string path = table_prefix + dbName + "/" + table_name + ".dat";

    // table用于存储表中数据
    Table table;

    // 存储列名和对应的下标
    std::map<std::string, int> column_idx;
    // 记录需要判断的列名的下标
    std::vector<int> cond_idx;
    // 存储条件,键存储的是列名 值存储的是要求的值
    std::map<std::string, std::string> cond;

    // 将表中数据加载到table中,将列名和其下标加载到column_idx中
    _loadTableFromFile(table, path, column_idx);

    // 将指令中需要满足的列名和其对应的值加载到cond中, 以及判断的列的下标加载到cond_idx中
    _afterWhere(command, cond, cond_idx, column_idx);

    // 存储需要修改的列和期望修改的值 set和where之间
    std::map<std::string, std::string> modify;

    _afterSet(command, modify);

    // 遍历table.data,匹配满足的行,修改期望的列
    for (auto iter = table.data.begin(); iter != table.data.end(); iter++) {
        // 得到表中的每一行数据
        // 判断是否满足条件
        bool is_valid = true;
        for (auto idx : cond_idx) {
            std::string col_name = table.columns[idx].name;
            std::string excepted_val = cond[col_name];
            if ((*iter)[idx] != excepted_val) {
                is_valid = false;
                break;
            }
        }
        if (is_valid) {
            // 满足条件，修改table表中对应的值
            for (auto mod : modify) {
                // 要修改的列名
                std::string col_name = mod.first;
                // 修改的数据
                std::string expected_val = mod.second;
                // 得到表中对应列的下标
                int idx = column_idx[col_name];
                // 修改对应表中对应下标的值
                (*iter)[idx] = expected_val;
            }
        }
    }
    // 删除表再重新插入表,更新数据
    std::string delete_table = "delete from " + table_name;

    deal_deleteTable(delete_table, dbName);
    // 在表中写入表名,表创建时候需要
    table.name = table_name;
    // 创建表
    deal_createTable_table(table, dbName);
    // 重写数据
    deal_InsertTable_table(table, dbName);
    std::cout << "数据更新成功" << std::endl;
}

void DML::_afterSet(std::string command, std::map<std::string, std::string>& modify) {
    int setPos = command.find("set");
    int wherePos = command.find("where");
    // 得到find结尾位置
    setPos += 3;
    // 获取到set之后where之前的命令
    // update student set name = tyq where id = 3;
    command = command.substr(setPos, wherePos - setPos);
    // 去掉空格
    command = _deal_whitespace(command);
    for (int i = 0; i < command.size(); i++) {
        if (command[i] == ' ') {
            continue;
        }
        // 记录开始坐标
        int start = i;
        int end = i;
        while (end < command.size()) {
            if (command[end] == '=') {
                // 遇到等号,截取列名
                break;
            }
            end++;
        }
        std::string col = command.substr(start, end - start);
        col = _deal_whitespace(col);
        // 得到限制条件的值
        start = end + 1;
        end++;
        while (end < command.size()) {
            if (command[end] == ',') {
                // 遇到 , 表示遇到下一个条件
                break;
            }
            end++;
        }
        std::string val = command.substr(start, end - start);
        val = _deal_whitespace(val);
        // 将期望修改的键值对加入modify中
        modify[col] = val;
        // 接着判断
        i = end;
    }
}

void DML::_loadTableFromFile(Table& table, std::string path, std::map<std::string, int>& column_idx) {
    // 将整个表的结构重新构造到Table中
    std::ifstream inputFile(path, std::ios::binary);
    if (!inputFile.is_open()) {
        perror("加载表失败");
        exit(1);
    }
    // 存储第一行信息(列名和类型)
    std::string nameAndtype;
    // 获取第一排的列名字
    getline(inputFile, nameAndtype);
    std::regex columnPattern("([a-zA-Z_]+)\\(([^)]+)\\)");
    std::smatch matcher;
    std::string columnName;
    std::string columnType;
    std::cin.clear();
    // 使用正则表达式匹配
    int idx = 0;
    while (std::regex_search(nameAndtype, matcher, columnPattern)) {
        columnName = matcher[1];
        columnType = matcher[2];
        Column column;
        column.name = columnName;
        column.type = columnType;
        table.columns.push_back(column);
        column_idx[columnName] = idx++;
        // 移到下一列
        nameAndtype = matcher.suffix().str();
    }
    // 读出数据,存到table中
    std::string oneLineData;
    while (getline(inputFile, oneLineData)) {
        std::vector<std::string> value;
        std::istringstream iss(oneLineData);
        std::string tmp;
        while (getline(iss, tmp, '\t')) {
            value.push_back(tmp);
        }
        table.data.push_back(value);
    }
}

void DML::_afterWhere(std::string command, std::map<std::string, std::string>& cond, std::vector<int>& cond_idx, std::map<std::string, int> column_idx) {
    int idx = command.find("where");
    // 得到find结尾位置
    idx += 5;
    command = command.substr(idx, command.size() - idx);
    for (int i = 0; i < command.size(); i++) {
        if (command[i] == ' ') {
            continue;
        }
        // 记录开始坐标
        int start = i;
        int end = i;
        while (end < command.size()) {
            if (command[end] == '=') {
                // 遇到等号,截取列名
                break;
            }
            end++;
        }
        std::string col = command.substr(start, end - start);
        col = _deal_whitespace(col);
        // 得到限制条件的值
        start = end + 1;
        end++;
        while (end < command.size()) {
            if (command[end] == ',') {
                // 遇到 , 表示遇到下一个条件
                break;
            }
            end++;
        }
        std::string val = command.substr(start, end - start);
        val = _deal_whitespace(val);
        // 将限制条件加入cond存储队列中
        cond[col] = val;
        // 将限制条件对应的列的下标加入cond_idx中,方便后续比较
        cond_idx.push_back(column_idx[col]);
        // 接着判断
        i = end;
    }
}

std::string DML::_deal_whitespace(std::string& word) {
    int pos = 0;
    while (pos < word.size()) {
        if (word[pos] != ' ') {
            break;
        }
        pos++;
    }
    int idx1 = pos;
    pos = word.size() - 1;
    while (pos) {
        if (word[pos] != ' ') {
            break;
        }
        pos--;
    }
    int idx2 = pos;
    word = word.substr(idx1, idx2 - idx1 + 1);
    return word;
}