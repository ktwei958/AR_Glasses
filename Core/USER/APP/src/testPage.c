#include "testPage.h"
#include "lvgl.h"

// 全局变量：用于滑块值更新
static lv_obj_t *slider_label;

// 静态变量：保存控件指针，只创建一次
static lv_obj_t *time_label = NULL;
static lv_obj_t *date_label = NULL;
static lv_obj_t *lat_label = NULL;
static lv_obj_t *lon_label = NULL;
static lv_obj_t *test_label = NULL;
static lv_obj_t *speed_label = NULL;
static lv_obj_t *direction_label = NULL;

/**
 * @brief 按钮点击事件回调（弹出提示）
 */
static void btn_click_event_cb(lv_event_t *e) {
	lv_obj_t *btn = lv_event_get_target(e);
	lv_obj_t *toast = lv_label_create(lv_scr_act());
	lv_label_set_text(toast, "Button Clicked!");
	lv_obj_set_style_text_align(toast, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_size(toast, 180, 40);
	lv_obj_align(toast, LV_ALIGN_CENTER, 0, 0);
	// 3秒后自动删除提示
	/*
	 lv_timer_t *timer = lv_timer_create(
	 [](lv_timer_t *t) { lv_obj_del(lv_timer_get_user_data(t)); },
	 3000, toast
	 );*/
}

/**
 * @brief 滑块值变化事件回调（更新标签显示）
 */
static void slider_value_change_cb(lv_event_t *e) {
	lv_obj_t *slider = lv_event_get_target(e);
	int32_t value = lv_slider_get_value(slider);
	// 更新标签显示当前滑块值
	lv_label_set_text_fmt(slider_label, "Brightness: %d%%", value);
}

/**
 * @brief 创建ST7789 240*240屏幕的LVGL测试页面
 */
void lvgl_st7789_test_page_create(void) {
	// 1. 获取/创建默认编码器组（用于控件交互）
	lv_group_t *default_group = lv_group_get_default();
	if (default_group == NULL) {
		default_group = lv_group_create();
		lv_group_set_default(default_group);
	}

	// 2. 创建顶部标题标签（适配240*240屏幕，居中顶部）
	lv_obj_t *title_label = lv_label_create(lv_scr_act());
	lv_label_set_text(title_label, "LVGL Test Page");
	lv_obj_set_style_text_font(title_label, &lv_font_montserrat_14, 0); // 适配屏幕的字体大小
	lv_obj_set_style_text_align(title_label, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_size(title_label, 220, 30); // 宽度不超过屏幕（240）
	lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10); // 顶部中间，偏移10px

	// 3. 创建开关控件（类似用户示例，适配240*240屏幕尺寸）
	lv_obj_t *switch_obj = lv_switch_create(lv_scr_act());
	lv_obj_set_size(switch_obj, 100, 60); // 尺寸适配240屏幕，避免过大
	lv_obj_align(switch_obj, LV_ALIGN_TOP_MID, 0, 50); // 标题下方，偏移50px
	lv_group_add_obj(default_group, switch_obj); // 绑定编码器

	// 4. 创建交互按钮（带点击事件）
	lv_obj_t *test_btn = lv_btn_create(lv_scr_act());
	lv_obj_set_size(test_btn, 80, 40); // 适配屏幕的按钮尺寸
	lv_obj_align(test_btn, LV_ALIGN_TOP_MID, 0, 120); // 开关下方，偏移120px
	// 按钮文本标签
	lv_obj_t *btn_label = lv_label_create(test_btn);
	lv_label_set_text(btn_label, "Click Me");
	lv_obj_center(btn_label);
	// 绑定点击事件
	lv_obj_add_event_cb(test_btn, btn_click_event_cb, LV_EVENT_CLICKED, NULL);
	lv_group_add_obj(default_group, test_btn); // 绑定编码器

	// 5. 创建滑块控件（控制示例：亮度）
	lv_obj_t *brightness_slider = lv_slider_create(lv_scr_act());
	lv_obj_set_size(brightness_slider, 180, 20); // 宽度适配屏幕（240）
	lv_obj_align(brightness_slider, LV_ALIGN_TOP_MID, 0, 170); // 按钮下方，偏移170px
	lv_slider_set_range(brightness_slider, 0, 100); // 范围：0~100%
	lv_slider_set_value(brightness_slider, 50, LV_ANIM_ON); // 默认值50%
	// 滑块值显示标签
	slider_label = lv_label_create(lv_scr_act());
	lv_label_set_text_fmt(slider_label, "Brightness: 50%%");
	lv_obj_set_size(slider_label, 120, 20);
	lv_obj_align_to(slider_label, brightness_slider, LV_ALIGN_BOTTOM_MID, 0,
			10); // 滑块下方
	// 绑定值变化事件
	lv_obj_add_event_cb(brightness_slider, slider_value_change_cb,
			LV_EVENT_VALUE_CHANGED, NULL);
	lv_group_add_obj(default_group, brightness_slider); // 绑定编码器

	// 6. 创建复选框控件（示例：使能背光）
	lv_obj_t *checkbox_obj = lv_checkbox_create(lv_scr_act());
	lv_checkbox_set_text(checkbox_obj, " Enable Backlight");
	lv_obj_set_size(checkbox_obj, 150, 30);
	lv_obj_align(checkbox_obj, LV_ALIGN_TOP_MID, 0, 210); // 滑块下方，适配240屏幕底部
	//   lv_checkbox_set_checked(checkbox_obj, true); // 默认勾选
	lv_group_add_obj(default_group, checkbox_obj); // 绑定编码器
}

void update_gps_display(lv_obj_t *parent, GPS_Data *data) {
	// 初始化控件（只执行一次）
		if (time_label == NULL) {
			lv_obj_t *parent = lv_scr_act();
			time_label = lv_label_create(parent);
			lv_obj_align(time_label, LV_ALIGN_TOP_LEFT, 5, 5);

			date_label = lv_label_create(parent);
			lv_obj_align_to(date_label, time_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);

			lat_label = lv_label_create(parent);
			lv_obj_align_to(lat_label, date_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);

			lon_label = lv_label_create(parent);
			lv_obj_align_to(lon_label, lat_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);

			speed_label = lv_label_create(parent);
			lv_obj_align_to(speed_label, lon_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
			direction_label = lv_label_create(parent);
						lv_obj_align_to(direction_label,speed_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
		}

		lv_label_set_text_fmt(time_label, "time: %02d:%02d:%02d", data->hour,
				data->minute, data->second);

		lv_label_set_text_fmt(date_label, "data: %04d-%02d-%02d", data->year,
				data->month, data->day);

		// 经纬度显示

		lv_label_set_text_fmt(lat_label, "lat: %.3f°%c", fabs(data->latitude),
				data->latitude >= 0 ? 'N' : 'S');

		lv_label_set_text_fmt(lon_label, "lon: %.3f°%c", fabs(data->longitude),
				data->longitude >= 0 ? 'E' : 'W');

		lv_label_set_text_fmt(speed_label, "speed: %.1f km/h", data->speed_km_h);
		lv_label_set_text_fmt(direction_label, "direction: %.1f°", data->course);

}

void update_gps_display_forTestUpdateFre(lv_obj_t *parent, GPS_Data *data,
		int testCount) {
	// 初始化控件（只执行一次）
	if (time_label == NULL) {
		lv_obj_t *parent = lv_scr_act();
		time_label = lv_label_create(parent);
		lv_obj_align(time_label, LV_ALIGN_TOP_LEFT, 5, 5);

		date_label = lv_label_create(parent);
		lv_obj_align_to(date_label, time_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);

		lat_label = lv_label_create(parent);
		lv_obj_align_to(lat_label, date_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);

		lon_label = lv_label_create(parent);
		lv_obj_align_to(lon_label, lat_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);

		test_label = lv_label_create(parent);
		lv_obj_align_to(test_label, lon_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
	}
	lv_label_set_text_fmt(time_label, "time: %02d:%02d:%02d", data->hour,
			data->minute, data->second);

	lv_label_set_text_fmt(date_label, "data: %04d-%02d-%02d", data->year,
			data->month, data->day);

	// 经纬度显示

	lv_label_set_text_fmt(lat_label, "lat: %.3f°%c", fabs(data->latitude),
			data->latitude >= 0 ? 'N' : 'S');

	lv_label_set_text_fmt(lon_label, "lon: %.3f°%c", fabs(data->longitude),
			data->longitude >= 0 ? 'E' : 'W');

	lv_label_set_text_fmt(test_label, "testCount: %d", testCount);
}

