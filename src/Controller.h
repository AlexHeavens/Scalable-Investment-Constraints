#ifndef PROJECT_CONTROLLER_H
#define PROJECT_CONTROLLER_H

#include <iostream>

namespace project {

class Controller {
private:

  bool hasExecuted = false;

public:

  inline void run() {
    std::cout << "Controller executed.\n";
    hasExecuted = true;
  }

  inline bool getHasExecuted() {
    return hasExecuted;
  }

};

}

#endif // PROJECT_CONTROLLER_H
