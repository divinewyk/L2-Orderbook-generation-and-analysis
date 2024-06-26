#include "UtilityFunctions.h"
#include <sstream>
#include <iomanip> 

std::string floatToStringWithPrecision(float value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

