// Defines some types to make it easier to describe and work with cellular automata state
#include "types.h"

namespace ca {

bool operator==(const World &lhs, const World &rhs) {
  // first check if dimensions match
  if (lhs.width != rhs.width || lhs.height != rhs.height) {
    return false;
  }

  // if dimensions match, compare the state vectors
  return lhs.state == rhs.state;
}

bool operator!=(const World &lhs, const World &rhs) {
  return !(lhs == rhs);
}

} // namespace ca
