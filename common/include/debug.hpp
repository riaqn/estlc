#ifndef COMMON_DEBUG_H
#define COMMON_DEBUG_H
#include <iostream>

enum Level {
  LEVEL_EMERG,
  LEVEL_ALERT,
  LEVEL_CRIT,
  LEVEL_ERR,
  LEVEL_WARNING,
  LEVEL_NOTICE,
  LEVEL_INFO,
  LEVEL_DEBUG,
};
extern Level LEVEL;
  
template <Level level>
class Debug {
public:
  template <typename T>
  Debug &operator <<(const T &t);
};

template <Level level>
template <typename T>
Debug<level>& Debug<level>::operator<<(const T &t) {
  if (level <= LEVEL)
    std::cout << t;
  return (*this);
}

#endif
