

#include "stdafx.h"
#include "GoogleMapHTMLGenerator.h"
#include "GMMapData.h"

namespace mray
{
namespace google
{
	
void GoogleMapHTMLGenerator::Generate(GMMapData* map)
{
	core::string result;
	result="<!DOCTYPE html>"\
		"<html>\n"\
		"<head>\n"\
		"<title>Google Maps Generated Page by MRAY Engine</title>\n"\
		"<link href=\"/maps/documentation/javascript/examples/default.css\" rel=\"stylesheet\">\n"\
		"<script src=\"https://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false\"></script>\n"\
		"<script src=\"https://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false&libraries=weather\"></script>\n"\
		"<script>\n"\
		"var map;\n"\
		"function initialize() {\n";

}


}
}

