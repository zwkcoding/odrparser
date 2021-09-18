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

#include <string>
#include <uos_odrparser/opendrive_1_4/types.hpp>

namespace opendrive_1_4
{
bool Load(std::string const &file, OpenDriveData &open_drive_data);
bool Parse(std::string const &content, OpenDriveData &open_drive_data);
/**
 * @brief Generates the full lane map
 *
 * @param[in] overlapMargin margin the lanes are narrowed when calculating
 * overlaps.
 *
 */
bool GenerateLaneMap(OpenDriveData &open_drive_data,
                     double const overlapMargin);

const RoadInformation &getRoad(const std::string &id,
                               const OpenDriveData &open_drive_data);

const geom::GeoLocation &getGeoReference(const OpenDriveData &open_drive_data);

const std::string &getRoadName(const RoadInformation &road);

int getRoadId(const RoadInformation &road);

int getRoadJunctionId(const RoadInformation &road);

double getRoadLength(const RoadInformation &road);

const std::vector<RoadTypeInfo> &getRoadType(const RoadInformation &road);

const std::vector<RoadSpeed> &getRoadSpeed(const RoadInformation &road);

} // namespace opendrive_1_4
