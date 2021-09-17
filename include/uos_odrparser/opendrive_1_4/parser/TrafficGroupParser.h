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

class TrafficGroupParser
{
private:
    void ParseTrafficLight(
        const pugi::xml_node &xmlNode,
        std::vector<opendrive_1_4::TrafficLight> &out_trafficLights);

    void ParseBoxAreas(const pugi::xml_node &xmlNode,
                       std::vector<opendrive_1_4::BoxComponent> &out_boxareas);

public:
    static void
    Parse(const pugi::xml_node &xmlNode,
          std::vector<opendrive_1_4::TrafficLightGroup> &out_trafficlightgroup);
};

} // namespace parser
} // namespace opendrive_1_4
