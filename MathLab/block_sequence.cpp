#include "block_sequence.h"
#include <numeric>
#include <sstream>

namespace {
  std::pair<std::string, std::istringstream> extract_type(const std::string& line)
  {
    std::string block_type;
    std::istringstream line_stream(line);
    line_stream >> block_type;
    return std::make_pair(std::move(block_type), std::move(line_stream));
  }
}

namespace mathlab {

  block_sequence::block_sequence(factory& factory) : factory_(factory) {}

  std::string block_sequence::append_from(std::istream& input_stream) {
    std::string block_type;
    std::istringstream constants;
    std::string invalid_lines;
    std::string line;
    while (!input_stream.eof()) {
      std::getline(input_stream, line);
      std::tie(block_type, constants) = extract_type(line);
      if (!block_type.empty()) {
        std::unique_ptr<block> block;
        try {
          block = factory_.create(block_type, constants);
        }
        catch (std::invalid_argument&) {
          // Deliberately empty - handle same as unknown command
        }
        if (block != nullptr)
          blocks_.push_back(std::make_pair(std::move(block_type), std::move(block)));
        else
          invalid_lines += line + "\n";
      }
    }
    return invalid_lines;
  }

  std::string block_sequence::load_from(std::istream& input_stream) {
    blocks_.clear();
    return append_from(input_stream);
  }

  std::ostream& block_sequence::dump(std::ostream& to_stream, bool with_line_numbers) const {
    int position = 1;
    for (auto& name_with_block : blocks_) {
      if (with_line_numbers)
        to_stream << position++ << ": ";
      to_stream << name_with_block.first << ' ' << *name_with_block.second << std::endl;
    }
    return to_stream;
  }

  double block_sequence::eval(double input) const {
    return std::accumulate(
      blocks_.cbegin(),
      blocks_.cend(),
      input,
      [](double val, const std::pair<std::string, std::unique_ptr<block>> &name_with_block) { return name_with_block.second->eval(val); });
  }

  void block_sequence::remove_at(unsigned index) {
    if (index < blocks_.size())
      blocks_.erase(blocks_.begin() + index);
  }

  void block_sequence::move_to_beginning(unsigned index) {
    if (index < blocks_.size() && index > 0) {
      std::swap(blocks_[0], blocks_[index]);
    }
  }
}
