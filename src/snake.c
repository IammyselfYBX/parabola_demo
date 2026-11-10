/**
 * @file snake.c
 * @brief 蛇身轨迹模块实现 - 循环队列管理轨迹节点
 */
#include "snake.h"
#include "console.h"
#include <stdio.h>

static const ColorAttr color_table[] = {0x60, 0x70, 0x40, 0x30, 0x80};
#define COLOR_COUNT 5

static int snake_position_ok(int x, int y)
{
    return x >= 0 && x < MAP_MAX_SIZE && y >= 0 && y < MAP_MAX_SIZE;
}

static void snake_erase_node(SnakeNode *node, char map[][MAP_MAX_SIZE])
{
    if (!snake_position_ok(node->x, node->y))
        return;

    console_reset_color();
    console_gotoxy(node->y, node->x);
    if (map[node->x][node->y] > 0)
        map[node->x][node->y]--;

    if (map[node->x][node->y] == ' ')
        printf("%c", ' ');
}

void snake_init(SnakeTrail *trail, int init_x, int init_y, int max_length)
{
    if (max_length < 1)
        max_length = 1;
    if (max_length > SNAKE_MAX_NODES)
        max_length = SNAKE_MAX_NODES;

    trail->step = 0;
    trail->max_length = max_length;
    trail->tail_active = 0;
    trail->color_index = 0;
    trail->nodes[0].x = init_x;
    trail->nodes[0].y = init_y;
}

void snake_remove_tail(SnakeTrail *trail, char map[][MAP_MAX_SIZE])
{
    if (!trail->tail_active || trail->max_length <= 0)
        return;

    if (trail->step < 0 || trail->step >= trail->max_length)
        trail->step = 0;

    snake_erase_node(&trail->nodes[trail->step], map);
}

void snake_add_head(SnakeTrail *trail, char map[][MAP_MAX_SIZE],
                    int x, int y, int width)
{
    if (!snake_position_ok(x, y) || trail->max_length <= 0)
        return;

    if (trail->step < 0 || trail->step >= trail->max_length)
        trail->step = 0;

    trail->nodes[trail->step].x = x;
    trail->nodes[trail->step].y = y;

    if (map[x][y] == '*') {
        map[x][y] = ' ';
        if (trail->max_length < SNAKE_MAX_NODES)
            trail->max_length++;
        console_gotoxy(2, width + 2);
        console_reset_color();
        printf("len:%5d", trail->max_length);
    }

    snake_set_color(trail);
    console_gotoxy(y, x);
    map[x][y]++;
    printf("%c", ' ');

    trail->step++;
    if (trail->step >= trail->max_length)
        trail->tail_active = 1;
    trail->step %= trail->max_length;
}

void snake_set_color(SnakeTrail *trail)
{
    console_set_color(color_table[trail->color_index % COLOR_COUNT]);
}

void snake_next_color(SnakeTrail *trail)
{
    trail->color_index = (trail->color_index + 1) % COLOR_COUNT;
}
