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

#include "uos_odrparser/opendrive_1_4/types.hpp"

#include <pugixml.hpp>

namespace opendrive_1_4
{
namespace parser
{

class ObjectParser
{
public:
    static void Parse(const pugi::xml_node &xmlNode,
                      std::vector<opendrive_1_4::RoadObjects> &out_objects);
};

} // namespace parser
} // namespace opendrive_1_4
