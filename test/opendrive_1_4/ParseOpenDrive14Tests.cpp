#include "gtest/gtest.h"

#include "uos_odrparser/opendrive_1_4/OpenDrive.hpp"
#include "uos_odrparser/opendrive_1_4/types.hpp"
#include <sstream>

#ifndef TRACKS_DIR
#define TRACKS_DIR "./"
#endif

namespace opendrive_1_4
{
namespace tests
{

/**
    * Base setup for the tests that parse the opendrive 1.4 test HD map.
    */
class ParseOpenDrive14Tests : public ::testing::Test
{
public:
    /**
        * The HD maps.
        */
    std::shared_ptr<OpenDriveData> testMapOpenDrive14;

    /**
        * The id of the test roads.
        */
    const char *testRoadIdOpenDrive14 = "2311000";

    /**
        * The test roads.
        */
    const RoadInformation *testRoadOpenDrive14;

    /**
        * The maximal difference after parsing.
        */
    double maxDifference = 1e-6;

    /**
        * @destrcutor
        */
    ~ParseOpenDrive14Tests() override = default;

    static void odrDataDeleter(OpenDriveData *p) { delete p; }

    /**
        * @setup
        */
    void SetUp() override
    {
        // create file name
        bool ret;
        std::stringstream ss;
        ss << TRACKS_DIR << "test_map_opendrive_1_4.xodr";
        testMapOpenDrive14 = std::shared_ptr<OpenDriveData>(
            new OpenDriveData(), [](OpenDriveData *p) { delete p; });
        printf("xodr file path: %s\n", ss.str().c_str());
        ret = Load(ss.str(), *testMapOpenDrive14);
        ASSERT_TRUE(ret);
        testRoadOpenDrive14 =
            &getRoad(testRoadIdOpenDrive14, *testMapOpenDrive14);
    };

