/**
 * @file platform.h
 * @brief 跨平台兼容层 - 封装Windows/Linux差异
 */
#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    typedef WORD ColorAttr;
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <termios.h>
    #include <sys/ioctl.h>
    #include <sys/select.h>
    typedef unsigned short ColorAttr;
#endif

/**
 * @brief 跨平台延时（毫秒）
 */
void platform_sleep(int ms);

/**
 * @brief 非阻塞读取单个按键
 */
int platform_getch(void);

/**
 * @brief 检测是否有按键输入
 */
int platform_kbhit(void);

/**
 * @brief 初始化终端
 */
void platform_init(void);

/**
 * @brief 恢复终端设置
 */
void platform_cleanup(void);

/**
 * @brief 设置控制台窗口大小
 */
void platform_set_console_size(int cols, int lines);

/**
 * @brief 设置终端窗口标题
 */
void platform_set_title(const char *title);

#endif /* PLATFORM_H */
