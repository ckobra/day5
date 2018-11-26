#include "factory.h"

namespace mathlab
{
  template <typename TBlock>
  void factory::register_block(const std::string& type_name) {
    types_[type_name] = TBlock::create_from_stream;
  }

  std::unique_ptr<block> factory::create(const std::string& type_name, std::istream& stream) const {
    const auto found = types_.find(type_name);
    if (found != types_.end())
      return found->second(stream);
    return std::unique_ptr<block>();
  }

  std::ostream& factory::dump_registered(std::ostream& to_stream) {
    for (auto pair : types_)
      to_stream << pair.first << ' ';
    return to_stream;
  }

  void register_all_blocks(factory& factory) {
    factory.register_block<identity>("identity");
    factory.register_block<addition>("addition");
    factory.register_block<multiplication>("multiplication");
    factory.register_block<power>("power");
    factory.register_block<condition>("condition");
    factory.register_block<addition>("addition");
    factory.register_block<limit>("limit");
  }
}
