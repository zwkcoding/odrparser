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

#include "uos_odrparser/opendrive_1_4/parser/ProfilesParser.h"

namespace opendrive_1_4
{
namespace parser
{

void ProfilesParser::ParseElevation(
    const pugi::xml_node &xmlNode,
    opendrive_1_4::ElevationProfileSet &out_elevation_profile)
{
    // record: elevation; instances: 1+
    for (pugi::xml_node laneSection = xmlNode.child("elevation"); laneSection;
         laneSection = laneSection.next_sibling("elevation"))
    {
        opendrive_1_4::ElevationProfile elevationProfile;

        elevationProfile.start_offset =
            std::stod(laneSection.attribute("s").value());
        elevationProfile.a = std::stod(laneSection.attribute("a").value());
        elevationProfile.b = std::stod(laneSection.attribute("b").value());
        elevationProfile.c = std::stod(laneSection.attribute("c").value());
        elevationProfile.d = std::stod(laneSection.attribute("d").value());
        out_elevation_profile.insert(elevationProfile);
    }
}

void ProfilesParser::ParseShape(
    const pugi::xml_node &xmlNode,
    opendrive_1_4::LateralProfileShapeSetMap &out_lateral_profile_shape)
{
    // record: lateral shape; instance: 0+
    for (pugi::xml_node laneSection = xmlNode.child("shape"); laneSection;
         laneSection = laneSection.next_sibling("shape"))
    {
        opendrive_1_4::LateralProfileShape lateralProfileShape;

        lateralProfileShape.s_position =
            std::stod(laneSection.attribute("s").value());
        // the lateral profiles polynomial is in t-axis direction
        lateralProfileShape.start_offset =
            std::stod(laneSection.attribute("t").value());
        lateralProfileShape.a = std::stod(laneSection.attribute("a").value());
        lateralProfileShape.b = std::stod(laneSection.attribute("b").value());
        lateralProfileShape.c = std::stod(laneSection.attribute("c").value());
        lateralProfileShape.d = std::stod(laneSection.attribute("d").value());

        // the LateralPolygonShapeSet at the same s_position are collected in a
        // map
        auto const insertResult = out_lateral_profile_shape.insert(
            {lateralProfileShape.s_position,
             opendrive_1_4::LateralProfileShapeSet()});
        insertResult.first->second.insert(lateralProfileShape);
    }
}

void ProfilesParser::ParseSuperelevation(
    const pugi::xml_node &xmlNode,
    opendrive_1_4::LateralProfileSuperelevationSet
        &out_lateral_profile_superelevation)
{
    // record: lateral superelevation; instances: 0+
    for (pugi::xml_node laneSection = xmlNode.child("superelevation");
         laneSection; laneSection = laneSection.next_sibling("superelevation"))
    {
        opendrive_1_4::LateralProfileSuperelevation
            lateralProfileSuperelevation;

        lateralProfileSuperelevation.start_offset =
            std::stod(laneSection.attribute("s").value());
        lateralProfileSuperelevation.a =
            std::stod(laneSection.attribute("a").value());
        lateralProfileSuperelevation.b =
            std::stod(laneSection.attribute("b").value());
        lateralProfileSuperelevation.c =
            std::stod(laneSection.attribute("c").value());
        lateralProfileSuperelevation.d =
            std::stod(laneSection.attribute("d").value());
        out_lateral_profile_superelevation.insert(
            std::move(lateralProfileSuperelevation));
    }
}

void ProfilesParser::Parse(const pugi::xml_node &xmlNode,
                           opendrive_1_4::RoadProfiles &out_road_profiles)
{
    ProfilesParser profilesParser;

    const pugi::xml_node elevationNode = xmlNode.child("elevationProfile");
    const pugi::xml_node lateralProfileNode = xmlNode.child("lateralProfile");

    if (elevationNode)
    {
        profilesParser.ParseElevation(elevationNode,
                                      out_road_profiles.elevation_profile);
    }

    if (lateralProfileNode)
    {
        profilesParser.ParseShape(lateralProfileNode,
                                  out_road_profiles.lateral_profile_shape);
        profilesParser.ParseSuperelevation(
            lateralProfileNode,
            out_road_profiles.lateral_profile_superelevation);
    }
}

} // namespace parser
} // namespace opendrive_1_4
