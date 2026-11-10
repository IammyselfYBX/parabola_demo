/**
 * @file platform.c
 * @brief 跨平台兼容层实现
 */
#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif
#include "platform.h"
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
/* ==================== Windows 实现 ==================== */

void platform_sleep(int ms)
{
    Sleep(ms);
}

int platform_getch(void)
{
    return _getch();
}

int platform_kbhit(void)
{
    return _kbhit();
}

void platform_init(void)
{
    HANDLE output;
    DWORD mode;

    /* 源码按 UTF-8 读取，可执行文件按 GBK 生成 */
    SetConsoleOutputCP(936);
    SetConsoleCP(936);

    /* 启用 ANSI 颜色/光标/清屏控制序列 */
    output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (output != INVALID_HANDLE_VALUE && GetConsoleMode(output, &mode)) {
        SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}

void platform_cleanup(void)
{
    printf("[?25h");
    fflush(stdout);
}

void platform_set_console_size(int cols, int lines)
{
    char cmd[64];
    sprintf(cmd, "mode con cols=%d lines=%d", cols, lines);
    system(cmd);
}

void platform_set_title(const char *title)
{
    SetConsoleTitleA(title);
}

#else
/* ==================== Linux/macOS 实现 ==================== */

static struct termios orig_termios;
static int termios_saved = 0;

void platform_sleep(int ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

int platform_getch(void)
{
    int ch;
    struct termios raw;

    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    ch = getchar();

    /* 方向键转义序列转换 */
    if (ch == 27) {
        int ch2 = getchar();
        if (ch2 == '[') {
            int ch3 = getchar();
            switch (ch3) {
            case 'A': ch = 'w'; break;
            case 'B': ch = 's'; break;
            case 'C': ch = 'd'; break;
            case 'D': ch = 'a'; break;
            default:  ch = ch3; break;
            }
        }
    }

    if (termios_saved)
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);

    return ch;
}

int platform_kbhit(void)
{
    struct timeval tv = {0, 0};
    fd_set fds;
    struct termios raw;

    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

    if (termios_saved)
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);

    return FD_ISSET(STDIN_FILENO, &fds);
}

void platform_init(void)
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    termios_saved = 1;

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void platform_cleanup(void)
{
    if (termios_saved) {
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
        termios_saved = 0;
    }
    printf("\033[?25h");
    fflush(stdout);
}



void platform_set_console_size(int cols, int lines)
{
    printf("[8;%d;%dt", lines, cols);
    fflush(stdout);
}

void platform_set_title(const char *title)
{
    printf("]0;%s", title);
    fflush(stdout);
}

#endif