    void TearDown() override {}
};

/**
    * Tests parsing the HD map header.
    */
TEST_F(ParseOpenDrive14Tests, testParsingGeoReference)
{
    //  EXPECT_NEAR(testMapOpenDrive14->getHeader().north, 5.350576134016e+06,
    //              maxDifference);
    //  ASSERT_STREQ(testMapOpenDrive14->getHeader().vendor.c_str(),
    //               "3D Mapping Solutions");

    //   EXPECT_NEAR(getGeoReference(*testMapOpenDrive14).latitude, 0.0,
    //               maxDifference);
    //   EXPECT_NEAR(getGeoReference(*testMapOpenDrive14).longitude, 9.0,
    //               maxDifference);
    ASSERT_STREQ(getGeoReference(*testMapOpenDrive14).projection.c_str(),
                 "+proj=tmerc +lat_0=0 +lon_0=9 +k=0.9996 +x_0=500000 +y_0=0 "
                 "+datum=WGS84 +units=m +no_defs");
}

/**
    * Tests parsing the test road basic attributes.
    */
TEST_F(ParseOpenDrive14Tests, testParsingRoadAttributes)
{
    ASSERT_STREQ(getRoadName(*testRoadOpenDrive14).c_str(), "");
    ASSERT_EQ(getRoadJunctionId(*testRoadOpenDrive14), -1);
    ASSERT_EQ(getRoadId(*testRoadOpenDrive14),
              std::stod(testRoadIdOpenDrive14));
    EXPECT_NEAR(getRoadLength(*testRoadOpenDrive14), 1.724447338294e+03,
                maxDifference);

    // Tests parsing the test road type.
    auto type = getRoadType(*testRoadOpenDrive14)[0];
    ASSERT_EQ(getRoadType(*testRoadOpenDrive14).size(), 1);

    EXPECT_NEAR(type.s, 0.0, maxDifference);
    EXPECT_STREQ(type.type.c_str(), "motorway");

    // Tests parsing the test road speed record.
    auto speed = getRoadSpeed(*testRoadOpenDrive14)[0];
    ASSERT_EQ(getRoadSpeed(*testRoadOpenDrive14).size(), 1);

    EXPECT_NEAR(speed.s, type.s, maxDifference);
    EXPECT_DOUBLE_EQ(speed.max, 40.0);
    EXPECT_STREQ(speed.unit.c_str(), "km/h");
}

/**
    * Tests parsing the test road plan view.
    */
TEST_F(ParseOpenDrive14Tests, testParsingPlanView)
{
    const auto &planView = testRoadOpenDrive14->geometry_attributes;

    EXPECT_EQ(planView.size(), 4);
    EXPECT_EQ(planView[0]->type, GeometryType::PARAMPOLY3);
    EXPECT_NEAR(planView[0]->length, 2.874078777576e+02, maxDifference);
    EXPECT_NEAR(planView[0]->start_position, 0.000000000000e+00, maxDifference);
    EXPECT_NEAR(planView[0]->heading, 4.423314246392e+00, maxDifference);
    EXPECT_NEAR(planView[0]->start_position_x, 6.961205248677e+05,
                maxDifference);
    EXPECT_NEAR(planView[0]->start_position_y, 5.347042677392e+06,
                maxDifference);
    EXPECT_NEAR(planView[0]->start_position_z, 0.0, maxDifference);
    EXPECT_NEAR(
        static_cast<GeometryAttributesParamPoly3 *>(planView[0].get())->aU,
        -0.000000000000e+00, maxDifference);
    EXPECT_NEAR(
        static_cast<GeometryAttributesParamPoly3 *>(planView[0].get())->bU,
        1.000000000000e+00, maxDifference);
    EXPECT_NEAR(
        static_cast<GeometryAttributesParamPoly3 *>(planView[0].get())->cU,
        -2.864238929279e-11, maxDifference);
    EXPECT_NEAR(
        static_cast<GeometryAttributesParamPoly3 *>(planView[0].get())->dU,
        9.895389049440e-14, maxDifference);
    EXPECT_NEAR(
        static_cast<GeometryAttributesParamPoly3 *>(planView[0].get())->aV,
        0.000000000000e+00, maxDifference);
    EXPECT_NEAR(
        static_cast<GeometryAttributesParamPoly3 *>(planView[0].get())->bV,
        2.775557561563e-16, maxDifference);
    EXPECT_NEAR(
        static_cast<GeometryAttributesParamPoly3 *>(planView[0].get())->cV,
        -1.284829717715e-07, maxDifference);
    EXPECT_NEAR(
        static_cast<GeometryAttributesParamPoly3 *>(planView[0].get())->dV,
        3.576738398807e-10, maxDifference);
    EXPECT_STREQ(static_cast<GeometryAttributesParamPoly3 *>(planView[0].get())
                     ->p_range.c_str(),
                 "arcLength");
}

/**
    * Tests parsing the test road elevation profile.
    */
TEST_F(ParseOpenDrive14Tests, testParsingElevationProfile)
{
    auto elevationProfile =
        testRoadOpenDrive14->road_profiles.elevation_profile;
    EXPECT_EQ(elevationProfile.size(), 12);
    EXPECT_NEAR((*elevationProfile.begin()).start_offset, 0.000000000000e+00,
                maxDifference);
    EXPECT_NEAR((*--elevationProfile.end()).start_offset, 1.447594347269e+03,
                maxDifference);
}

/**
    * Tests parsing the test road lateral profile.
    */
TEST_F(ParseOpenDrive14Tests, testParsingLateralProfile)
{
    auto &lateralProfile =
        testRoadOpenDrive14->road_profiles.lateral_profile_superelevation;
    EXPECT_EQ(lateralProfile.size(), 9);
    EXPECT_NEAR((*lateralProfile.begin()).start_offset, 0.000000000000e+00,
                maxDifference);
    EXPECT_NEAR((*(--lateralProfile.end())).start_offset, 1.574499999681e+03,
                maxDifference);

    auto lateralProfileShape =
        testRoadOpenDrive14->road_profiles.lateral_profile_shape;
    EXPECT_EQ(lateralProfileShape.size(), 0);
}

/**
    * Tests parsing the test road objects.
    */
TEST_F(ParseOpenDrive14Tests, testParsingObjects)
{
    auto objects = testRoadOpenDrive14->road_objects;
    EXPECT_EQ(objects.size(), 52);
    auto testObject = objects[2];
    EXPECT_STREQ(testObject.type.c_str(), "pole");
    EXPECT_STREQ(testObject.name.c_str(), "permanentDelineator");
    EXPECT_EQ(testObject.id, 4007953);
    EXPECT_NEAR(testObject.speed, 0, maxDifference);
    EXPECT_NEAR(testObject.outline.u, 0, maxDifference);
}

/**
    * Asserts that the given lane has the given lane properties set.
    */
void assertBasicLaneProperties(const LaneInfo &lane, int id, LaneType type,
                               bool level, int widthsSize, int heightsSize,
                               int bordersSize)
{
    EXPECT_EQ(lane.attributes.id, id);
    EXPECT_EQ(lane.attributes.type, type);
    EXPECT_EQ(lane.attributes.level, level);
    EXPECT_EQ(lane.lane_border.size(), bordersSize);
    EXPECT_EQ(lane.lane_width.size(), widthsSize);
    EXPECT_EQ(lane.lane_height.size(), heightsSize);
}

/**
    * Tests parsing the test road lanes.
    */
TEST_F(ParseOpenDrive14Tests, testParsingLanes)
{
    auto &lanes = testRoadOpenDrive14->lanes;
    auto &laneOffsets = lanes.lane_offset;
    auto &laneSections = lanes.lane_sections;
    EXPECT_EQ(laneOffsets.size(), 134);
    EXPECT_EQ(laneSections.size(), 19);
    auto &laneOffset = *++laneOffsets.begin();
    EXPECT_EQ(laneOffset,
              OffsetPoly3(22.6999904090, 1.823477504728e+00, 9.794341324697e-04,
                          1.242681575038e-04, -3.618877182703e-06));
    const auto &laneSection = laneSections[1];
    EXPECT_DOUBLE_EQ(laneSection.start_position, 50.6432349799);
    EXPECT_EQ(laneSection.is_single_side, false);
    EXPECT_EQ(laneSection.left.size(), 2);
    EXPECT_EQ(laneSection.right.size(), 7);
    EXPECT_EQ(laneSection.center.size(), 1);
    const auto &lane = laneSection.center[0];
    assertBasicLaneProperties(lane, 0, LaneType::None, false, 0, 0, 0);
    const auto &lane_1 = laneSection.left[1];
    assertBasicLaneProperties(lane_1, 1, LaneType::Shoulder, false, 32, 0, 0);
    EXPECT_EQ(*(++lane_1.lane_width.begin()),
              OffsetPoly3(8.999987120617e+00, 4.842894418816e-01,
                          3.677476466731e-03, 1.318781841080e-03,
                          -8.442551396173e-05));
    const auto &lane_2 = laneSection.right[0];
    assertBasicLaneProperties(lane_2, -1, LaneType::Driving, false, 10, 0, 0);
    ASSERT_EQ(*(++lane_2.lane_width.begin()),
              OffsetPoly3(2.710000324784e+01, 3.655760640140e+00,
                          -3.708509612515e-04, -1.551918803442e-04,
                          6.621513573606e-06));
}

} // namespace tests
} // namespace opendrive_1_4