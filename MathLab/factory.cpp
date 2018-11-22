#include "factory.h"

namespace mathlab
{
  void factory::register_block(const std::string& type, fn_type from_stream_creator) {
    block_types_[type] = from_stream_creator;
  }

  std::unique_ptr<block> factory::create(const std::string& type, std::istream& input)
  {
    block* pblock = nullptr;
    const auto found = block_types_.find(type);
    if (found != block_types_.end())
      pblock = found->second(input);
    return std::unique_ptr<block>(pblock); 
  }

  std::ostream& factory::dump_registered(std::ostream& to_stream)
  {
    for (auto pair : block_types_)
      to_stream << pair.first << ' ';
    return to_stream;
  }

  void register_all_blocks(factory& factory)
  {
    factory.register_block(mathlab::identity::my_type, [](std::istream&) {return new mathlab::identity(); });
    factory.register_block(mathlab::addition::my_type, [](std::istream& input) {return new mathlab::addition(input); });
    factory.register_block(mathlab::multiplication::my_type, [](std::istream& input) {return new mathlab::multiplication(input); });
    factory.register_block(mathlab::power::my_type, [](std::istream& input) {return new mathlab::power(input); });
    factory.register_block(mathlab::condition::my_type, [](std::istream& input) {return new mathlab::condition(input); });
    factory.register_block(mathlab::limit::my_type, [](std::istream& input) {return new mathlab::limit(input); });
  }
}
