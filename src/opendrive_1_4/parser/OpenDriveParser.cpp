#include "uos_odrparser/opendrive_1_4/parser/OpenDriveParser.hpp"

#include "uos_odrparser/opendrive_1_4/parser/ControllerParser.h"
#include "uos_odrparser/opendrive_1_4/parser/GeoReferenceParser.h"
#include "uos_odrparser/opendrive_1_4/parser/GeometryParser.h"
#include "uos_odrparser/opendrive_1_4/parser/JunctionParser.h"
#include "uos_odrparser/opendrive_1_4/parser/LaneParser.h"
#include "uos_odrparser/opendrive_1_4/parser/ObjectParser.h"
#include "uos_odrparser/opendrive_1_4/parser/ProfilesParser.h"
#include "uos_odrparser/opendrive_1_4/parser/RoadLinkParser.h"
#include "uos_odrparser/opendrive_1_4/parser/TrafficGroupParser.h"
#include "uos_odrparser/opendrive_1_4/parser/TrafficSignParser.h"
#include "uos_odrparser/opendrive_1_4/parser/TrafficSignalsParser.h"

#include <pugixml.hpp>

namespace opendrive_1_4
{
namespace parser
{

bool OpenDriveParser::Parse(const char *xml,
                            opendrive_1_4::OpenDriveData &out_open_drive_data,
                            XmlInputType inputType, std::string *out_error)
{
    namespace odp = opendrive_1_4::parser;
    int road_count;

    pugi::xml_document xmlDoc;
    pugi::xml_parse_result pugiParseResult;

    unsigned int const parse_options_ensure_no_doctype =
        pugi::parse_default & ~pugi::parse_doctype;
    switch (inputType)
    {
        case XmlInputType::FILE:
        {
            pugiParseResult =
                xmlDoc.load_file(xml, parse_options_ensure_no_doctype);
        }
        break;

        case XmlInputType::CONTENT:
        {
            pugiParseResult =
                xmlDoc.load_string(xml, parse_options_ensure_no_doctype);
        }
        break;

        default:
        {
            printf("OpenDriveParser::Parse >> invalid XmlInputType "
                   "provided {%d}.\n",
                   (int)inputType);
            return false;
        }
        break;
    }

    if (pugiParseResult == false)
    {
        if (out_error != nullptr)
        {
            *out_error = pugiParseResult.description();
        }

        return false;
    }

    auto revMinor = xmlDoc.child("OpenDRIVE")
                        .child("header")
                        .attribute("revMinor")
                        .as_int();
    switch (revMinor)
    {
        case 4:
            break;
        default:
            printf("Maps of the revision 1.%d are not supported.", revMinor);
            return false;
    }

    out_open_drive_data.geoReference = odp::GeoReferenceParser::Parse(
        xmlDoc.child("OpenDRIVE").child("header").child_value("geoReference"));

    // record: road; instances: 1+;
    road_count = 0;
    for (pugi::xml_node road = xmlDoc.child("OpenDRIVE").child("road"); road;
         road = road.next_sibling("road"))
    {
        opendrive_1_4::RoadInformation openDriveRoadInformation;

        openDriveRoadInformation.attributes.name =
            road.attribute("name").as_string();
        openDriveRoadInformation.attributes.id = road.attribute("id").as_int();
        openDriveRoadInformation.attributes.length =
            road.attribute("length").as_double();
        openDriveRoadInformation.attributes.junction =
            road.attribute("junction").as_int();

        // record: road type; instances: 0+;
        for (pugi::xml_node node_type : road.children("type"))
        {
            RoadTypeInfo type{0.0, ""};

            type.s = node_type.attribute("s").as_double();
            type.type = node_type.attribute("type").as_string();
            openDriveRoadInformation.attributes.type.emplace_back(type);

            // record: speed; instances: 0..1;
            pugi::xml_node speed_node = node_type.child("speed");
            if (speed_node)
            {
                RoadSpeed speed{0.0, 0.0, ""};
                speed.s = type.s;
                speed.max = speed_node.attribute("max").as_double();
                speed.unit = speed_node.attribute("unit").as_string();
                openDriveRoadInformation.attributes.speed.emplace_back(speed);
            }
        }

        // record: road plan view; instances: 1;
        odp::GeometryParser::Parse(
            road.child("planView"),
            openDriveRoadInformation.geometry_attributes);

        // record: road elevation/lateral profile; instances: 0..1;
        odp::ProfilesParser::Parse(road,
                                   openDriveRoadInformation.road_profiles);

        // record: lanes; instances: 1;
        odp::LaneParser::Parse(road.child("lanes"),
                               openDriveRoadInformation.lanes);

        odp::ObjectParser::Parse(road, openDriveRoadInformation.road_objects);

        // record: road link; instances: 0..1; doc: 5.3.2;
        odp::RoadLinkParser::Parse(road.child("link"),
                                   openDriveRoadInformation.road_link);
        odp::TrafficSignalsParser::Parse(
            road.child("signals"), openDriveRoadInformation.traffic_signals,
            openDriveRoadInformation.traffic_signal_references);

        odp::ControllerParser::Parse(road, out_open_drive_data.controllers,
                                     out_open_drive_data.controllersignals);

        out_open_drive_data.roads.emplace_back(
            std::move(openDriveRoadInformation));
        out_open_drive_data.roads_map.emplace(road.attribute("id").as_string(),
                                              road_count++);
    }

    // Extracting junction information
    for (pugi::xml_node junction = xmlDoc.child("OpenDRIVE").child("junction");
         junction; junction = junction.next_sibling("junction"))
    {
        odp::JunctionParser::Parse(junction, out_open_drive_data.junctions);
    }

    for (pugi::xml_node trafficsigns =
             xmlDoc.child("OpenDRIVE").child("trafficsign");
         trafficsigns; trafficsigns = trafficsigns.next_sibling("trafficsign"))
    {
        odp::TrafficSignParser::Parse(trafficsigns,
                                      out_open_drive_data.trafficsigns);
    }

    return true;
}
} // namespace parser
} // namespace opendrive_1_4
