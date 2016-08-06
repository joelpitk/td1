#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <limits>

#define NORM_COST 10
#define DIAG_COST 14

#define DEBUG false

#define LARGE_VALUE std::numeric_limits<std::size_t>::max()/1024

#define TILE_SIZE 32
#define FPS 60
#define FRAME 17
#define DT 17.0f

#define STARTING_MONEY 600
#define STARTING_HEALTH 20

enum directions {NO, UP, DOWN, LEFT, RIGHT};

#endif
