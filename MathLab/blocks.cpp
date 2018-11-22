#include "blocks.h"

namespace mathlab
{
  std::ostream& operator<<(std::ostream& stream, const block& block) {
    block.dump(stream);
    return stream;
  }
}