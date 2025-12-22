#include "TemplateView.h"
#include <stdarg.h>
#include <stdio.h>
//#include"Utils/RouteParser/RouteParser.h"

#include <algorithm>  // std::max_element
#include <vector>

//#include"Utils/FileWrite/FileWrite.h"

using namespace Page;

void TemplateView::Create(lv_obj_t* root)
{
    lv_obj_t* label = lv_label_create(root);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);
    lv_label_set_text(label, "");
    ui.labelTitle = label;

    label = lv_label_create(root);
    lv_label_set_text(label, "");
    lv_obj_center(label);
    ui.labelTick = label;

}


//设置背景颜色
void TemplateView::SetBox(lv_obj_t* root) {
    ui.box = lv_obj_create(root);
    lv_obj_set_size(ui.box, 240, 240);
    lv_obj_set_style_bg_color(ui.box, lv_color_hex(0x1C1C1C), 0);
    lv_obj_center(ui.box);
    lv_obj_set_style_bg_opa(ui.box, LV_OPA_COVER, 0); 
}

void TemplateView::SetChart(lv_obj_t* root) {

    // 动态数组存储高度
    std::vector<lv_coord_t> points_val = { 80, 90, 78, 150, 200, 340, 75, 55, 60 };

    // 最大值与最小值
    auto [min_it, max_it] = std::minmax_element(points_val.begin(), points_val.end());
    size_t idx_min = std::distance(points_val.begin(), min_it);
    size_t idx_max = std::distance(points_val.begin(), max_it);
    const uint32_t point_cnt = static_cast<uint32_t>(points_val.size());

    //创建方块背景图
    chart.bg_box = lv_chart_create(root);
    lv_obj_set_size(chart.bg_box, 240, 120);
    // 设置背景
    lv_obj_set_style_bg_color(chart.bg_box,lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_bg_opa(chart.bg_box, LV_OPA_COVER, 0);
    // 设置圆角
    lv_obj_set_style_radius(chart.bg_box, 5, 0);
    // 设置边框（可选）
    lv_obj_set_style_border_width(chart.bg_box, 2, 0);
    lv_obj_set_style_border_color(chart.bg_box, lv_color_hex(0x843636), 0);

    // 创建折线图
    chart.chart1 = lv_chart_create(chart.bg_box);
    lv_obj_set_size(chart.chart1, 200, 100);
    lv_obj_set_style_pad_top(chart.chart1, 20, 0);
    lv_obj_align(chart.chart1, LV_ALIGN_RIGHT_MID, -3, 0);

    lv_obj_set_style_bg_color(chart.chart1, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_bg_opa(chart.chart1, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(chart.chart1, 2, 0);
    lv_obj_set_style_border_color(chart.chart1, lv_color_hex(0x843636), 0);
    lv_obj_set_style_border_opa(chart.chart1, LV_OPA_TRANSP, 50); 
    lv_obj_set_style_radius(chart.chart1, 4, 0);

    lv_chart_set_div_line_count(chart.chart1, 3, 0); 
    lv_obj_set_style_line_color(chart.chart1, lv_color_make(255, 255, 255), LV_PART_MAIN);
    lv_obj_set_style_line_width(chart.chart1, 1, LV_PART_MAIN);
    lv_obj_set_style_line_opa(chart.chart1, LV_OPA_30, LV_PART_MAIN);

    // 设置纵轴范围
    lv_coord_t minv = points_val[0];
    lv_coord_t maxv = points_val[0];
    for (uint32_t i = 1; i < point_cnt; i++) {
        if (points_val[i] < minv) minv = points_val[i];
        if (points_val[i] > maxv) maxv = points_val[i];
    }
    lv_coord_t margin = (maxv - minv) / 10 + 1;
    lv_chart_set_range(chart.chart1, LV_CHART_AXIS_PRIMARY_Y, minv - margin, maxv + margin);
    lv_chart_set_point_count(chart.chart1, point_cnt);

    // 添加折线序列
    lv_chart_series_t* ser1 = lv_chart_add_series(chart.chart1, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    // 设置折线样式
    lv_obj_set_style_line_color(chart.chart1, lv_palette_main(LV_PALETTE_RED), LV_PART_ITEMS);
    lv_obj_set_style_line_width(chart.chart1, 2, LV_PART_ITEMS);
    lv_obj_set_style_line_opa(chart.chart1, LV_OPA_COVER, LV_PART_ITEMS);

    // 填充折线图数据
    for (uint32_t i = 0; i < point_cnt; i++) {
        lv_chart_set_next_value(chart.chart1, ser1, points_val[i]);
    }

    lv_chart_refresh(chart.chart1);

    // 添加海拔标签
    chart.label_altitude_max = lv_label_create(chart.bg_box);
    chart.label_altitude_mid = lv_label_create(chart.bg_box);
    chart.label_altitude_min = lv_label_create(chart.bg_box);
    chart.label_meter = lv_label_create(chart.bg_box);

    lv_label_set_text_fmt(chart.label_altitude_max, "%d", idx_min);
    lv_label_set_text_fmt(chart.label_altitude_mid, "%d", ((idx_min + idx_max) / 2));
    lv_label_set_text_fmt(chart.label_altitude_min, "%d", idx_max);
    lv_label_set_text_fmt(chart.label_meter, "m");


    lv_obj_set_style_text_font(chart.label_altitude_max,&lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(chart.label_altitude_max, lv_color_hex(0xFFFFFF), 0); 
    lv_obj_align_to(chart.label_altitude_max, chart.chart1, LV_ALIGN_OUT_LEFT_MID, -8, -25);

    lv_obj_set_style_text_font(chart.label_altitude_mid, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(chart.label_altitude_mid, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align_to(chart.label_altitude_mid, chart.chart1, LV_ALIGN_OUT_LEFT_MID, -8, 20);

    lv_obj_set_style_text_font(chart.label_altitude_min, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(chart.label_altitude_min, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align_to(chart.label_altitude_min, chart.chart1, LV_ALIGN_OUT_LEFT_MID, -8, 60);

    lv_obj_set_style_text_font(chart.label_meter, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(chart.label_meter, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align_to(chart.label_meter, chart.chart1, LV_ALIGN_OUT_LEFT_TOP, -8, 0);

}


void TemplateView::SetUserValue(lv_obj_t* root) {

    lv_obj_t* line_style = lv_line_create(root);
    static lv_point_t line_points_distance[] = { {0, 0}, {180, 0} }; 
    lv_line_set_points(line_style, line_points_distance, 2);               
    lv_obj_set_style_line_width(line_style, 2, 0);              
    lv_obj_set_style_line_color(line_style, lv_color_hex(0xFFFFFF), 0); 
    lv_obj_center(line_style);                                 

    lv_obj_align(line_style, LV_ALIGN_CENTER, 0, -58);
    lv_obj_set_style_line_opa(line_style, LV_OPA_COVER, 0);


    user.label_heart = lv_label_create(root);
    user.label_speed = lv_label_create(root);
    user.label_time = lv_label_create(root);

    lv_label_set_text(user.label_speed, "04 : 32");
    lv_label_set_text(user.label_heart, "160bmp");
    lv_label_set_text(user.label_time, "13:57");


    lv_obj_set_style_text_font(user.label_speed,&lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(user.label_speed, lv_color_hex(0xFFFFFF), 0);   
    lv_obj_align_to(user.label_speed, line_style, LV_ALIGN_CENTER, -60, -15);
    //icon.lightning = show_icon(root, ResourcePool::GetImage("lightning"), lv_color_make(255, 67, 54));
    lv_obj_align_to(icon.lightning, user.label_speed, LV_ALIGN_OUT_LEFT_MID, 0, 1);


    lv_obj_set_style_text_font(user.label_heart, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(user.label_heart, lv_color_hex(0xFFFFFF), 0);  
    lv_obj_align_to(user.label_heart, line_style, LV_ALIGN_CENTER, 70, -15);
    //icon.heart = show_icon(root, ResourcePool::GetImage("heart"), lv_color_make(255, 67, 54));
    lv_obj_align_to(icon.heart, user.label_heart, LV_ALIGN_OUT_LEFT_MID, -5, 1);



    lv_obj_set_style_text_font(user.label_time, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(user.label_time, lv_color_hex(0xFFFFFF), 0);   
    lv_obj_align_to(user.label_time, lv_scr_act(), LV_ALIGN_CENTER, 5, -100);
    //icon.clock = show_icon(root, ResourcePool::GetImage("clock"), lv_color_make(255, 255, 255));
    lv_obj_align_to(icon.clock, user.label_time, LV_ALIGN_OUT_LEFT_MID, -5, -2);

}

void TemplateView::SetOutsideValue(lv_obj_t* root) {


    climb.bar_climb_progress = lv_bar_create(root);
    lv_obj_set_size(climb.bar_climb_progress, 180, 6);            
    lv_obj_align(climb.bar_climb_progress, LV_ALIGN_CENTER, 0, -24); 

    lv_bar_set_range(climb.bar_climb_progress, 0, 100);
    lv_bar_set_value(climb.bar_climb_progress, 45, LV_ANIM_OFF);

    lv_obj_set_style_bg_color(climb.bar_climb_progress, lv_color_hex(0xf44336), LV_PART_MAIN);      
    lv_obj_set_style_bg_opa(climb.bar_climb_progress, LV_OPA_COVER, LV_PART_MAIN);             
    lv_obj_set_style_radius(climb.bar_climb_progress, 0, LV_PART_MAIN);                        

    lv_obj_set_style_outline_color(climb.bar_climb_progress, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_outline_width(climb.bar_climb_progress, 2, 0);
    lv_obj_set_style_outline_pad(climb.bar_climb_progress, 1, 0); 

    lv_obj_set_style_bg_color(climb.bar_climb_progress, lv_color_hex(0x505051), LV_PART_INDICATOR); 
    lv_obj_set_style_radius(climb.bar_climb_progress, 0, LV_PART_INDICATOR);  

    climb.label_climb_now = lv_label_create(root);
    climb.label_climb_nextpro = lv_label_create(root);
    climb.label_climb_progess = lv_label_create(root);
    climb.label_climb_next = lv_label_create(root);

    lv_label_set_text(climb.label_climb_now, "218m");
    lv_label_set_text(climb.label_climb_nextpro, "76m");
    lv_label_set_text(climb.label_climb_next, "97m");
    lv_label_set_text(climb.label_climb_progess, "756m");

    lv_obj_set_style_text_font(climb.label_climb_now, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(climb.label_climb_now, lv_color_hex(0xFFFFFF), 0);  
    lv_obj_align_to(climb.label_climb_now, lv_scr_act(), LV_ALIGN_CENTER, 82, 5);
    //icon.climb_up = show_icon(root, ResourcePool::GetImage("climb_up"), lv_color_make(255, 255, 255));
    lv_obj_align_to(icon.climb_up, climb.label_climb_now
    		, LV_ALIGN_OUT_LEFT_MID, -5, 0);

    lv_obj_set_style_text_font(climb.label_climb_next, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(climb.label_climb_next, lv_color_hex(0xf44336), 0);  
    lv_obj_align_to(climb.label_climb_next, lv_scr_act(), LV_ALIGN_CENTER, 5, 5);
    //icon.climb_next = show_icon(root, ResourcePool::GetImage("climb_next"), lv_color_make(244, 67, 54));
    lv_obj_align_to(icon.climb_next, climb.label_climb_next, LV_ALIGN_OUT_LEFT_MID, 0, 0);

    lv_obj_set_style_text_font(climb.label_climb_nextpro, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(climb.label_climb_nextpro, lv_color_hex(0xFFFFFF), 0);  
    lv_obj_align_to(climb.label_climb_nextpro, lv_scr_act(), LV_ALIGN_CENTER, -70, 5);
    //icon.climb_down = show_icon(root, ResourcePool::GetImage("climb_down"), lv_color_make(255, 255, 255));
    lv_obj_align_to(icon.climb_down, climb.label_climb_nextpro, LV_ALIGN_OUT_LEFT_MID, -5, 0);


    lv_obj_set_style_text_font(climb.label_climb_progess,&lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(climb.label_climb_progess, lv_color_hex(0xFFFFFF), 0);   
    lv_obj_align_to(climb.label_climb_progess, climb.bar_climb_progress, LV_ALIGN_CENTER, 5, -15);

}


lv_obj_t* TemplateView::show_icon(lv_obj_t* parent, const void* img_src, lv_color_t color)
{
    if (parent == NULL) parent = lv_scr_act();   

    lv_obj_t* img = lv_img_create(parent);     
    lv_img_set_src(img, img_src);               

    lv_obj_set_style_img_recolor(img, color, 0);
    lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, 0);

    return img;
}
