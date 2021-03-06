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

#pragma once

#include "uos_odrparser/opendrive_1_4/types.hpp"

#include <pugixml.hpp>

namespace opendrive_1_4
{
namespace parser
{

class JunctionParser
{
private:
    void ParseConnection(
        const pugi::xml_node &xmlNode,
        std::vector<opendrive_1_4::JunctionConnection> &out_connections,
        std::vector<opendrive_1_4::JunctionController> &out_controllers);

    void
    ParseLaneLink(const pugi::xml_node &xmlNode,
                  std::vector<opendrive_1_4::JunctionLaneLink> &out_lane_link);

public:
    static void Parse(const pugi::xml_node &xmlNode,
                      std::vector<opendrive_1_4::Junction> &out_junction);
};

} // namespace parser
} // namespace opendrive_1_4
