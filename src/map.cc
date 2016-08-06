#include "map.hh"

Map::Map(std::ifstream& file, CL_GraphicContext& gc, CL_ResourceManager& resources)
{
    readMap(file, gc, resources);
}

// Returns the map dimensions as x, y (size_t) coordinate pair.
const std::pair<size_t, size_t> Map::getDimensions() const
{
    return std::pair<size_t, size_t> (width, height);
}

std::vector<std::vector<Tile> >& Map::getTiles() 
{
    return tile_array;
}

Tile& Map::getTile(size_t row, size_t column) 
{
    return tile_array[row][column];
}

// Read map function takes care of reading the map and its dimensions.
void Map::readMap(std::ifstream& file, CL_GraphicContext& gc, CL_ResourceManager& resources)
{
    // Stores the map ASCII temporarily
    std::vector<std::string> map_array;

    while (!file.eof())
    {
        
        std::string current_line;
        getline(file, current_line);
        trim(current_line);
                
        // Skipping empty lines, if a closing bracket is found break the loop.
        if (current_line.empty()) continue; 
        if (current_line.find("}") != std::string::npos) break;
                
        // Reading the map dimensions.
        if (current_line.find("=") != std::string::npos) 
        {
            // Separating the key and value parts and trimming them both.
            std::string key = current_line.substr(0, current_line.find("="));
            std::string value_str = current_line.substr(current_line.find("=") + 1);
            trim(key);
            trim(value_str);

            // Type casting the value_str into int
            int value = boost::lexical_cast<int>(value_str);
               
            if (key == "width") 
            {
                width = value;
            } 
            else if (key == "height") 
            {
                height = value;
            }
            continue;
        }
                
        // Checking that the map dimensions were read before the actual map.
        if (!width || !height) 
        {
            throw std::invalid_argument("Unable to read map dimensions.");
        }

        // Here is the read for the actual map ASCII.
        if (current_line.length() == width) 
        {
            map_array.push_back(current_line);
        }
        else 
        {
            throw std::invalid_argument("The dimensions of the map are not consistent.");
        }
    }

    // Checking the map validity.
    checkMap(map_array);

    // Convert to tiles.
    for (size_t i = 0; i < height; i++) 
    {
        std::vector<Tile> vector_row;

        for (size_t j = 0; j < width; j++) 
        {
            char tile_char = map_array[i][j];
            vector_row.push_back(Tile(tile_char, gc, resources));
        }
        tile_array.push_back(vector_row);
    }
}

void Map::addEntrance(size_t r, size_t c) 
{
    entrances.push_back(std::pair<size_t, size_t> (r,c));
}

void Map::checkMap(std::vector<std::string>& map_array) 
{   
    for (size_t i = 0; i < width; i++) 
    {
        // Top border check
        if (map_array[0][i] != '#') 
        {
            if (!isdigit(map_array[0][i])) 
            {
                throw std::invalid_argument("The top border is screwed.");
            }
            else 
            {
                addEntrance(0, i);
            }
        }

        // Bottom border check
        if (map_array[height-1][i] != '#') 
        {
            if (!isdigit(map_array[height-1][i])) 
            {
                throw std::invalid_argument("The bottom border is screwed.");
            }
            else 
            {
                addEntrance(height-1, i);
            }
        }
    }

    for (size_t i = 1; i < height-1; i++) 
    {
                
        // Left border check
        if (map_array[i][0] != '#') 
        {
            if (!isdigit(map_array[i][0])) 
            {
                throw std::invalid_argument("The left border is screwed.");
            }
            else 
            {
                addEntrance(i, 0);
            }
        }

        // Right border check
        if (map_array[i][width-1] != '#') 
        {
            if (!isdigit(map_array[i][width-1])) 
            {
                throw std::invalid_argument("The right border is screwed.");
            }
            else 
            {
                addEntrance(i,width-1);
            }
        }
    }       

    // Has exit check
    for (size_t i = 1; i < height - 1; i++) 
    {
        for (size_t j = 1; j < width - 1; j++) 
        {
            if (map_array[i][j] == 'E') 
            {
                exits.push_back(std::pair<size_t,size_t>(i, j));
            }
        }
    }

    // Final check that the map has an entrance and an exit.
    if (entrances.size() == 0 || exits.size() == 0) 
    {
    	throw std::invalid_argument("The map is missing an exit or an entrance.");
    }
}

const std::vector<std::pair<size_t, size_t> >& Map::getEntrances() const
{
    return entrances;
}
const std::vector<std::pair<size_t, size_t> >& Map::getExits() const
{
    return exits;
}

Tile& Map::operator()(const size_t row, const size_t col) 
{
    
    if (row < height && col < width) 
    {
        return tile_array[row][col];
    }
    else 
    {
	throw std::out_of_range("Tried to access coordinates not on map");
    }
}
