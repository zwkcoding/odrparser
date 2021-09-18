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

#include "uos_odrparser/opendrive_1_4/parser/GeometryParser.h"

void opendrive_1_4::parser::GeometryParser::ParseArc(
    const pugi::xml_node &xmlNode,
    opendrive_1_4::GeometryAttributesArc *out_geometry_arc)
{
    out_geometry_arc->type = opendrive_1_4::GeometryType::ARC;
    out_geometry_arc->curvature = xmlNode.attribute("curvature").as_double();
}

void opendrive_1_4::parser::GeometryParser::ParseLine(
    const pugi::xml_node &xmlNode,
    opendrive_1_4::GeometryAttributesLine *out_geometry_line)
{
    ODP_UNUSED(xmlNode);
    out_geometry_line->type = opendrive_1_4::GeometryType::LINE;
}

void opendrive_1_4::parser::GeometryParser::ParseSpiral(
    const pugi::xml_node &xmlNode,
    opendrive_1_4::GeometryAttributesSpiral *out_geometry_spiral)
{
    out_geometry_spiral->type = opendrive_1_4::GeometryType::SPIRAL;
    out_geometry_spiral->curve_end = xmlNode.attribute("curvEnd").as_double();
    out_geometry_spiral->curve_start =
        xmlNode.attribute("curvStart").as_double();
}

void opendrive_1_4::parser::GeometryParser::ParsePoly3(
    const pugi::xml_node &xmlNode,
    opendrive_1_4::GeometryAttributesPoly3 *out_geometry_poly3)
{
    out_geometry_poly3->type = opendrive_1_4::GeometryType::POLY3;
    out_geometry_poly3->a = std::stod(xmlNode.attribute("a").value());
    out_geometry_poly3->b = std::stod(xmlNode.attribute("b").value());
    out_geometry_poly3->c = std::stod(xmlNode.attribute("c").value());
    out_geometry_poly3->d = std::stod(xmlNode.attribute("d").value());
}

void opendrive_1_4::parser::GeometryParser::ParseParamPoly3(
    const pugi::xml_node &xmlNode,
    opendrive_1_4::GeometryAttributesParamPoly3 *out_geometry_param_poly3)
{
    out_geometry_param_poly3->type = opendrive_1_4::GeometryType::PARAMPOLY3;
    out_geometry_param_poly3->aU = std::stod(xmlNode.attribute("aU").value());
    out_geometry_param_poly3->bU = std::stod(xmlNode.attribute("bU").value());
    out_geometry_param_poly3->cU = std::stod(xmlNode.attribute("cU").value());
    out_geometry_param_poly3->dU = std::stod(xmlNode.attribute("dU").value());
    out_geometry_param_poly3->aV = std::stod(xmlNode.attribute("aV").value());
    out_geometry_param_poly3->bV = std::stod(xmlNode.attribute("bV").value());
    out_geometry_param_poly3->cV = std::stod(xmlNode.attribute("cV").value());
    out_geometry_param_poly3->dV = std::stod(xmlNode.attribute("dV").value());
    out_geometry_param_poly3->p_range = xmlNode.attribute("pRange").value();
}

void opendrive_1_4::parser::GeometryParser::Parse(
    const pugi::xml_node &xmlNode,
    std::vector<std::unique_ptr<opendrive_1_4::GeometryAttributes>>
        &out_geometry_attributes)
{
    opendrive_1_4::parser::GeometryParser gometry_parser;

    // record: geometry; instances: 1+;
    for (pugi::xml_node roadGeometry = xmlNode.child("geometry"); roadGeometry;
         roadGeometry = roadGeometry.next_sibling("geometry"))
    {
        std::unique_ptr<opendrive_1_4::GeometryAttributes> geometry_attributes;
        std::string firstChildName(roadGeometry.first_child().name());

        // record: geometric element; instances: 1;
        if (firstChildName == "arc")
        {
            geometry_attributes =
                std::make_unique<opendrive_1_4::GeometryAttributesArc>();
            gometry_parser.ParseArc(
                roadGeometry.child("arc"),
                static_cast<opendrive_1_4::GeometryAttributesArc *>(
                    geometry_attributes.get()));
        }
        else if (firstChildName == "line")
        {
            geometry_attributes =
                std::make_unique<opendrive_1_4::GeometryAttributesLine>();
            gometry_parser.ParseLine(
                roadGeometry.child("line"),
                static_cast<opendrive_1_4::GeometryAttributesLine *>(
                    geometry_attributes.get()));
        }
        else if (firstChildName == "spiral")
        {
            geometry_attributes =
                std::make_unique<opendrive_1_4::GeometryAttributesSpiral>();
            gometry_parser.ParseSpiral(
                roadGeometry.child("spiral"),
                static_cast<opendrive_1_4::GeometryAttributesSpiral *>(
                    geometry_attributes.get()));
        }
        else if (firstChildName == "poly3")
        {
            geometry_attributes =
                std::make_unique<opendrive_1_4::GeometryAttributesPoly3>();
            gometry_parser.ParsePoly3(
                roadGeometry.child("poly3"),
                static_cast<opendrive_1_4::GeometryAttributesPoly3 *>(
                    geometry_attributes.get()));
        }
        else if (firstChildName == "paramPoly3")
        {
            geometry_attributes =
                std::make_unique<opendrive_1_4::GeometryAttributesParamPoly3>();
            gometry_parser.ParseParamPoly3(
                roadGeometry.child("paramPoly3"),
                static_cast<opendrive_1_4::GeometryAttributesParamPoly3 *>(
                    geometry_attributes.get()));
        }
        else
        {
            ODP_ASSERT(false, "Geometry type unknown");
        }

        geometry_attributes->start_position =
            std::stod(roadGeometry.attribute("s").value());

        geometry_attributes->start_position_x =
            std::stod(roadGeometry.attribute("x").value());
        geometry_attributes->start_position_y =
            std::stod(roadGeometry.attribute("y").value());

        geometry_attributes->heading =
            std::stod(roadGeometry.attribute("hdg").value());
        geometry_attributes->length =
            std::stod(roadGeometry.attribute("length").value());

        out_geometry_attributes.emplace_back(std::move(geometry_attributes));
    }
}
