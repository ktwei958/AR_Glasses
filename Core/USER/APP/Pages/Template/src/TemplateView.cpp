#include "TemplateView.h"
#include <stdarg.h>
#include <stdio.h>
//#include"Utils/RouteParser/RouteParser.h"

#include <algorithm>  // std::max_element

#define COLOR_ORANGE    lv_color_hex(0xff931e)
//�Ѿ�д�˼�ֵ����ԣ�������һ���༫ֵ�㣬����������ͨ����ֵ��������жϣ��ж������½�������ȡ���յĺ������ͼ�����꣬��ͼ��

using namespace Page;

//��ʼ��ͼ�����ʽ
void TemplateView::InitChart(lv_obj_t* root) {
	//�޸�ͼ��ߴ�
	const int lenth = 210;
	const int High = 80;
	const int l_gap = -10;
	const int h_gap = 50;

	//��ʼ������
	layout.background = lv_obj_create(root);
	lv_obj_set_size(layout.background, 240, 240);
	lv_obj_set_style_bg_color(layout.background, lv_color_hex(0x000000), 0);
	lv_obj_center(layout.background);
	lv_obj_set_style_bg_opa(layout.background, LV_OPA_COVER, 0);

	////��ʼ�����
	segmentChart.chart = lv_chart_create(root);
	lv_obj_set_size(segmentChart.chart, lenth, High);
	lv_obj_align(segmentChart.chart, LV_ALIGN_TOP_MID, l_gap, h_gap);
	//�����Ƴ�������ģʽ
	lv_chart_set_update_mode(segmentChart.chart, LV_CHART_UPDATE_MODE_SHIFT);



	//���÷ָ���
	segmentChart.line_dive_left = lv_obj_create(root);
	lv_obj_set_size(segmentChart.line_dive_left, 2, High+15);   // �� �� ��
	lv_obj_set_style_bg_color(segmentChart.line_dive_left, lv_color_hex(0x843636), 0);
	lv_obj_set_style_bg_opa(segmentChart.line_dive_left, LV_OPA_COVER, 0);
	lv_obj_set_style_border_width(segmentChart.line_dive_left, 0, 0);
	lv_obj_align_to(segmentChart.line_dive_left, segmentChart.chart, LV_ALIGN_TOP_LEFT,0,0);

	segmentChart.line_dive_right = lv_obj_create(root);
	lv_obj_set_size(segmentChart.line_dive_right, 2, High + 15);   // �� �� ��
	lv_obj_set_style_bg_color(segmentChart.line_dive_right, lv_color_hex(0x843636), 0);
	lv_obj_set_style_bg_opa(segmentChart.line_dive_right, LV_OPA_COVER, 0);
	lv_obj_set_style_border_width(segmentChart.line_dive_right, 0, 0);
	lv_obj_align_to(segmentChart.line_dive_right, segmentChart.chart, LV_ALIGN_TOP_RIGHT, 0, 0);

	segmentChart.line_dive_top = lv_obj_create(root);
	lv_obj_set_size(segmentChart.line_dive_top, lenth, 2);   // �� �� ��
	lv_obj_set_style_bg_color(segmentChart.line_dive_top, lv_color_hex(0x843636), 0);
	lv_obj_set_style_bg_opa(segmentChart.line_dive_top, LV_OPA_COVER, 0);
	lv_obj_set_style_border_width(segmentChart.line_dive_top, 0, 0);
	lv_obj_align_to(segmentChart.line_dive_top, segmentChart.chart, LV_ALIGN_TOP_MID, 0, 0);

	segmentChart.line_dive_bottom = lv_obj_create(root);
	lv_obj_set_size(segmentChart.line_dive_bottom, lenth, 2);   // �� �� ��
	lv_obj_set_style_bg_color(segmentChart.line_dive_bottom, lv_color_hex(0x843636), 0);
	lv_obj_set_style_bg_opa(segmentChart.line_dive_bottom, LV_OPA_COVER, 0);
	lv_obj_set_style_border_width(segmentChart.line_dive_bottom, 0, 0);
	lv_obj_align_to(segmentChart.line_dive_bottom, segmentChart.chart, LV_ALIGN_BOTTOM_MID, 0, 0);

	//���߷ָ���
	static lv_point_t line_pts[] = { {0, 0},{210, 0} };
	
	segmentChart.dotted_line = lv_line_create(root);
	lv_line_set_points(segmentChart.dotted_line, line_pts, 2);
	lv_obj_set_style_line_width(segmentChart.dotted_line, 1, 0);
	lv_obj_set_style_line_color(segmentChart.dotted_line, lv_color_hex(0x843636), 0);
	lv_obj_set_style_line_dash_width(segmentChart.dotted_line, 1, 0);
	lv_obj_set_style_line_dash_gap(segmentChart.dotted_line, 3, 0);

	lv_obj_set_width(segmentChart.dotted_line, lv_obj_get_width(segmentChart.chart));
	lv_obj_align_to(segmentChart.dotted_line, segmentChart.chart,LV_ALIGN_CENTER,0, 0);


	//��������
	segmentChart.label_altitude_text = lv_label_create(root);
	lv_label_set_text(segmentChart.label_altitude_text, "ASCENT");
	lv_obj_set_style_text_font(segmentChart.label_altitude_text, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(segmentChart.label_altitude_text, lv_color_hex(0x999999), 0);
	lv_obj_align_to(segmentChart.label_altitude_text, segmentChart.chart, LV_ALIGN_OUT_TOP_RIGHT, -20, -5);

	segmentChart.label_altitude_m = lv_label_create(root);
	lv_label_set_text(segmentChart.label_altitude_m, "M");
	lv_obj_set_style_text_font(segmentChart.label_altitude_m, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(segmentChart.label_altitude_m, lv_color_hex(0x999999), 0);
	lv_obj_align_to(segmentChart.label_altitude_m, segmentChart.label_altitude_text, LV_ALIGN_OUT_TOP_RIGHT, 0, -2);

	segmentChart.label_altitude = lv_label_create(root);
	lv_label_set_text(segmentChart.label_altitude, "000");
	lv_obj_set_style_text_font(segmentChart.label_altitude, &lv_font_montserrat_24, 0);
	lv_obj_set_style_text_color(segmentChart.label_altitude, lv_color_hex(0xFFFFFF), 0);
	lv_obj_align_to(segmentChart.label_altitude, segmentChart.label_altitude_m, LV_ALIGN_OUT_LEFT_MID, 4, -4);

	//���þ���
	segmentChart.label_distance_text = lv_label_create(root);
	lv_label_set_text(segmentChart.label_distance_text, "DISTANCE");
	lv_obj_set_style_text_font(segmentChart.label_distance_text, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(segmentChart.label_distance_text, lv_color_hex(0x999999), 0);
	lv_obj_align_to(segmentChart.label_distance_text, segmentChart.chart, LV_ALIGN_OUT_TOP_LEFT, 20, -5);

	segmentChart.label_distance_km = lv_label_create(root);
	lv_label_set_text(segmentChart.label_distance_km, "KM");
	lv_obj_set_style_text_font(segmentChart.label_distance_km, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(segmentChart.label_distance_km, lv_color_hex(0x999999), 0);
	lv_obj_align_to(segmentChart.label_distance_km, segmentChart.label_distance_text, LV_ALIGN_OUT_TOP_RIGHT, 0, -2);

	segmentChart.label_distance = lv_label_create(root);
	lv_label_set_text(segmentChart.label_distance, "0.00");
	lv_obj_set_style_text_font(segmentChart.label_distance, &lv_font_montserrat_24, 0);
	lv_obj_set_style_text_color(segmentChart.label_distance, lv_color_hex(0xFFFFFF), 0);
	lv_obj_align_to(segmentChart.label_distance, segmentChart.label_distance_km, LV_ALIGN_OUT_LEFT_MID, -2, -4);

	//��������ͼ�ֶ�˵��
	segmentChart.label_altitude_max = lv_label_create(root);
	lv_label_set_text(segmentChart.label_altitude_max, "0");
	lv_obj_set_style_text_font(segmentChart.label_altitude_max, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(segmentChart.label_altitude_max, lv_color_hex(0x999999), 0);
	lv_obj_align_to(segmentChart.label_altitude_max, segmentChart.chart, LV_ALIGN_OUT_RIGHT_TOP,1,0);

	segmentChart.label_altitude_min = lv_label_create(root);
	lv_label_set_text(segmentChart.label_altitude_min, "0");
	lv_obj_set_style_text_font(segmentChart.label_altitude_min, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(segmentChart.label_altitude_min, lv_color_hex(0x999999), 0);
	lv_obj_align_to(segmentChart.label_altitude_min, segmentChart.chart, LV_ALIGN_OUT_RIGHT_BOTTOM,1, 0);

	segmentChart.label_distance_start = lv_label_create(root);
	lv_label_set_text(segmentChart.label_distance_start, "0.00");
	lv_obj_set_style_text_font(segmentChart.label_distance_start, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(segmentChart.label_distance_start, lv_color_hex(0x999999), 0);
	lv_obj_align_to(segmentChart.label_distance_start, segmentChart.chart, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 0);

	segmentChart.label_distance_end = lv_label_create(root);
	lv_label_set_text(segmentChart.label_distance_end, "0.00");
	lv_obj_set_style_text_font(segmentChart.label_distance_end, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(segmentChart.label_distance_end, lv_color_hex(0x999999), 0);
	lv_obj_align_to(segmentChart.label_distance_end, segmentChart.chart, LV_ALIGN_OUT_BOTTOM_RIGHT, -5, 0);

	segmentChart.line_pin = lv_obj_create(root);
	lv_obj_set_size(segmentChart.line_pin, 2, High-4);   // �� �� ��
	lv_obj_set_style_bg_color(segmentChart.line_pin, lv_color_hex(0xff3600), 0);
	lv_obj_set_style_bg_opa(segmentChart.line_pin, LV_OPA_COVER, 0);
	lv_obj_set_style_border_width(segmentChart.line_pin, 0, 0);
	lv_obj_align_to(segmentChart.line_pin, segmentChart.chart, LV_ALIGN_LEFT_MID, 0, 0);

	//����Ϊ���Դ���
	segmentChart.ser = lv_chart_add_series(segmentChart.chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
	lv_obj_set_style_line_color(segmentChart.chart, lv_palette_main(LV_PALETTE_RED), LV_PART_ITEMS);
	lv_obj_set_style_line_width(segmentChart.chart, 2, LV_PART_ITEMS);
	lv_obj_set_style_line_opa(segmentChart.chart, LV_OPA_COVER, LV_PART_ITEMS);
	
	//��ȡ��Ļ���صĳ�ʼ����
	segmentChart.x = ((240-lenth)/2)+l_gap;
	lv_chart_refresh(segmentChart.chart);
}
void TemplateView::Create(lv_obj_t* root)
{
    lv_obj_t* cont = lv_obj_create(root);
    lv_obj_remove_style_all(cont);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(cont, 110, 50);
    lv_obj_set_style_border_color(cont, COLOR_ORANGE, 0);
    lv_obj_set_style_border_side(cont, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_width(cont, 3, 0);
    lv_obj_set_style_border_post(cont, true, 0);
    lv_obj_center(cont);
    ui.cont = cont;

    lv_obj_t* label = lv_label_create(cont);
    lv_obj_set_style_text_font(label,  &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text(label, "Template");
    lv_obj_center(label);
    ui.labelLogo = label;

    ui.anim_timeline = lv_anim_timeline_create();

#define ANIM_DEF(start_time, obj, attr, start, end) \
     {start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

    lv_anim_timeline_wrapper_t wrapper[] =
    {
        ANIM_DEF(0, ui.cont, width, 0, lv_obj_get_style_width(ui.cont, 0)),
        ANIM_DEF(500, ui.labelLogo, y, lv_obj_get_style_height(ui.cont, 0), lv_obj_get_y(ui.labelLogo)),
        LV_ANIM_TIMELINE_WRAPPER_END
    };

    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

