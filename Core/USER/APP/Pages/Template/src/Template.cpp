#include "Template.h"
//#include"Utils/FileWrite/FileWrite.h"
using namespace Page;

Template::Template()
    : timer(nullptr)
{
}

Template::~Template()
{

}

void Template::UpdateChart(int index_begin, int index_end) {


    if (index_end == -1) {
        int size_points = Model.rawCoordinates.size();

        Model.UpdateData(Model.rawCoordinates, 0, size_points -1);
        lv_label_set_text_fmt(View.segmentChart.label_altitude_max, "%d", Model.max_altitude);
        lv_label_set_text_fmt(View.segmentChart.label_altitude_min, "%d", Model.min_altitude);
        lv_label_set_text_fmt(View.segmentChart.label_distance_end, "%.2f", Model.Distance);
        lv_label_set_text_fmt(View.segmentChart.label_distance, "%.2f", Model.Distance);
        lv_label_set_text_fmt(View.segmentChart.label_altitude, "%d", Model.Ascent);

        const uint32_t point_cnt = static_cast<uint32_t>(size_points);

        lv_coord_t minv = Model.rawCoordinates[index_begin].altitude, maxv = Model.rawCoordinates[index_begin].altitude;
        for (uint32_t i = 1; i < point_cnt; i++) {
            if (Model.rawCoordinates[i].altitude < minv) minv = Model.rawCoordinates[i].altitude;
            if (Model.rawCoordinates[i].altitude > maxv) maxv = Model.rawCoordinates[i].altitude;
        }
        lv_coord_t margin = (maxv - minv) / 10 + 1;

        lv_chart_set_range(View.segmentChart.chart, LV_CHART_AXIS_PRIMARY_Y, minv - margin, maxv + margin);
        lv_chart_set_point_count(View.segmentChart.chart, point_cnt);

        for (uint32_t i = 0; i < point_cnt; i++) {
            lv_chart_set_next_value(View.segmentChart.chart, View.segmentChart.ser, Model.rawCoordinates[i].altitude);
        }
        lv_chart_refresh(View.segmentChart.chart);
    }

    else {

        Model.UpdateData(Model.rawCoordinates, index_begin, index_end);
        lv_label_set_text_fmt(View.segmentChart.label_altitude_max, "%d", Model.max_altitude);
        lv_label_set_text_fmt(View.segmentChart.label_altitude_min, "%d", Model.min_altitude);
        lv_label_set_text_fmt(View.segmentChart.label_distance_end, "%.2f", Model.Distance);
        lv_label_set_text_fmt(View.segmentChart.label_distance, "%.2f", Model.Distance);
        lv_label_set_text_fmt(View.segmentChart.label_altitude, "%d", Model.Ascent);

        const uint32_t point_cnt = static_cast<uint32_t>(index_end- index_begin);

        lv_coord_t minv = Model.rawCoordinates[index_begin].altitude, maxv = Model.rawCoordinates[index_begin].altitude;
        for (uint32_t i = index_begin; i < index_end; i++) {
            if (Model.rawCoordinates[i].altitude < minv) minv = Model.rawCoordinates[i].altitude;
            if (Model.rawCoordinates[i].altitude > maxv) maxv = Model.rawCoordinates[i].altitude;
        }
        lv_coord_t margin = (maxv - minv) / 10 + 1;

        lv_chart_set_range(View.segmentChart.chart, LV_CHART_AXIS_PRIMARY_Y, minv - margin, maxv + margin);
        lv_chart_set_point_count(View.segmentChart.chart, point_cnt);


        lv_chart_set_all_value(View.segmentChart.chart, View.segmentChart.ser, LV_CHART_POINT_NONE);

        for (uint32_t i = index_begin; i < index_end; i++) {
            lv_chart_set_next_value(View.segmentChart.chart, View.segmentChart.ser, Model.rawCoordinates[i].altitude);
        }
        lv_chart_refresh(View.segmentChart.chart);
    }

};


static int counter = 0;
static lv_timer_t* counter_timer = nullptr;
static int counter_chart = 0;

static void counter_timer_cb(lv_timer_t* timer) {
    Template* instance = (Template*)timer->user_data;
    instance->updateCounter();
}


void Template::updateCounter() {
 //   FileWrite Test1;

    counter++;
    if (counter >= 210) {
        counter = 0;
        counter_chart += 1;
    }


    lv_obj_set_x(View.segmentChart.line_pin, View.segmentChart.x + counter);
    if (counter_chart < 5) {
        UpdateChart(counter_chart * 210, (counter_chart + 1) * 210 - 1);
    }
    else {
        counter_chart = 0;
        UpdateChart(counter_chart * 210, (counter_chart + 1) * 210 - 1);
    }

}

void Template::onCustomAttrConfig()
{
    LV_LOG_USER(__func__);
    SetCustomCacheEnable(true);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 1000, lv_anim_path_bounce);
}

void Template::onViewLoad()
{
    LV_LOG_USER(__func__);
    AttachEvent(_root);
//    View.Create(_root);
    View.InitChart(_root);
    Model.KMLParser();



    UpdateChart(0, 210);
//   UpdateChart(210, 420);
    //UpdateChart(629, 839);
    counter_timer = lv_timer_create(counter_timer_cb, 20, this);
    
}

void Template::onViewDidLoad()
{
    LV_LOG_USER(__func__);
}

void Template::onViewWillAppear()
{

    LV_LOG_USER(__func__);
    Param_t param;
    param.color = lv_color_white();
    param.time = 1000;

    PAGE_STASH_POP(param);

    lv_obj_set_style_bg_color(_root, param.color, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(_root, LV_OPA_COVER, LV_PART_MAIN);
    timer = lv_timer_create(onTimerUpdate, param.time, this);

}

void Template::onViewDidAppear()
{
    LV_LOG_USER(__func__);
}

void Template::onViewWillDisappear()
{
    LV_LOG_USER(__func__);
}

void Template::onViewDidDisappear()
{
    LV_LOG_USER(__func__);
    lv_timer_del(timer);
}

void Template::onViewUnload()
{
    LV_LOG_USER(__func__);
}

void Template::onViewDidUnload()
{
    LV_LOG_USER(__func__);
}

void Template::AttachEvent(lv_obj_t* obj)
{
    lv_obj_set_user_data(obj, this);
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Template::Update()
{
    updateCounter();
}

void Template::onTimerUpdate(lv_timer_t* timer)
{
    Template* instance = (Template*)timer->user_data;

    instance->Update();
}

void Template::onEvent(lv_event_t* event)
{
    Template* instance = (Template*)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t* obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (obj == instance->_root)
    {
        if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LEAVE)
        {
            instance->_Manager->Pop();
        }
    }
}

