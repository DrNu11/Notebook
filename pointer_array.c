#include <stddef.h>
#include <limits.h>

/*
 * 高性能指针版数组操作函数（C99/C11）
 * 说明：
 * 1) 全部采用指针遍历，避免重复的下标寻址开销。
 * 2) 重要参数检查放在入口，满足“检查返回值/参数”的要求。
 * 3) 所有函数为纯 C 实现，便于移植到嵌入式或受限环境。
 */

/*
 * 求和：返回 int 数组元素之和。
 * 参数：
 *   arr  : 输入数组指针（不可为 NULL，len > 0 时必须有效）
 *   len  : 数组长度
 * 返回：
 *   成功：求和结果
 *   失败：若 arr 为空且 len > 0，返回 0（调用方需自行判断输入合法性）
 *
 * 注：这里不做溢出处理（保持最高效率），如需防溢出请改用更大类型。
 */
int array_sum_ptr(const int *arr, size_t len)
{
    if (arr == NULL && len > 0) {
        /* 入参非法，返回 0；高性能场景下避免额外开销 */
        return 0;
    }

    const int *p = arr;
    const int *end = arr + len;
    int sum = 0;

    /* 指针线性遍历，编译器易于自动向量化 */
    while (p < end) {
        sum += *p;
        ++p;
    }

    return sum;
}

/*
 * 反转：就地反转 int 数组。
 * 参数：
 *   arr : 输入/输出数组指针（不可为 NULL，len > 0 时必须有效）
 *   len : 数组长度
 * 返回：
 *   0  : 成功
 *  -1  : 入参非法（arr 为空且 len > 0）
 */
int array_reverse_ptr(int *arr, size_t len)
{
    if (arr == NULL && len > 0) {
        return -1;
    }

    /* 双指针从两端向中间逼近，交换元素 */
    int *left = arr;
    int *right = arr + len;

    /* right 指向尾后位置，先左移再交换，减少边界判断 */
    while (left < right) {
        --right;
        if (left >= right) {
            break;
        }
        const int tmp = *left;
        *left = *right;
        *right = tmp;
        ++left;
    }

    return 0;
}

/*
 * 拷贝：将 src 拷贝到 dst（长度为 len）。
 * 参数：
 *   dst : 目标数组指针（不可为 NULL，len > 0 时必须有效）
 *   src : 源数组指针（不可为 NULL，len > 0 时必须有效）
 *   len : 数组长度
 * 返回：
 *   0  : 成功
 *  -1  : 入参非法
 *
 * 注意：不处理重叠区域，若可能重叠请改用 memmove。
 */
int array_copy_ptr(int *dst, const int *src, size_t len)
{
    if ((dst == NULL || src == NULL) && len > 0) {
        return -1;
    }

    int *d = dst;
    const int *s = src;
    const int *end = src + len;

    /* 线性拷贝，编译器可自动展开或向量化 */
    while (s < end) {
        *d = *s;
        ++d;
        ++s;
    }

    return 0;
}

/*
 * 查找最大值：返回数组最大元素。
 * 参数：
 *   arr : 输入数组指针（不可为 NULL，len > 0 时必须有效）
 *   len : 数组长度
 *   ok  : 输出参数（可为 NULL），成功时写 1，失败写 0
 * 返回：
 *   成功：最大值
 *   失败：若 arr 为空或 len 为 0，返回 INT_MIN
 */
int array_max_ptr(const int *arr, size_t len, int *ok)
{
    if (ok != NULL) {
        *ok = 0;
    }
    if (arr == NULL || len == 0) {
        return INT_MIN;
    }

    const int *p = arr;
    const int *end = arr + len;
    int maxv = *p;
    ++p;

    /* 单次遍历，分支尽量简单 */
    while (p < end) {
        if (*p > maxv) {
            maxv = *p;
        }
        ++p;
    }

    if (ok != NULL) {
        *ok = 1;
    }
    return maxv;
}
