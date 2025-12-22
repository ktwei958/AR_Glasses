#ifndef __TEMPLATE_VIEW_H
#define __TEMPLATE_VIEW_H

#include "Page.h"//这个是开源项目里面设置调整显示页面的。你可以注释掉，然后按你的思路去显示即可
#include "lvgl/lvgl.h"

namespace Page
{

    class TemplateView
    {
    public:
        void Create(lv_obj_t* root);
        void SetBox(lv_obj_t* root);
        void SetChart(lv_obj_t* root);
        void SetUserValue(lv_obj_t* root);
        void SetOutsideValue(lv_obj_t* root);
    public:
        struct
        {
            lv_obj_t* labelTitle;
            lv_obj_t* labelTick;
            lv_obj_t* box;

        } ui;

        struct {
            lv_obj_t* label_altitude_min;
            lv_obj_t* label_altitude_mid;
            lv_obj_t* label_altitude_max;
            lv_obj_t* label_meter;
            lv_obj_t* bg_box;
            lv_obj_t* chart1;
        }chart;

        struct {
            lv_obj_t* label_speed;
            lv_obj_t* label_heart;
            lv_obj_t* label_time;
        }user;

        struct {
            lv_obj_t* label_climb_progess;
            lv_obj_t* label_climb_next;
            lv_obj_t* label_climb_now;
            lv_obj_t* label_climb_nextpro;
            lv_obj_t* bar_climb_progress;
        }climb;

        struct {
            lv_obj_t* clock;
            lv_obj_t* lightning;
            lv_obj_t* heart;
            lv_obj_t* climb_up;
            lv_obj_t* climb_next;
            lv_obj_t* climb_down;
        }icon;
    private:
        lv_obj_t* show_icon(lv_obj_t* parent, const void* img_src, lv_color_t color);

    };

}

#endif // !__VIEW_H
