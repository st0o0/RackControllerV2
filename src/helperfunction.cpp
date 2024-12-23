#include <helperfunction.h>

bool to_bool(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  std::istringstream is(str);
  bool b;
  is >> std::boolalpha >> b;
  return b;
}

void add_value(std::vector<float> &vec, float value, size_t maxValues)
{
  if (vec.size() < maxValues)
  {
    vec.reserve(maxValues);
  }
  vec.insert(vec.begin(), value);
  if (vec.size() > maxValues)
  {
    vec.resize(maxValues);
  }
}