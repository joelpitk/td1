#include "common.hh"

/* Helper function to trim the individual lines. */
void trim(std::string& s)
{
    if (s.find_first_not_of(" \t") != std::string::npos)
    {
        s = s.substr(s.find_first_not_of(" \t"));
    }
    if (s.find_last_not_of(" \t") != std::string::npos)
    {
        s = s.substr(0, s.find_last_not_of(" \t") + 1);
    }
    else
    {
        s = "";
    }
}
