/*
 * kml_parse.cpp
 *
 *  Created on: Jan 7, 2026
 *      Author: wkt98
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lvgl.h"
#include "track_param.h"
#include "kml_parse.h"



/* 解析器配置 */
#define KML_READ_CHUNK_SIZE  512   // 读取块大小
#define MAX_NUM_BUFFER       32    // 浮点数字符串缓冲
#define MIN_COORD_COUNT      2     // 至少需要2个坐标（经度+纬度）
typedef enum {
    STATE_SEARCH_TAG,      // 寻找 <coordinates>
    STATE_PARSE_CONTENT,   // 解析内容
    STATE_FOUND_EXIT       // 备用状态
} ParserState_t;

/**
 * @brief 使用 LVGL FS API 解析 KML
 */
int kml_parse_coordinates(const char* filepath, std::vector<RawCoordinates>& out_track) {
    lv_fs_file_t file;
    lv_fs_res_t res;

    // 1. 打开文件 (LVGL API)
    // 注意：LVGL 路径通常需要驱动器前缀，如 "S:/path/to/file.kml"
    res = lv_fs_open(&file, filepath, LV_FS_MODE_RD);
    if (res != LV_FS_RES_OK) {
        LV_LOG_USER("Error opening KML file: %s (Res: %d)", filepath, res);
        return -1;
    }

    // [优化] 预分配内存
    out_track.clear();
    out_track.reserve(2000);

    char buffer[KML_READ_CHUNK_SIZE];
    uint32_t bytes_read = 0; // LVGL read 接口通常使用 uint32_t
    ParserState_t state = STATE_SEARCH_TAG;

    char num_buf[MAX_NUM_BUFFER];
    int num_idx = 0;
    int tuple_idx = 0; // 0=Lon, 1=Lat, 2=Alt

    const char* TAG_START = "<gx:coord>";
    const char* TAG_END = "</gx:coord>";
    int match_idx = 0;

    // [新增] 临时变量
    RawCoordinates temp_point = { 0.0f, 0.0f, 0.0f };

    while (1) {
        // 2. 读取文件 (LVGL API)
        res = lv_fs_read(&file, buffer, KML_READ_CHUNK_SIZE, &bytes_read);

        // 错误检查
        if (res != LV_FS_RES_OK) {
            LV_LOG_USER("Error reading KML file (Res: %d)", res);
            lv_fs_close(&file);
            return -1;
        }

        // EOF 检查：如果读取到的字节数为 0，说明文件结束
        if (bytes_read == 0) {
            break;
        }

        for (uint32_t i = 0; i < bytes_read; i++) {
            char c = buffer[i];

            if (state == STATE_SEARCH_TAG) {
                if (c == TAG_START[match_idx]) {
                    match_idx++;
                    if (TAG_START[match_idx] == '\0') {
                        state = STATE_PARSE_CONTENT;
                        match_idx = 0;
                        tuple_idx = 0;
                        num_idx = 0;
                        // 重置临时点
                        temp_point = { 0.0f, 0.0f, 0.0f };
                    }
                }
                else {
                    match_idx = 0;
                    if (c == TAG_START[0]) match_idx = 1;
                }
            }
            else if (state == STATE_PARSE_CONTENT) {
                // A. 检查结束标签
                if (c == TAG_END[match_idx]) {
                    match_idx++;
                    if (TAG_END[match_idx] == '\0') {
                        state = STATE_SEARCH_TAG;
                        match_idx = 0;

                        // 处理 buffer 中残留的最后一个数字（通常是海拔）
                        if (num_idx > 0) {
                            num_buf[num_idx] = '\0';
                            float val = strtof(num_buf, NULL);
                            num_idx = 0;

                            if (tuple_idx == 0) temp_point.longitude = val;
                            else if (tuple_idx == 1) temp_point.latitude = val;
                            else if (tuple_idx == 2) temp_point.altitude = val;

                            // 凑齐了经纬度，推入 vector
                            // 注意：这里保留了你原来代码的逻辑，必须 tuple_idx == 2 (即读完3个数) 才 push
                            // 如果有的数据没有海拔，这里可能需要改为 >= 1
                            if (tuple_idx == 2) {
                                out_track.push_back(temp_point);
                            }
                        }
                        continue;
                    }
                }
                else {
                    match_idx = 0;
                }

                // B. 解析数字
                if ((c >= '0' && c <= '9') || c == '-' || c == '.' || c == 'e' || c == 'E') {
                    if (num_idx < MAX_NUM_BUFFER - 1) {
                        num_buf[num_idx++] = c;
                    }
                }
                // C. 分隔符处理 (空格, 换行, 逗号)
                else if (c == ',' || c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                    if (num_idx > 0) {
                        num_buf[num_idx] = '\0'; // 封口
                        float val = strtof(num_buf, NULL);
                        num_idx = 0;

                        // 存入临时变量
                        if (tuple_idx == 0) temp_point.longitude = val;
                        else if (tuple_idx == 1) temp_point.latitude = val;
                        else if (tuple_idx == 2) temp_point.altitude = val;

                        if (c == ',' || c == ' ') {
                            tuple_idx++;
                        }
                        /* // 保留你的注释逻辑
                        else {
                            // 换行符或其他空白，通常意味着一个点的结束
                            if (tuple_idx >= 1) {
                                out_track.push_back(temp_point);
                                temp_point.altitude = 0.0f;
                            }
                            tuple_idx = 0;
                        }
                        */
                    }
                }
            }
        }
    }

    // 3. 关闭文件 (LVGL API)
    lv_fs_close(&file);

    // 处理文件末尾可能的残留数据
    if (state == STATE_PARSE_CONTENT && num_idx > 0) {
        num_buf[num_idx] = '\0';
        float val = strtof(num_buf, NULL);

        if (tuple_idx == 0) temp_point.longitude = val;
        else if (tuple_idx == 1) temp_point.latitude = val;
        else if (tuple_idx == 2) temp_point.altitude = val;

        if (tuple_idx >= 1) {
            out_track.push_back(temp_point);
        }
    }

    return out_track.size();
}
