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
#include <map>
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
     * @brief 根据命令创建表
     *
     * @param command 输入命令
     * @param dbName 数据库名
     */
    void deal_createTable_command(const std::string command, const std::string dbName);

    /**
     * @brief 根据table创建表
     *
     * @param command 输入命令
     * @param dbName 数据库名
     */
    void deal_createTable_table(const Table table, const std::string dbName);

    /**
     * @brief 删除表
     *
     * @param command 输入命令
     * @param dbName 数据库名
     */
    void deal_deleteTable(const std::string command, const std::string dbName);

    /**
     * @brief 通过命令向表中插入数据
     *
     * @param command 输入命令
     * @param dbName 数据库名
     */
    void deal_InsertTable_command(std::string command, const std::string dbName);

    /**
     * @brief 通过table向表中插入数据
     *
     * @param table
     * @param dbName
     */
    void deal_InsertTable_table(Table table, const std::string dbName);

    /**
     * @brief 查询表中数据
     *
     * @param command 输入命令
     * @param dbName 数据库名
     */
    void deal_selectTable(std::string command, const std::string dbName);

private:
    /**
     * @brief 创建表时处理输入语句
     *
     * @param command
     * @param table
     */
    void _deal_command(std::string command, Table& table);
    /**
     * @brief 将数据存入表文件中
     *
     * @param table 存储即将插入的数据
     * @param path  表文件路径
     */
    void _saveTableToFile(Table& table, std::string path);

    /**
     * @brief 将表中数据加载到Table中
     *
     * @param table 存储数据的结构体
     * @param path 表的路径
     * @param column_idx 存储列明对应的下标
     */
    void _loadTableFromFile(Table& table, std::string path, std::map<std::string, int>& column_idx);

    /**
     * @brief 处理where之后的语句，得到键值对
     *
     * @param command 命令
     * @param cond 存储条件的集合
     */
    void _afterWhere(std::string command, std::map<std::string, std::string>& cond, std::vector<int>& cond_idx, std::map<std::string, int> column_idx);

    /**
     * @brief 处理单词的空格
     *
     * @param word
     */
    std::string _deal_whitespace(std::string& word);
};

#endif