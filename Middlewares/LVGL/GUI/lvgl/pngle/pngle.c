// pngle.c（简化版，仅保留RGB888解码，适配STM32）
#include "pngle.h"
#include <string.h>
#include <stdlib.h>
#include "lvgl.h"  // 关键：LVGL内存函数声明在此

#define PNG_SIGNATURE 0x89504E470D0A1A0AUL

typedef struct {
    uint32_t sig;
    uint32_t width;
    uint32_t height;
    uint8_t depth;
    uint8_t color_type;
    uint8_t compression;
    uint8_t filter;
    uint8_t interlace;
} png_header_t;

pngle_t *pngle_init(void) {
    pngle_t *pngle = (pngle_t *)lv_mem_alloc(sizeof(pngle_t)); // 替换为lv_mem_alloc
    if (!pngle) return NULL;
    memset(pngle, 0, sizeof(pngle_t));
    return pngle;
}

int pngle_feed(pngle_t *pngle, const uint8_t *data, size_t len) {
    static png_header_t hdr;
    static uint32_t parse_state = 0;
    static uint32_t row = 0;

    for (size_t i = 0; i < len; i++) {
        switch (parse_state) {
            // 解析PNG签名（8字节）
            case 0 ... 7:
                ((uint8_t *)&hdr.sig)[parse_state] = data[i];
                parse_state++;
                if (parse_state == 8 && hdr.sig != PNG_SIGNATURE) return PNGLE_ERROR;
                break;
            // 解析IHDR块（宽度/高度）
            case 8 ... 19:
                ((uint8_t *)&hdr.width)[parse_state - 8] = data[i];
                parse_state++;
                if (parse_state == 20) {
                    pngle->width = __builtin_bswap32(hdr.width);  // 字节序转换
                    pngle->height = __builtin_bswap32(hdr.height);
                    pngle->bpp = hdr.depth;
                    // 分配单行缓存（240×4=960字节，适配240宽度）
                    pngle->row_buf = (uint8_t *)lv_mem_alloc(pngle->width * 4); // 替换为lv_mem_alloc
                }
                break;
            // 解析像素数据（逐行解码）
            default:
                if (row < pngle->height) {
                    // 简化过滤：直接读取RGB数据（忽略Alpha通道）
                    memcpy(pngle->row_buf, &data[i], pngle->width * 4);
                    // 触发回调：渲染当前行到LVGL
                    if (pngle->on_draw) {
                        pngle->on_draw(pngle->user_data, 0, row, pngle->width, 1, pngle->row_buf);
                    }
                    row++;
                    i += pngle->width * 4 - 1; // 跳过已处理的像素
                }
                break;
        }
    }
    return PNGLE_OK;
}

void pngle_destroy(pngle_t *pngle) {
    if (pngle->row_buf) lv_mem_free(pngle->row_buf); // 替换为lv_mem_free
    lv_mem_free(pngle); // 替换为lv_mem_free
}
