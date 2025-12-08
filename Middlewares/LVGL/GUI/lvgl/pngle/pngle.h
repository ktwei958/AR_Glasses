// pngle.h
#ifndef PNGLE_H
#define PNGLE_H

#include <stdint.h>
#include <stddef.h>

#define PNGLE_OK          0
#define PNGLE_ERROR       -1

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t bpp;          // 位深（通常32位RGBA）
    uint8_t *row_buf;     // 单行像素缓存（减少RAM占用）
    // 解码回调：每解码一行就触发，用于渲染到LVGL
    void (*on_draw)(void *user_data, int x, int y, int w, int h, uint8_t *rgba);
    void *user_data;      // 回调传参（LVGL画布对象）
} pngle_t;

// 初始化pngle
pngle_t *pngle_init(void);
// 分块喂入PNG数据（适配TF卡分块读取）
int pngle_feed(pngle_t *pngle, const uint8_t *data, size_t len);
// 释放资源
void pngle_destroy(pngle_t *pngle);

#endif // PNGLE_H
