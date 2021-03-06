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

#include "uos_odrparser/opendrive_1_4/parser/ControllerParser.h"

void opendrive_1_4::parser::ControllerParser::Parse(
    const pugi::xml_node &xmlNode,
    std::vector<opendrive_1_4::Controller> &out_controllers,
    std::vector<opendrive_1_4::ControllerSignal> &out_controller_signals)
{
    for (pugi::xml_node controller_node = xmlNode.child("controller");
         controller_node;
         controller_node = controller_node.next_sibling("controller"))
    {
        opendrive_1_4::Controller gController;
        gController.id = std::stoi(controller_node.attribute("id").value());
        gController.name = controller_node.attribute("name").value();
        gController.sequence = controller_node.attribute("sequence").as_int();

        out_controllers.emplace_back(gController);

        for (pugi::xml_node control_node : controller_node.children("control"))
        {
            opendrive_1_4::ControllerSignal gControllerSignal;
            gControllerSignal.id = control_node.attribute("signalId").as_int();
            gControllerSignal.type = control_node.attribute("type").value();
            out_controller_signals.emplace_back(gControllerSignal);
        }
    }
}
