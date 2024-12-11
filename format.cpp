#include <sstream>
#include "format.h"
#include <iomanip>
using std::string;

#define HOUR  3600
#define MIN   60

QString Format::ElapsedTime(long seconds) {
  int h = seconds / HOUR;
  int sec = seconds % HOUR;
  int min = sec / MIN;
  sec %= MIN;

  std::ostringstream out_stream;
  out_stream.width(2);
  out_stream.fill('0');
  out_stream << h << ":";
  out_stream.width(2);
  out_stream.fill('0');
  out_stream << min << ":";
  out_stream.width(2);
  out_stream.fill('0');
  out_stream << sec;

  return Format::formatString(out_stream.str());
}

QString Format::formatString(const string& str){
    return QString::fromStdString(str);
}

QString Format::formatDouble(double value){
    std::ostringstream out_stream;
    out_stream << std::fixed << std::setprecision(2) << value;
    return QString::fromStdString(out_stream.str());
}
