#ifndef __TEMPLATE_MODEL_H
#define __TEMPLATE_MODEL_H
#include <string>
#include <vector>
#include "lvgl/lvgl.h"
//#include"Utils/RouteParser/RouteParser.h"

namespace Page
{

class TemplateModel
{
public:
    uint32_t TickSave;
    uint32_t GetData();


    struct RawCoordinates {
        double latitude;
        double lontitude;
        double altitude;
    };
    std::vector<RawCoordinates> rawCoordinates;
    int min_altitude;
    int max_altitude;

    double Distance;
    int Ascent;
    

    void KMLParser();

    double CalcDistance(std::vector<RawCoordinates>& points,int index_begin = 0,int index_end = -1);

    int CalcAscent(std::vector<RawCoordinates>& points, int index_begin = 0, int index_end = -1);

    void UpdateData(std::vector<RawCoordinates>& points, int index_begin = 0, int index_end = -1);

private:

    const std::string parserPath = "C:\\Users\\����\\Desktop\\X-TRACK-2.7\\Software\\X-Track\\USER\\App\\Resource\\KML_Resource\\test1.kml";

};

}

#endif
