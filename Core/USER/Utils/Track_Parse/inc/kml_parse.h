/*
 * kml_parse.h
 *
 *  Created on: Jan 7, 2026
 *      Author: wkt98
 */

#include "main.h"
#include "track_param.h"
#include <vector>

#ifndef USER_UTILS_TRACK_PARSE_KML_PARSE_H_
#define USER_UTILS_TRACK_PARSE_KML_PARSE_H_


int kml_parse_coordinates(const char* filepath, std::vector<RawCoordinates>& out_track);


#endif /* USER_UTILS_TRACK_PARSE_KML_PARSE_H_ */
