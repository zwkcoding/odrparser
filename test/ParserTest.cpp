#include <gtest/gtest.h>
#include <odrparser/odrparser.h>
#include <sstream>

#ifndef TRACKS_DIR
#define TRACKS_DIR "./"
#endif

TEST(ParserTest, DemoCode)
{

    // create file name
    std::stringstream ss;
    ss << TRACKS_DIR << "/sample1.1.xodr";

    // create container instance
    odr::OpenDRIVEFile odrData;

    // load xml file content to container (replace <...> by the file name)
    odr::loadFile(ss.str(), odrData);

    // pointer to the ODR data
    odr_1_5::OpenDRIVE *odrr = odrData.OpenDRIVE1_5.get();

    // access the header
    const auto header = odrr->sub_header.get();
    std::cout << *header->_date << std::endl; // e.g. "Thu Feb  8 14:24:06 2007"

    // access the roads vector
    const auto &roads = odrr->sub_road;
    std::cout << roads.size() << std::endl; // e.g. 36

    // access a single road content
    const auto &rd = odrr->sub_road.front();
    std::cout << rd.sub_lanes->sub_laneSection.size() << std::endl; // e.g. 1
}

TEST(ParserTest, LoadODRFile15)
{

    odr::OpenDRIVEFile odrFile;

    // create file name
    std::stringstream ss;
    ss << TRACKS_DIR << "/sample1.1.xodr";

    // load file
    odr::loadFile(ss.str(), odrFile);
    odr_1_5::OpenDRIVE *odrr = odrFile.OpenDRIVE1_5.get();

    const auto header = odrr->sub_header.get();
    EXPECT_EQ("Thu Feb  8 14:24:06 2007", *header->_date);

    const auto &roads = odrr->sub_road;
    EXPECT_EQ(36, roads.size());

    const auto &rd = odrr->sub_road.front();

    EXPECT_EQ(1, rd.sub_lanes->sub_laneSection.size());
    EXPECT_EQ(2,
              rd.sub_lanes->sub_laneSection.front().sub_left->sub_lane.size());
    EXPECT_EQ(
        1,
        *rd.sub_lanes->sub_laneSection.front().sub_left->sub_lane.back()._id);
    EXPECT_EQ(
        2,
        *rd.sub_lanes->sub_laneSection.front().sub_left->sub_lane.front()._id);
    EXPECT_DOUBLE_EQ(1.5, *rd.sub_lanes->sub_laneSection.front()
                               .sub_left->sub_lane.front()
                               .sub_width.front()
                               ._a);

    EXPECT_TRUE(rd.sub_planView->sub_geometry.front().sub_line);
    EXPECT_TRUE(rd.sub_planView->sub_geometry.at(1).sub_spiral);
    EXPECT_TRUE(rd.sub_planView->sub_geometry.at(2).sub_arc);

    EXPECT_FALSE(rd.sub_planView->sub_geometry.front().sub_arc);
    EXPECT_FALSE(rd.sub_planView->sub_geometry.at(1).sub_line);
    EXPECT_FALSE(rd.sub_planView->sub_geometry.at(2).sub_spiral);
}
