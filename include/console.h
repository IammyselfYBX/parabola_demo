/**
 * @file console.h
 * @brief 控制台工具模块 - 使用ANSI转义序列实现跨平台控制台操作
 */
#ifndef CONSOLE_H
#define CONSOLE_H

#include "platform.h"

/**
 * @brief 隐藏控制台光标
 */
void console_hide_cursor(void);

/**
 * @brief 显示控制台光标
 */
void console_show_cursor(void);

/**
 * @brief 清空屏幕并把光标移动到左上角
 */
void console_clear(void);

/**
 * @brief 移动光标到指定坐标
 * @param x 列坐标（水平方向）
 * @param y 行坐标（垂直方向）
 */
void console_gotoxy(int x, int y);

/**
 * @brief 设置控制台文本属性（颜色）
 * @param attr 颜色属性值
 */
void console_set_color(ColorAttr attr);

/**
 * @brief 重置控制台颜色为默认
 */
void console_reset_color(void);

#endif /* CONSOLE_H */
