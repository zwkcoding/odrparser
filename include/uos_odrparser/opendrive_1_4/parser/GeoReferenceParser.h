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
#include <string>

namespace opendrive_1_4
{
namespace parser
{

class GeoReferenceParser
{
public:
    static ::opendrive_1_4::geom::GeoLocation
    Parse(const std::string &geo_reference_string);
};

} // namespace parser
} // namespace opendrive_1_4
