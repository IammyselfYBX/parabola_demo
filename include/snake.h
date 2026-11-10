/**
 * @file snake.h
 * @brief 蛇身轨迹模块 - 管理球体运动的轨迹（类贪吃蛇效果）
 *
 * 使用循环队列存储轨迹节点，实现蛇身的增长和消除。
 */
#ifndef SNAKE_H
#define SNAKE_H

#include "map.h"

/* 轨迹节点最大数量 */
#define SNAKE_MAX_NODES 1000

/**
 * @brief 轨迹节点结构体
 */
typedef struct {
    int x;  /* 节点X坐标 */
    int y;  /* 节点Y坐标 */
} SnakeNode;

/**
 * @brief 蛇身管理器结构体
 */
typedef struct {
    SnakeNode nodes[SNAKE_MAX_NODES]; /* 循环队列存储的节点数组 */
    int step;          /* 当前步数/写入位置 */
    int max_length;    /* 最大蛇身长度 */
    int tail_active;   /* 是否已开始消除尾部（1=是） */
    int color_index;   /* 当前颜色索引 */
} SnakeTrail;

/**
 * @brief 初始化蛇身管理器
 * @param trail       指向蛇身管理器的指针
 * @param init_x      初始X坐标
 * @param init_y      初始Y坐标
 * @param max_length  初始最大长度
 */
void snake_init(SnakeTrail *trail, int init_x, int init_y, int max_length);

/**
 * @brief 移除蛇尾节点（如果蛇身已达最大长度）
 * @param trail 指向蛇身管理器的指针
 * @param map   地图数组指针
 */
void snake_remove_tail(SnakeTrail *trail, char map[][MAP_MAX_SIZE]);

/**
 * @brief 添加新的蛇头节点并渲染
 * @param trail 指向蛇身管理器的指针
 * @param map   地图数组指针
 * @param x     新节点X坐标
 * @param y     新节点Y坐标
 * @param width 地图宽度（用于显示信息位置）
 */
void snake_add_head(SnakeTrail *trail, char map[][MAP_MAX_SIZE],
                    int x, int y, int width);

/**
 * @brief 设置蛇身颜色
 * @param trail 指向蛇身管理器的指针
 */
void snake_set_color(SnakeTrail *trail);

/**
 * @brief 碰撞后切换颜色
 * @param trail 指向蛇身管理器的指针
 */
void snake_next_color(SnakeTrail *trail);

#endif /* SNAKE_H */
