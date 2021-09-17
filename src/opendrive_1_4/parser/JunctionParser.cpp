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

#include "uos_odrparser/opendrive_1_4/parser/JunctionParser.h"

void opendrive_1_4::parser::JunctionParser::Parse(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::Junction> &out_junction)
{
    opendrive_1_4::parser::JunctionParser parser;
    opendrive_1_4::Junction junction;

    junction.attributes.id = std::atoi(xmlNode.attribute("id").value());
    junction.attributes.name = xmlNode.attribute("name").value();

    parser.ParseConnection(xmlNode, junction.connections, junction.controllers);
    out_junction.emplace_back(junction);
}

void opendrive_1_4::parser::JunctionParser::ParseConnection(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::JunctionConnection> &out_connections,
    std::vector<opendrive_1_4::JunctionController> &out_controllers)
{
    for (pugi::xml_node junctionConnection = xmlNode.child("connection");
         junctionConnection;
         junctionConnection = junctionConnection.next_sibling("connection"))
    {
        opendrive_1_4::JunctionConnection jConnection;

        jConnection.attributes.id =
            std::atoi(junctionConnection.attribute("id").value());
        jConnection.attributes.contact_point =
            junctionConnection.attribute("contactPoint").value();

        jConnection.attributes.incoming_road =
            std::atoi(junctionConnection.attribute("incomingRoad").value());
        jConnection.attributes.connecting_road =
            std::atoi(junctionConnection.attribute("connectingRoad").value());

        ParseLaneLink(junctionConnection, jConnection.links);
        out_connections.emplace_back(jConnection);
    }

    for (pugi::xml_node junctionController : xmlNode.child("controller"))
    {
        opendrive_1_4::JunctionController jController;
        jController.attributes.id =
            std::atoi(junctionController.attribute("id").value());
        out_controllers.emplace_back(jController);
    }
}

void opendrive_1_4::parser::JunctionParser::ParseLaneLink(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::JunctionLaneLink> &out_lane_link)
{
    for (pugi::xml_node junctionLaneLink = xmlNode.child("laneLink");
         junctionLaneLink;
         junctionLaneLink = junctionLaneLink.next_sibling("laneLink"))
    {
        opendrive_1_4::JunctionLaneLink jLaneLink;

        jLaneLink.from = std::atoi(junctionLaneLink.attribute("from").value());
        jLaneLink.to = std::atoi(junctionLaneLink.attribute("to").value());

        out_lane_link.emplace_back(jLaneLink);
    }
}
