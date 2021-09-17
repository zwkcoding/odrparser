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

class GeometryParser
{
private:
    void ParseArc(const pugi::xml_node &xmlNode,
                  opendrive_1_4::GeometryAttributesArc *out_geometry_arc);

    void ParseLine(const pugi::xml_node &xmlNode,
                   opendrive_1_4::GeometryAttributesLine *out_geometry_line);

    void
    ParseSpiral(const pugi::xml_node &xmlNode,
                opendrive_1_4::GeometryAttributesSpiral *out_geometry_spiral);

    void ParsePoly3(const pugi::xml_node &xmlNode,
                    opendrive_1_4::GeometryAttributesPoly3 *out_geometry_poly3);

    void ParseParamPoly3(
        const pugi::xml_node &xmlNode,
        opendrive_1_4::GeometryAttributesParamPoly3 *out_geometry_param_poly3);

public:
    static void
    Parse(const pugi::xml_node &xmlNode,
          std::vector<std::unique_ptr<opendrive_1_4::GeometryAttributes>>
              &out_geometry_attributes);
};
} // namespace parser
} // namespace opendrive_1_4
