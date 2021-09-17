//
// Created by brucknem on 22.02.21.
//

#include "OpenDriveTests.hpp"
#include <OpenDRIVE/HDMapFactory.hpp>

namespace opendrive_1_4
{
namespace tests
{

std::vector<opendrive_1_4::Geometry> createMockPlanView(int sections,
                                                        double sectionLength)
{
    std::vector<opendrive_1_4::Geometry> result;

    result.reserve(sections);
    for (int i = 0; i < sections; i++)
    {
        result.emplace_back(opendrive_1_4::Geometry{i * sectionLength,
                                                    {sectionLength * i, 0},
                                                    0,
                                                    (double)sectionLength,
                                                    {0, 1, 0, 0},
                                                    {0, 0, 0, 0}});
    }
    return result;
}

std::vector<opendrive_1_4::Elevation>
createMockElevationProfile(int sections, double sectionLength)
{
    std::vector<opendrive_1_4::Elevation> result;

    result.reserve(sections);
    for (int i = 0; i < sections; i++)
    {
        result.emplace_back(opendrive_1_4::Elevation{
            i * sectionLength, {i * sectionLength, 1, 0, 0}});
    }
    return result;
}

std::vector<opendrive_1_4::SuperElevation>
createMockLateralProfileSuperElevation(int sections, double sectionLength)
{
    std::vector<opendrive_1_4::SuperElevation> result;

    double quarterPi = M_PI / 4.;
    result.reserve(sections);
    for (int i = 0; i < sections; i++)
    {
        result.emplace_back(opendrive_1_4::SuperElevation{
            i * sectionLength, {quarterPi, 0, 0, 0}});
    }
    return result;
}

std::vector<opendrive_1_4::Shape>
createMockLateralProfileShape(int sections, double sectionLength)
{
    std::vector<opendrive_1_4::Shape> result;

    for (int i = 0; i < sections; i++)
    {
        result.emplace_back(
            opendrive_1_4::Shape{i * sectionLength, 0, {0, 0, 0, 0}});
        result.emplace_back(
            opendrive_1_4::Shape{i * sectionLength, 10, {0, -1, 0, 0}});
    }
    return result;
}

std::vector<opendrive_1_4::Object> createMockObjects()
{
    std::string pole = "pole";
    std::string permanentDelineator = "permanentDelineator";
    std::string none = "none";
    return {
        opendrive_1_4::Object(0, 0, pole, permanentDelineator, "0", 1, 0, 0,
                              none, 0, 0, 0, 0, 0.1, 1),
        opendrive_1_4::Object(0, 10, "else", "else", "1", 1, 0, 0, none, 0, 0,
                              0, 0, 0.1, 1),
        opendrive_1_4::Object(20, 15, pole, permanentDelineator, "2", 1, 0, 0,
                              none, 0, 0, 0, 0, 0.1, 1),
    };
}

std::vector<opendrive_1_4::CubicPolynomWrapper>
createMockLaneOffsets(int sections, double sectionLength)
{
    std::vector<opendrive_1_4::CubicPolynomWrapper> result;
    result.reserve(sections);
    for (int i = 0; i < sections; i++)
    {
        result.emplace_back(
            opendrive_1_4::CubicPolynomWrapper(i * sectionLength, 1, 0, 0, 0));
    }
    return result;
}

std::vector<opendrive_1_4::RoadMark> createMockRoadMarks()
{
    double width = 0.15;
    return {opendrive_1_4::RoadMark(
        1, "solid", "standard", "standard", "standard", width, "both", 0,
        // TODO add for type
        {},
        {
            opendrive_1_4::RoadMark::Line(2, 1, 0, -1, "standard", width,
                                          "standard"),
            opendrive_1_4::RoadMark::Line(4, 1, 0, 1, "standard", width,
                                          "standard"),
        })};
}

std::vector<opendrive_1_4::LaneSection>
createMockLaneSections(int sections, double sectionLength, int numLanesPerSide)
{
    std::vector<opendrive_1_4::LaneSection> result;
    for (int i = 0; i < sections; i++)
    {
        std::vector<Lane> right;
        std::vector<Lane> left;
        for (int id = 1; id <= numLanesPerSide; id++)
        {
            left.emplace_back(opendrive_1_4::Lane(
                id, "driving", false, {},
                {opendrive_1_4::CubicPolynomWrapper{
                    0, {sectionLength / numLanesPerSide, 0, 0, 0}}},
                {},
                //                                            createMockRoadMarks()
                {}));
            right.emplace_back(opendrive_1_4::Lane(
                -id, "driving", false, {},
                {opendrive_1_4::CubicPolynomWrapper{
                    0, {sectionLength / numLanesPerSide, 0, 0, 0}}},
                {},
                //                                            createMockRoadMarks()
                {}));
        }
        result.emplace_back(opendrive_1_4::LaneSection(
            i * sectionLength, false, left,
            opendrive_1_4::Lane(0, "none", false, {}, {}, {},
                                createMockRoadMarks()),
            right));
    }
    return result;
}

opendrive_1_4::Lanes createMockLanes(int sections, double sectionLength,
                                     int numLanesPerSide)
{
    return opendrive_1_4::Lanes(
        createMockLaneOffsets(sections, sectionLength),
        createMockLaneSections(sections, sectionLength, numLanesPerSide));
}

void OpenDriveTests::SetUp()
{
    auto road = opendrive_1_4::Road(
        "0", "test", sections * sectionLength, "-1", {{0, "motorway"}},
        createMockObjects(), createMockPlanView(sections, sectionLength),
        createMockElevationProfile(sections, sectionLength),
        createMockLateralProfileSuperElevation(sections, sectionLength),
        createMockLateralProfileShape(sections, sectionLength),
        createMockLanes(sections, sectionLength, numLanesPerSide));
    mockTestRoad = std::make_shared<opendrive_1_4::Road>(road);

    mockTestMap = std::make_shared<opendrive_1_4::HDMap>(opendrive_1_4::HDMap{
        "test.xodr",
        {{mockTestRoadId, road}},
        {"+proj=tmerc +lat_0=0 +lon_0=9 +k=0.9996 +x_0=500000 +y_0=0 "
         "+datum=WGS84 +units=m +no_defs",
         "3D Mapping Solutions", 5.350576134016e+06, 5.350576134016e+06,
         5.350576134016e+06, 5.350576134016e+06}});
}
}
}