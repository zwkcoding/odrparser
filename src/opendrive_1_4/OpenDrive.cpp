#include "uos_odrparser/opendrive_1_4/OpenDrive.hpp"

#include "uos_odrparser/opendrive_1_4/geometry/GeometryGenerator.hpp"
#include "uos_odrparser/opendrive_1_4/parser/OpenDriveParser.hpp"

namespace opendrive_1_4
{

bool Load(std::string const &file,
          opendrive_1_4::OpenDriveData &open_drive_data)
{
    return parser::OpenDriveParser::Parse(file.c_str(), open_drive_data,
                                          parser::XmlInputType::FILE);
}

bool Parse(std::string const &content,
           opendrive_1_4::OpenDriveData &open_drive_data)
{
    return parser::OpenDriveParser::Parse(content.c_str(), open_drive_data,
                                          parser::XmlInputType::CONTENT);
}

bool GenerateLaneMap(opendrive_1_4::OpenDriveData &open_drive_data,
                     double const overlapMargin)
{
    return geometry::GenerateGeometry(open_drive_data, overlapMargin);
}

const RoadInformation &getRoad(const std::string &id,
                               const OpenDriveData &open_drive_data)
{
    const auto &it = open_drive_data.roads_map.find(id);
    if (it != open_drive_data.roads_map.end())
    {
        return open_drive_data.roads.at(it->second);
    }
    throw std::invalid_argument("Could not find a road with the id " + id);
}

const geom::GeoLocation &getGeoReference(const OpenDriveData &open_drive_data)
{
    return open_drive_data.geoReference;
}

const std::string &getRoadName(const RoadInformation &road)
{
    return road.attributes.name;
}

int getRoadId(const RoadInformation &road) { return road.attributes.id; }

int getRoadJunctionId(const RoadInformation &road)
{
    return road.attributes.junction;
}

double getRoadLength(const RoadInformation &road)
{
    return road.attributes.length;
}

const std::vector<RoadTypeInfo> &getRoadType(const RoadInformation &road)
{
    return road.attributes.type;
}

const std::vector<RoadSpeed> &getRoadSpeed(const RoadInformation &road)
{
    return road.attributes.speed;
}

} // namespace opendrive_1_4
