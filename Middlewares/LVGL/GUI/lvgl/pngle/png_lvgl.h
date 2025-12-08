//// png_lvgl.h
//#ifndef PNG_LVGL_H
//#define PNG_LVGL_H
//
//#include "lvgl.h"   // 仅需包含这一个LVGL头文件（v8.2）
//#include "ff.h"     // FATFS头文件
//#include "pngle.h"  // PNG解码库头文件
//
//// 将RGBA8888转换为RGB565（适配ST7789）
//static inline uint16_t rgba8888_to_rgb565(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
//    (void)a; // 忽略Alpha（如需透明可扩展）
//    uint16_t r5 = (r >> 3) & 0x1F;
//    uint16_t g6 = (g >> 2) & 0x3F;
//    uint16_t b5 = (b >> 3) & 0x1F;
//    return (r5 << 11) | (g6 << 5) | b5;
//}
//
//// PNG解码回调：逐行渲染到LVGL画布
//static void png_draw_cb(void *user_data, int x, int y, int w, int h, uint8_t *rgba) {
//    lv_obj_t *canvas = (lv_obj_t *)user_data;
//    lv_color_t *buf = lv_canvas_get_buf(canvas);
//    uint32_t canvas_w = lv_canvas_get_width(canvas);
//
//
//    // 逐像素转换为RGB565并写入画布
//    for (int row = 0; row < h; row++) {
//        for (int col = 0; col < w; col++) {
//            uint8_t r = rgba[(row * w + col) * 4 + 0];
//            uint8_t g = rgba[(row * w + col) * 4 + 1];
//            uint8_t b = rgba[(row * w + col) * 4 + 2];
//            uint16_t rgb565 = rgba8888_to_rgb565(r, g, b, 255);
//            buf[(y + row) * canvas_w + (x + col)] = lv_color_make(rgb565 >> 11, (rgb565 >> 5) & 0x3F, rgb565 & 0x1F);
//        }
//    }
//
//    // 局部刷新：只刷新当前行，避免整屏卡顿
//    lv_area_t area = {x, y, x + w - 1, y + h - 1};
//    lv_canvas_inv_area(canvas, &area);
//}
//
//// 加载TF卡中的PNG文件（分块解码，适配大图片）
//lv_obj_t *png_lvgl_load(const char *png_path, lv_obj_t *parent) {
//    // 1. 创建LVGL画布（替代lv_img，支持逐行渲染）
//    lv_obj_t *canvas = lv_canvas_create(parent);
//    // 关键修改：LV_IMG_CF_TRUE_COLOR_16 → LV_IMG_CF_TRUE_COLOR
//    lv_canvas_set_buffer(canvas, (lv_color_t *)lv_mem_alloc(240*240*2), 240, 240, LV_IMG_CF_TRUE_COLOR);
//
//    // 2. 打开TF卡中的PNG文件
//    FIL fp;
//    FRESULT res = f_open(&fp, png_path, FA_READ);
//    if (res != FR_OK) {
//        lv_obj_t *err_label = lv_label_create(canvas);
//        lv_label_set_text(err_label, "PNG load fail");
//        return canvas;
//    }
//
//    // 3. 初始化pngle解码器
//    pngle_t *pngle = pngle_init();
//    pngle->on_draw = png_draw_cb;
//    pngle->user_data = canvas;
//
//    // 4. 分块读取PNG数据（每次512字节，避免一次性加载大文件）
//    uint8_t buf[512];
//    UINT br;
//    while (f_read(&fp, buf, sizeof(buf), &br) == FR_OK && br > 0) {
//        pngle_feed(pngle, buf, br);
//    }
//
//    // 5. 释放资源
//    f_close(&fp);
//    pngle_destroy(pngle);
//
//    return canvas;
//}
//
//#endif // PNG_LVGL_H
