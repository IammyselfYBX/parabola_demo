/**
 * @file physics.c
 * @brief 物理引擎实现 - 斜抛运动的核心计算
 */
#include "physics.h"
#include <math.h>

/* 初始化物理状态：设置起始位置和分解初速度 */
void physics_init(PhysicsState *state, double start_x, double start_y,
                  double velocity, double angle)
{
    state->x = start_x;
    state->y = start_y;
    /* 分解速度到X（垂直）和Y（水平）方向，速度向右向下为正 */
    state->vx = -velocity * sin(angle * PI / 180.0);
    state->vy = velocity * cos(angle * PI / 180.0);
    state->old_x = (int)start_x;
    state->old_y = (int)start_y;
}

/*
 * 执行一步物理计算
 *
 * 算法核心：
 *   分别计算X移动1单位所需时间t1，和Y移动1单位所需时间t2，
 *   比较两个方向的位移量，选择较短的方向优先移动，
 *   确保轨迹在屏幕上连续不断裂。
 *
 *   X方向：解方程 1/2*g*t^2 + Vx*t = ±1 得到t1
 *   Y方向：t2 = 1/|Vy|，s2 = Vx*t2 + 0.5*g*t2^2
 */
void physics_step(PhysicsState *state)
{
    double t1, t2, s1, s2, temp;
    double x1_t1, x2_t1, x3_t1, x4_t1;
    int flag;

    /* 计算Y移动一个单位所需时间及对应X位移 */
    t2 = 1.0 / fabs(state->vy);
    s2 = state->vx * t2 + 0.5 * GRAVITY * t2 * t2;

    /* 计算X移动一个单位所需时间：解一元二次方程 5*t^2 + Vx*t - 1 = 0（正负1共4个解） */
    x1_t1 = (-state->vx + sqrt(state->vx * state->vx + 20)) / 10;
    x2_t1 = (-state->vx - sqrt(state->vx * state->vx + 20)) / 10;
    x3_t1 = (-state->vx + sqrt(state->vx * state->vx - 20)) / 10;
    x4_t1 = (-state->vx - sqrt(state->vx * state->vx - 20)) / 10;

    /* 从4个解中选取最小正值作为t1 */
    if (x1_t1 > 0 && x2_t1 > 0)
        t1 = x1_t1 < x2_t1 ? x1_t1 : x2_t1;
    else
        t1 = x1_t1 >= 0 ? x1_t1 : x2_t1;

    if (x3_t1 > 0 && x4_t1 > 0)
        temp = x3_t1 < x4_t1 ? x3_t1 : x4_t1;
    else if (x3_t1 * x4_t1 < 0)
        temp = x3_t1 >= 0 ? x3_t1 : x4_t1;
    else
        temp = 10; /* 无有效解时给一个大值 */

    t1 = t1 < temp ? t1 : temp;
    s1 = t1 * state->vy; /* X移动1单位时Y的位移 */

    /* 择短判断：选择位移更小的方向优先移动，保证轨迹连续 */
    if ((int)s1 == 0)
        flag = 1;
    else if ((int)s2 == 0)
        flag = 2;
    else
        flag = s1 < s2 ? 1 : 2;

    /* 根据选择的方向更新位置和速度 */
    if (flag == 1) {
        /* X方向移动1单位 */
        state->x = state->x + state->vx * t1 + 5 * t1 * t1;
        state->y = state->y + s1;
        state->vx = state->vx + GRAVITY * t1;
    } else {
        /* Y方向移动1单位 */
        state->y = state->y + state->vy * t2;
        state->x = state->x + s2;
        state->vx = state->vx + GRAVITY * t2;
    }

    /* 将浮点坐标转换为整型用于屏幕打印 */
    state->old_x = (int)state->x;
    state->old_y = (int)state->y;
}

/* 检测边界碰撞并反弹 */
int physics_check_boundary(PhysicsState *state, int width, int length)
{
    int collided = 0;

    /* 垂直方向碰墙反弹，并夹紧坐标，避免大步长越界 */
    if (state->x <= 2) {
        state->x = 2;
        state->vx = fabs(state->vx);
        collided = 1;
    } else if (state->x >= width - 2) {
        state->x = width - 2;
        state->vx = -fabs(state->vx);
        collided = 1;
    }

    /* 水平方向碰墙反弹，并夹紧坐标，避免大步长越界 */
    if (state->y <= 2) {
        state->y = 2;
        state->vy = fabs(state->vy);
        collided = 1;
    } else if (state->y >= length - 2) {
        state->y = length - 2;
        state->vy = -fabs(state->vy);
        collided = 1;
    }

    /* 同步整数屏幕坐标 */
    state->old_x = (int)state->x;
    state->old_y = (int)state->y;
    return collided;
}
