/**
 * @file order.h
 * @author 杨径骁 (2372256530@qq.com)
 * @brief 处理输入命令的头文件
 * @version 1.0
 * @date {2023.10.22}
 *
 * @copyright Copyright (c) 2023
 *
 *
 */

#ifndef _ORDER_H
#define _ORDER_H
#include <iostream>
#include <string>

#include "dml.h"

// const std::string prefix_path = "/home/yjx/Mine/Myproject/resources/";
class Order {
public:
    /**
     * @brief 默认构造函数
     */
    Order() = default;

    /**
     * @brief 默认析构函数
     */
    virtual ~Order() = default;

public:
    /**
     * @brief 存储输入的命令类型,方便定位到指定的操作函数
     * Show 展示命令的格式规范
     * Tree 展示数据库的目录架构
     * Quit 退出程序
     * Clear 清空屏幕
     * Create_Database 创建数据库
     * Drop_Database 销毁数据库
     * Use 切换数据库
     * Create_Trble 创建表
     * Drop_Table 删除表
     * Select 查询表中数据
     * Update 更新表中数据
     * Delete 删除表中数据
     * Insert 增添表中数据
     * Unknown 未知命令
     */
    enum command_t {
        Show,
        Tree,
        Quit,
        Clear,
        Create_Database,
        Drop_Database,
        Use,
        Create_Table,
        Drop_table,
        Select,
        Update,
        Delete,
        Insert,
        Unknown
    };

    /**
     * @brief 每次输入命令之前需要将命令的全部成员清空
     */
    void clear();

    /**
     * @brief 设置命令
     *
     * @param command 输入的命令
     */
    void set_command(const std::string& command);

    /**
     * @brief 具体处理函数
     */
    void run();

private:
    /**
     * @brief 获取命令的类型
     *
     * @param command 输入的指令
     * @return command_t 返回指令类型
     */
    command_t _getCommandType();

    /**
     * @brief 处理展示功能,重新展示主页
     */
    void _deal_show();

    /**
     * @brief 以树形结构展示目录结构
     */
    void _deal_tree();

    /**
     * @brief 退出系统
     */
    void _deal_quit();

    /**
     * @brief 清屏
     */
    void _deal_clear();

    /**
     * @brief 创建一个数据库
     */
    void _deal_createDB();

    /**
     * @brief 删除一个数据库
     */
    void _deal_dropDB();

    /**
     * @brief 处理未知的命令
     */
    void _deal_unknown();

    /**
     * @brief 切换当前使用的数据库
     */
    void _deal_changeDB();

    /**
     * @brief 创建表
     */
    void _deal_createTable();

    /**
     * @brief 删除表或表中数据
     */
    void _deal_deleteTable();

    /**
     * @brief 向表中插入数据
     */
    void _deal_insertTable();

    /**
     * @brief 查询表中数据
     */
    void _deal_selectTable();

    /**
     * @brief 从命令行中找到数据库的名称
     *
     * @param n 从第几位开始查找数据库名称
     */
    std::string _deal_findName(int n);

    /**
     * @brief 打印信息至屏幕
     * @param path 要打开的文件路径
     */
    void open_and_print(const std::string path);

private:
    /**
     * @brief 存储当前用户输入的命令
     */
    std::string command;
    /**
     * @brief 输入命令对应的类型
     */
    command_t m_command_type;

    /**
     * @brief 当前使用数据库的名称
     */
    std::string m_db_name;

    DML dml;
};

#endif