/**
 * @file console.c
 * @brief 控制台工具模块实现 - 使用ANSI转义序列实现跨平台
 */
#include "console.h"
#include <stdio.h>

/* 隐藏控制台光标 */
void console_hide_cursor(void)
{
    printf("\033[?25l");
    fflush(stdout);
}

/* 显示控制台光标 */
void console_show_cursor(void)
{
    printf("\033[?25h");
    fflush(stdout);
}

/* 清空屏幕并把光标移动到左上角 */
void console_clear(void)
{
    printf("\033[2J\033[H");
    fflush(stdout);
}

/* 移动光标到指定位置 */
void console_gotoxy(int x, int y)
{
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

/* 将Windows风格颜色属性转换为ANSI转义序列 */
void console_set_color(ColorAttr attr)
{
    static const int ansi_fg[] = {30,34,32,36,31,35,33,37,90,94,92,96,91,95,93,97};
    static const int ansi_bg[] = {40,44,42,46,41,45,43,47,100,104,102,106,101,105,103,107};

    int fg = attr & 0x0F;
    int bg = (attr >> 4) & 0x0F;

    printf("\033[%d;%dm", ansi_fg[fg], ansi_bg[bg]);
    fflush(stdout);
}

/* 重置为默认颜色 */
void console_reset_color(void)
{
    printf("\033[0m");
    fflush(stdout);
}
