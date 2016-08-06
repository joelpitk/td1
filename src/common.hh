#ifndef COMMON_HH
#define COMMON_HH

#include <boost/lexical_cast.hpp>

#include <stdexcept>
#include <iostream>
#include <fstream>

#include <map>
#include <vector>
#include <list>
#include <string>

#include "constants.hh"

typedef std::map<std::string, std::string> KeyvalueMap;

void trim(std::string& s);

#endif
