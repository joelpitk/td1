#include "level.hh"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LevelTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( my_level_test ) {

    std::string filename = "testmap.txt";
    Level t = Level(filename);

    const std::vector<std::pair<size_t, size_t> >& entrances = t.getEntrances();
    const std::vector<std::pair<size_t, size_t> >& exits = t.getExits();

    BOOST_TEST_MESSAGE("testing " << filename);

    BOOST_CHECK(entrances.size() == 2);
    BOOST_CHECK(exits.size() == 1);

    BOOST_CHECK(entrances.at(0).second == entrances.at(1).second);
    BOOST_CHECK(entrances.at(0).first == 11);
    BOOST_CHECK(entrances.at(1).first == 9);

    BOOST_TEST_MESSAGE("entrance: " << entrances.at(0).first << "," << entrances.at(0).second);
    BOOST_TEST_MESSAGE("entrance: " << entrances.at(1).first << "," << entrances.at(1).second);
    BOOST_TEST_MESSAGE("testing " << filename);

}
