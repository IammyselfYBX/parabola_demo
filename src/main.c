/**
 * @file main.c
 * @brief 抛物线Demo主程序入口
 *
 * 斜抛运动小demo：在控制台中模拟物理抛物线轨迹，
 * 球体碰墙反弹并留下类贪吃蛇的彩色轨迹。
 */
#include "console.h"
#include "physics.h"
#include "map.h"
#include "snake.h"
#include <stdio.h>

#define LAUNCH_ANGLE   70.0
#define INIT_VELOCITY  25
#define INIT_MAX_LEN   60
#define LOOP_COUNT     10000

int main(void)
{
    PhysicsState phys;
    SnakeTrail trail;
    char (*map)[MAP_MAX_SIZE];
    int cnt = LOOP_COUNT;

    platform_init();
    platform_set_console_size(170, 50);
    platform_set_title("Parabola Demo");

    printf("抛物线 Demo\n");
    printf("如果花屏则调小控制台字体，按回车开始...\n");
    fflush(stdout);

    while (platform_kbhit())
        platform_getch();
    platform_getch();

    console_clear();
    console_hide_cursor();

    map_init(DEFAULT_WIDTH, DEFAULT_LENGTH, DEFAULT_FOOD_COUNT);
    map_draw(DEFAULT_WIDTH, DEFAULT_LENGTH);
    map = map_get_data();

    physics_init(&phys, DEFAULT_WIDTH - 5, 4, INIT_VELOCITY, LAUNCH_ANGLE);
    snake_init(&trail, phys.old_x, phys.old_y, INIT_MAX_LEN);

    while (cnt--) {
        physics_step(&phys);

        if (physics_check_boundary(&phys, DEFAULT_WIDTH, DEFAULT_LENGTH))
            snake_next_color(&trail);

        /* 边界修正后同步整数坐标 */
        phys.old_x = (int)phys.x;
        phys.old_y = (int)phys.y;

        snake_remove_tail(&trail, map);
        snake_add_head(&trail, map, phys.old_x, phys.old_y, DEFAULT_WIDTH);
        platform_sleep(10);
    }

    platform_cleanup();
    return 0;
}
