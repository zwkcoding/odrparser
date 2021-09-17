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

#include "uos_odrparser/opendrive_1_4/parser/RoadLinkParser.h"

#include <cstdlib>

opendrive_1_4::ElementType toElementType(std::string const elementType)
{
    if (elementType == "junction")
    {
        return opendrive_1_4::ElementType::Junction;
    }
    else if (elementType == "road")
    {
        return opendrive_1_4::ElementType::Road;
    }

    return opendrive_1_4::ElementType::Invalid;
}

opendrive_1_4::ContactPoint toContactPoint(std::string const contactPoint)
{
    if (contactPoint == "start")
    {
        return opendrive_1_4::ContactPoint::Start;
    }
    else if (contactPoint == "end")
    {
        return opendrive_1_4::ContactPoint::End;
    }

    return opendrive_1_4::ContactPoint::Invalid;
}

void opendrive_1_4::parser::RoadLinkParser::ParseLink(
    const pugi::xml_node &xmlNode,
    opendrive_1_4::RoadLinkInformation *out_link_information)
{
    out_link_information->id =
        std::stoi(xmlNode.attribute("elementId").value());
    out_link_information->element_type =
        toElementType(xmlNode.attribute("elementType").value());
    out_link_information->contact_point =
        toContactPoint(xmlNode.attribute("contactPoint").value());
}

void opendrive_1_4::parser::RoadLinkParser::Parse(
    const pugi::xml_node &xmlNode, opendrive_1_4::RoadLink &out_road_link)
{
    RoadLinkParser parser;

    const pugi::xml_node predecessorNode = xmlNode.child("predecessor");
    const pugi::xml_node successorNode = xmlNode.child("successor");

    if (predecessorNode)
    {
        out_road_link.predecessor =
            std::make_unique<opendrive_1_4::RoadLinkInformation>();
        parser.ParseLink(predecessorNode, out_road_link.predecessor.get());
    }

    if (successorNode)
    {
        out_road_link.successor =
            std::make_unique<opendrive_1_4::RoadLinkInformation>();
        parser.ParseLink(successorNode, out_road_link.successor.get());
    }
}
