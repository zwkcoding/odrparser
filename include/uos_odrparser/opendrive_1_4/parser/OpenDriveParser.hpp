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

enum class XmlInputType : int
{
    FILE,
    CONTENT
};

struct OpenDriveParser
{
    static bool Parse(const char *xml,
                      opendrive_1_4::OpenDriveData &out_open_drive_data,
                      XmlInputType inputType, std::string *out_error = nullptr);
    static bool Load(const char *xml,
                     opendrive_1_4::OpenDriveData &out_open_drive_data,
                     XmlInputType inputType, std::string *out_error = nullptr);
};
} // namespace parser
} // namespace opendrive_1_4
