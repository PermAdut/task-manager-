#ifndef FORMAT_H
#define FORMAT_H

#include <QString>
#include <string>

namespace Format {
QString ElapsedTime(long times);
QString formatString(const std::string& str);
QString formatDouble(double value);
};


#endif
