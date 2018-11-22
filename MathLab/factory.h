#pragma once
#include "blocks.h"
#include <string>
#include <map>
#include <functional>

namespace mathlab
{
  struct factory final {
    using fn_type = std::function<block*(std::istream&)>;
    void register_block(const std::string& type, fn_type from_stream_creator);
    // Returns nullptr if block with given type is not registered
    // Throws invalid_argument if block is known but input stream does not contain expected number of constants
    std::unique_ptr<block> create(const std::string& type, std::istream& input);
    std::ostream& dump_registered(std::ostream &to_stream);
  private:
    std::map<std::string, fn_type> block_types_;
  };

  void register_all_blocks(factory& factory);
}