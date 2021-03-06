/*
 * ----------------- BEGIN LICENSE BLOCK ---------------------------------
 *
 * Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
 * de Barcelona (UAB).
 * Copyright (C) 2019-2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 * ----------------- END LICENSE BLOCK -----------------------------------
 */

#include "uos_odrparser/opendrive_1_4/parser/LaneParser.h"
#include <algorithm>
#include <cctype>
#include <string>

namespace opendrive_1_4
{
namespace parser
{

LaneType toLaneType(std::string const laneType)
{
    if (laneType == "none")
    {
        return LaneType::None;
    }
    else if (laneType == "driving")
    {
        return LaneType::Driving;
    }
    else if (laneType == "stop")
    {
        return LaneType::Stop;
    }
    else if (laneType == "shoulder")
    {
        return LaneType::Shoulder;
    }
    else if (laneType == "biking")
    {
        return LaneType::Biking;
    }
    else if (laneType == "sidewalk")
    {
        return LaneType::Sidewalk;
    }
    else if (laneType == "border")
    {
        return LaneType::Border;
    }
    else if (laneType == "restricted")
    {
        return LaneType::Restricted;
    }
    else if (laneType == "bidirectional")
    {
        return LaneType::Bidirectional;
    }
    else if (laneType == "parking")
    {
        return LaneType::Parking;
    }
    else if (laneType == "median")
    {
        return LaneType::Median;
    }
    else if (laneType == "special1")
    {
        return LaneType::Special1;
    }
    else if (laneType == "special2")
    {
        return LaneType::Special2;
    }
    else if (laneType == "special3")
    {
        return LaneType::Special3;
    }
    else if (laneType == "roadWorks")
    {
        return LaneType::RoadWorks;
    }
    else if (laneType == "tram")
    {
        return LaneType::Tram;
    }
    else if (laneType == "rail")
    {
        return LaneType::Rail;
    }
    else if (laneType == "entry")
    {
        return LaneType::Entry;
    }
    else if (laneType == "exit")
    {
        return LaneType::Exit;
    }
    else if (laneType == "offRamp")
    {
        return LaneType::OffRamp;
    }
    else if (laneType == "onRamp")
    {
        return LaneType::OnRamp;
    }
    else
    {
        return LaneType::None;
    }
}

bool toBool(std::string const &boolString, bool defaultValue)
{
    auto boolStringLower = boolString;
    std::transform(boolStringLower.begin(), boolStringLower.end(),
                   boolStringLower.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    if (boolStringLower == "false")
    {
        return false;
    }
    else if (boolStringLower == "true")
    {
        return true;
    }
    return defaultValue;
}

void LaneParser::ParseLane(const pugi::xml_node &xmlNode,
                           std::vector<opendrive_1_4::LaneInfo> &out_lane)
{
    // record: lane; parent: <left> / <center> / <right> instances: 1+;
    for (pugi::xml_node lane = xmlNode.child("lane"); lane;
         lane = lane.next_sibling("lane"))
    {
        LaneInfo currentLane;

        currentLane.attributes.type =
            toLaneType(lane.attribute("type").value());
        currentLane.attributes.level =
            toBool(lane.attribute("level").value(), false);
        currentLane.attributes.id = std::atoi(lane.attribute("id").value());

        ParseLaneSpeed(lane, currentLane.lane_speed);
        ParseLaneWidth(lane, currentLane.lane_width);
        ParseLaneLink(lane, currentLane.link);
        ParseLaneRoadMark(lane, currentLane.road_marker);
        ParseLaneRoadMarkType(lane, currentLane.road_marker_type);
        ParseLaneRoadMarkLine(lane, currentLane.road_marker_line);
        ParseLaneMaterial(lane, currentLane.lane_materials);
        ParseLaneBorder(lane, currentLane.lane_border);
        ParseLaneVisibility(lane, currentLane.lane_visibility);
        ParseLaneAccess(lane, currentLane.lane_access);
        ParseLaneHeight(lane, currentLane.lane_height);
        ParseLaneRule(lane, currentLane.lane_rule);

        out_lane.emplace_back(std::move(currentLane));
    }
}

void LaneParser::ParseLaneWidth(const pugi::xml_node &xmlNode,
                                opendrive_1_4::LaneWidthSet &out_lane_width)
{
    // record: lane width; instances: 1+ (if no <border> entry is present);
    for (pugi::xml_node laneWidth = xmlNode.child("width"); laneWidth;
         laneWidth = laneWidth.next_sibling("width"))
    {
        LaneWidth laneWidthInfo;

        laneWidthInfo.start_offset =
            std::stod(laneWidth.attribute("sOffset").value());

        laneWidthInfo.a = std::stod(laneWidth.attribute("a").value());
        laneWidthInfo.b = std::stod(laneWidth.attribute("b").value());

        laneWidthInfo.c = std::stod(laneWidth.attribute("c").value());
        laneWidthInfo.d = std::stod(laneWidth.attribute("d").value());

        out_lane_width.insert(std::move(laneWidthInfo));
    }
}

void LaneParser::ParseLaneLink(
    const pugi::xml_node &xmlNode,
    std::unique_ptr<opendrive_1_4::LaneLink> &out_lane_link)
{
    const pugi::xml_node linkNode = xmlNode.child("link");
    // Only when lanes end at a junction or have no physical link,
    // this record should be omitted
    if (!linkNode)
    {
        return;
    }

    const pugi::xml_node predecessorNode = linkNode.child("predecessor");
    const pugi::xml_node successorNode = linkNode.child("successor");

    out_lane_link = (predecessorNode || successorNode)
                        ? std::make_unique<opendrive_1_4::LaneLink>()
                        : nullptr;
    if (out_lane_link == nullptr)
    {
        return;
    }

    out_lane_link->predecessor_id =
        predecessorNode ? std::atoi(predecessorNode.attribute("id").value())
                        : 0;
    out_lane_link->successor_id =
        successorNode ? std::atoi(successorNode.attribute("id").value()) : 0;
}

void LaneParser::ParseLaneOffset(const pugi::xml_node &xmlNode,
                                 opendrive_1_4::LaneOffsetSet &out_lane_offset)
{
    LaneOffset lanesOffset;

    lanesOffset.start_offset = std::stod(xmlNode.attribute("s").value());
    lanesOffset.a = std::stod(xmlNode.attribute("a").value());
    lanesOffset.b = std::stod(xmlNode.attribute("b").value());
    lanesOffset.c = std::stod(xmlNode.attribute("c").value());
    lanesOffset.d = std::stod(xmlNode.attribute("d").value());

    out_lane_offset.insert(lanesOffset);
}

void LaneParser::ParseLaneRoadMark(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::LaneRoadMark> &out_lane_mark)
{
    // record: road mark; instances: 0+;
    for (pugi::xml_node road_mark = xmlNode.child("roadMark"); road_mark;
         road_mark = road_mark.next_sibling("roadMark"))
    {
        LaneRoadMark roadMarker;

        if (road_mark.attribute("sOffset") != nullptr)
        {
            roadMarker.start_offset =
                std::stod(road_mark.attribute("sOffset").value());
        }

        if (road_mark.attribute("width") != nullptr)
        {
            roadMarker.width = std::stod(road_mark.attribute("width").value());
        }

        if (road_mark.attribute("type") != nullptr)
        {
            roadMarker.type = road_mark.attribute("type").value();
        }

        if (road_mark.attribute("weight") != nullptr)
        {
            roadMarker.weigth = road_mark.attribute("weight").value();
        }

        if (road_mark.attribute("material") != nullptr)
        {
            roadMarker.material = road_mark.attribute("material").value();
        }

        if (road_mark.attribute("color") != nullptr)
        {
            roadMarker.color = road_mark.attribute("color").value();
        }

        if (road_mark.attribute("laneChange") != nullptr)
        {
            roadMarker.lane_change = road_mark.attribute("laneChange").value();
        }

        out_lane_mark.emplace_back(std::move(roadMarker));
    }
}

void LaneParser::ParseLaneSpeed(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::LaneSpeed> &out_lane_speed)
{
    // record: lane speed; instances: 0+;
    for (pugi::xml_node laneSpeed = xmlNode.child("speed"); laneSpeed;
         laneSpeed = laneSpeed.next_sibling("speed"))
    {
        LaneSpeed lane_speed = {0.0, 0.0, ""};

        lane_speed.start_offset =
            std::stod(laneSpeed.attribute("sOffset").value());
        lane_speed.max_speed = std::stod(laneSpeed.attribute("max").value());
        lane_speed.unit = laneSpeed.attribute("unit").value();

        out_lane_speed.emplace_back(std::move(lane_speed));
    }
}

void LaneParser::Parse(const pugi::xml_node &xmlNode, Lanes &out_lanes)
{
    LaneParser laneParser;

    // record: laneOffset; instances: 0+;
    for (pugi::xml_node laneSection = xmlNode.child("laneOffset"); laneSection;
         laneSection = laneSection.next_sibling("laneOffset"))
    {
        laneParser.ParseLaneOffset(laneSection, out_lanes.lane_offset);
    }

    // record: laneSection; instances: 1+;
    for (pugi::xml_node laneSection = xmlNode.child("laneSection"); laneSection;
         laneSection = laneSection.next_sibling("laneSection"))
    {
        LaneSection laneSec;
        laneSec.start_position = std::stod(laneSection.attribute("s").value());
        // until we know more, we set end to start.
        // set function: calculateLaneSectionBounds
        laneSec.end_position = laneSec.start_position;

        laneSec.is_single_side =
            toBool(laneSection.attribute("singleSide").value(), false);

        // record: left/center/right lanes within a lane section; instances: 1;
        if (laneSection.child("left").empty() &&
            laneSection.child("center").empty() &&
            laneSection.child("right").empty())
        {
            ODP_ASSERT(
                false,
                "At least one entry(left, center or right) must be present!");
        }
        pugi::xml_node lane = laneSection.child("left");
        laneParser.ParseLane(lane, laneSec.left);

        lane = laneSection.child("center");
        laneParser.ParseLane(lane, laneSec.center);

        lane = laneSection.child("right");
        laneParser.ParseLane(lane, laneSec.right);

        out_lanes.lane_sections.emplace_back(std::move(laneSec));
    }
}

void LaneParser::ParseLaneRoadMarkLine(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::LaneRoadMarkLine> &out_lane_mark_line)
{
    LaneRoadMarkLine roadMarkerLine;
    // record: road mark line; instances: 1+;
    for (pugi::xml_node road_mark_line =
             xmlNode.child("roadMark").child("type").child("line");
         road_mark_line;
         road_mark_line = road_mark_line.next_sibling("roadMark"))
    {
        if (road_mark_line.attribute("length"))
        {
            roadMarkerLine.length =
                std::stod(road_mark_line.attribute("length").value());
        }

        if (road_mark_line.attribute("space"))
        {
            roadMarkerLine.space =
                std::stod(road_mark_line.attribute("space").value());
        }

        if (road_mark_line.attribute("tOffset"))
        {
            roadMarkerLine.t = std::stod(road_mark_line.attribute("t").value());
        }

        if (road_mark_line.attribute("sOffset"))
        {
            roadMarkerLine.start_offset =
                std::stod(road_mark_line.attribute("sOffset").value());
        }

        if (road_mark_line.attribute("rule"))
        {
            roadMarkerLine.rule = road_mark_line.attribute("rule").value();
        }

        if (road_mark_line.attribute("width"))
        {
            roadMarkerLine.width =
                std::stod(road_mark_line.attribute("width").value());
        }
        out_lane_mark_line.emplace_back(std::move(roadMarkerLine));
    }
}

void LaneParser::ParseLaneRoadMarkType(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::LaneRoadMarkType> &out_lane_mark_type)
{
    LaneRoadMarkType roadMarkerType;
    pugi::xml_node road_mark_type = xmlNode.child("roadMark").child("type");

    // record: road mark type; instances: 0..1;
    if (road_mark_type)
    {
        roadMarkerType.name = road_mark_type.attribute("name").value();
        if (road_mark_type.attribute("width"))
        {
            roadMarkerType.width =
                std::stod(road_mark_type.attribute("width").value());
        }
    }
    out_lane_mark_type.emplace_back(std::move(roadMarkerType));
}

void LaneParser::ParseLaneMaterial(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::LaneMaterial> &out_lane_material)
{
    for (pugi::xml_node laneMaterial : xmlNode.child("material"))
    {
        LaneMaterial laneMaterialInfo;

        laneMaterialInfo.start_offset =
            std::stod(laneMaterial.attribute("sOffset").value());
        laneMaterialInfo.surface = laneMaterial.attribute("surface").value();
        laneMaterialInfo.friction =
            std::stod(laneMaterial.attribute("friction").value());
        laneMaterialInfo.roughness =
            std::stod(laneMaterial.attribute("roughness").value());

        out_lane_material.emplace_back(laneMaterialInfo);
    }
}

void LaneParser::ParseLaneBorder(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::LaneBorder> &out_lane_border)
{
    // record: lane border; instances: 1+ (if no <width> entry is present)
    for (pugi::xml_node laneBorder : xmlNode.child("border"))
    {
        LaneBorder laneBorderInfo;

        laneBorderInfo.start_offset =
            std::stod(laneBorder.attribute("sOffset").value());
        laneBorderInfo.a = std::stod(laneBorder.attribute("a").value());
        laneBorderInfo.b = std::stod(laneBorder.attribute("b").value());
        laneBorderInfo.c = std::stod(laneBorder.attribute("c").value());
        laneBorderInfo.d = std::stod(laneBorder.attribute("d").value());

        out_lane_border.emplace_back(std::move(laneBorderInfo));
    }
}

void LaneParser::ParseLaneVisibility(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::LaneVisibility> &out_lane_visibility)
{
    for (pugi::xml_node laneVisibility : xmlNode.child("visibility"))
    {
        LaneVisibility laneVisibilityInfo;

        laneVisibilityInfo.start_offset =
            std::stod(laneVisibility.attribute("sOffset").value());
        laneVisibilityInfo.forward =
            std::stod(laneVisibility.attribute("forward").value());
        laneVisibilityInfo.back =
            std::stod(laneVisibility.attribute("back").value());
        laneVisibilityInfo.left =
            std::stod(laneVisibility.attribute("left").value());
        laneVisibilityInfo.right =
            std::stod(laneVisibility.attribute("right").value());

        out_lane_visibility.emplace_back(laneVisibilityInfo);
    }
}

void LaneParser::ParseLaneAccess(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::LaneAccess> &out_lane_access)
{
    for (pugi::xml_node laneAccess : xmlNode.child("access"))
    {
        LaneAccess laneAccessInfo;

        laneAccessInfo.start_offset =
            std::stod(laneAccess.attribute("sOffset").value());
        laneAccessInfo.restriction =
            laneAccess.attribute("restriction").value();

        out_lane_access.emplace_back(laneAccessInfo);
    }
}

void LaneParser::ParseLaneHeight(const pugi::xml_node &xmlNode,
                                 opendrive_1_4::LaneHeightSet &out_lane_height)
{
    for (pugi::xml_node laneHeight : xmlNode.child("height"))
    {
        LaneHeight laneHeightInfo;

        laneHeightInfo.start_offset =
            std::stod(laneHeight.attribute("sOffset").value());
        laneHeightInfo.inner = std::stod(laneHeight.attribute("inner").value());
        laneHeightInfo.outer = std::stod(laneHeight.attribute("outer").value());

        out_lane_height.insert(laneHeightInfo);
    }
}

void LaneParser::ParseLaneRule(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::LaneRule> &out_lane_rule)
{
    for (pugi::xml_node laneRule : xmlNode.child("rule"))
    {
        LaneRule laneRuleInfo;

        laneRuleInfo.start_offset =
            std::stod(laneRule.attribute("sOffset").value());
        laneRuleInfo.value = laneRule.attribute("value").value();

        out_lane_rule.emplace_back(laneRuleInfo);
    }
}

} // namespace parser
} // namespace opendrive_1_4
