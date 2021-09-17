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

#include "uos_odrparser/opendrive_1_4/parser/GeoReferenceParser.h"
#include "uos_odrparser/opendrive_1_4/types.hpp"

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <limits>
#include <vector>

#include <proj.h> // version later v7.0

namespace opendrive_1_4
{
namespace parser
{

::opendrive_1_4::geom::GeoLocation
GeoReferenceParser::Parse(const std::string &geo_reference_string)
{
    bool is_valid_proj;
    PJ_CONTEXT *C;
    PJ *P;
    PJ *P_for_GIS;
    PJ_COORD refPoint;
    ::opendrive_1_4::geom::GeoLocation result{
        std::numeric_limits<double>::quiet_NaN(),
        std::numeric_limits<double>::quiet_NaN(), 0.0, ""};

    result.projection = geo_reference_string;

    // https://proj.org/development/quickstart.html
    /* or you may set C=PJ_DEFAULT_CTX if you are sure you will     */
    /* use PJ objects from only one thread                          */
    C = proj_context_create();
    P = proj_create_crs_to_crs(C, "EPSG:4326", geo_reference_string.c_str(),
                               NULL);
    is_valid_proj = true;
    if (0 == P)
        is_valid_proj = false;
    else
    {
        /* This will ensure that the order of coordinates for the input CRS */
        /* will be longitude, latitude, whereas EPSG:4326 mandates latitude, */
        /* longitude */
        P_for_GIS = proj_normalize_for_visualization(C, P);
        if (0 == P_for_GIS)
        {
            is_valid_proj = false;
            fprintf(stderr, "Oops\n");
        }
        else
        {
            proj_destroy(P);
            P = P_for_GIS;
        }
    }
    if (!is_valid_proj)
    {
        std::vector<std::string> geo_ref_splitted;
        boost::split(geo_ref_splitted, geo_reference_string,
                     boost::is_any_of(" "));
        for (auto const &element : geo_ref_splitted)
        {
            std::vector<std::string> geo_ref_key_value;
            boost::split(geo_ref_key_value, element, boost::is_any_of("="));
            if (geo_ref_key_value.size() != 2u)
            {
                continue;
            }

            if (geo_ref_key_value[0] == "+lat_0")
            {
                result.latitude = std::stod(geo_ref_key_value[1]);
            }
            else if (geo_ref_key_value[0] == "+lon_0")
            {
                result.longitude = std::stod(geo_ref_key_value[1]);
            }
        }
    }
    else
    {
        refPoint.enu.e = 0;
        refPoint.enu.n = 0;
        refPoint.enu.u = 0;
        const auto geoPoint = proj_trans(P, PJ_INV, refPoint);
        result.longitude = geoPoint.lp.lam;
        result.latitude = geoPoint.lp.phi;

        /* Clean up */
        proj_destroy(P);
        /* may be omitted in the single threaded case */
        proj_context_destroy(C);
    }

    return result;
}

} // namespace parser
} // namespace opendrive_1_4
