#ifndef MAP_HH
#define MAP_HH

#include "common.hh"
#include "tile.hh"
#include <stdexcept>

class Map {

public:
    Map() { }
    Map(std::ifstream& file, CL_GraphicContext& gc, CL_ResourceManager& resources);
    const std::pair<size_t, size_t> getDimensions() const;
    std::vector<std::vector<Tile> >& getTiles();
    Tile& getTile(size_t row, size_t column);

    const std::vector<std::pair<size_t, size_t> >& getEntrances() const;
    const std::vector<std::pair<size_t, size_t> >& getExits() const;

    Tile& operator()(const size_t row, const size_t col);

private:

    void readMap(std::ifstream &file, CL_GraphicContext& gc, CL_ResourceManager& resources);
    void checkMap(std::vector<std::string>& map_array);
    void addEntrance(size_t row, size_t column);

    size_t width;
    size_t height;
    std::vector<std::pair<size_t, size_t> > entrances;
    std::vector<std::pair<size_t, size_t> > exits;
    std::vector<std::vector<Tile> > tile_array;
};

#endif
