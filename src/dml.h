/**
 * @file dml.h
 * @author 杨径骁 (2372256530@qq.com)
 * @brief DML语句的操作函数头文件
 * @version 1.0
 * @date 2023.10.24
 *
 * @copyright Copyright (c) 2023 电子科技大学
 *
 */

#ifndef _DML_H
#define _DML_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// 定义表结构
/**
 * @brief  创建结构体存储列的名字和类型
 */
struct Column {
    std::string name;
    std::string type;
};

struct Table {
    std::string name;
    std::vector<Column> columns;
    std::vector<std::vector<std::string>> data;
};

class DML {
public:
    /**
     * @brief 默认构造函数
     */
    DML() = default;
    /**
     * @brief
     */
    virtual ~DML() = default;

public:
    /**
     * @brief 创建表
     */
    void deal_createTable(const std::string command, const std::string dbName);

    /**
     * @brief 删除表
     */
    void deal_deleteTable(const std::string command, const std::string dbName);

    /**
     * @brief 向表中插入数据
     */
    void deal_InsertTable(std::string command, const std::string dbName);

    /**
     * @brief 更新表中数据
     */
    // void deal_updateTable();
private:
    void _deal_command(std::string command, Table& table);
    void _saveTableToFile(Table& table, std::string path);
};

#endif