/**
 * @file map.h
 * @brief 地图模块 - 管理游戏地图的初始化和渲染
 */
#ifndef MAP_H
#define MAP_H

/* 地图最大尺寸 */
#define MAP_MAX_SIZE 1000

/* 默认地图宽度（行数）和长度（列数） */
#define DEFAULT_WIDTH  40
#define DEFAULT_LENGTH 150

/* 默认食物数量，设为0可关闭贪吃蛇模式 */
#define DEFAULT_FOOD_COUNT 500

/**
 * @brief 获取地图数组的指针
 * @return 指向地图二维数组的指针
 */
char (*map_get_data(void))[MAP_MAX_SIZE];

/**
 * @brief 初始化地图：绘制边界、放置食物
 * @param width  地图高度（行数）
 * @param length 地图宽度（列数）
 * @param food_count 食物数量
 */
void map_init(int width, int length, int food_count);

/**
 * @brief 在屏幕上打印整张地图
 * @param width  地图高度（行数）
 * @param length 地图宽度（列数）
 */
void map_draw(int width, int length);

#endif /* MAP_H */
