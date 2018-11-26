#pragma once
#include "blocks.h"
#include <string>
#include <map>
#include <functional>

namespace mathlab
{
  struct factory {
    template<typename TBlock> void register_block(const std::string& type_name);
    // Returns nullptr if block with given type is not registered
    // Throws invalid_argument if block is known but input stream does not contain expected number of constants
    std::unique_ptr<block> create(const std::string& type_name, std::istream& stream) const;
    std::ostream& dump_registered(std::ostream &to_stream);
  private:
    std::map<std::string, std::function<std::unique_ptr<block>(std::istream&)>> types_;
  };

  void register_all_blocks(factory& factory);
}