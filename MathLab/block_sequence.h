#pragma once
#include "factory.h"
#include <vector>
#include <memory>
#include <string>

namespace mathlab {
  class block_sequence {
    std::vector<std::pair<std::string, std::unique_ptr<block>>> blocks_;
    factory& factory_;
  public:
    block_sequence(factory& factory);
    // Appends one or more block from supplied stream
    // Returns invalid lines
    std::string append_from(std::istream& input_stream);
    // Load sequence from supplied stream
    // Returns invalid lines
    std::string load_from(std::istream& input_stream);
    std::ostream& dump(std::ostream& to_stream, bool with_line_numbers) const;
    double eval(double input) const;
    void remove_at(unsigned index);
    void move_to_beginning(unsigned index);
  };
}
