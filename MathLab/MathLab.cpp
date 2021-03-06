#include <iostream>
#include <tuple>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "factory.h"
#include "block_sequence.h"
#include <iterator>


void dump_usage() {
  std::cout << "Commands:" << std::endl;
  std::cout << "  lr - list all registered blocks" << std::endl;
  std::cout << "  l - list all blocks in sequence" << std::endl;
  std::cout << "  a block_type [constant1] [constantN] - adds new block to sequence" << std::endl;
  std::cout << "  r position - removes block from specified position" << std::endl;
  std::cout << "  m position - moves block from specified position to first position" << std::endl;
  std::cout << "  e number - evaluates sequence using specified number" << std::endl;
  std::cout << "  ef file_name - evaluates sequence with numbers from file" << std::endl;
  std::cout << "  h - prints help" << std::endl;
  std::cout << "  x - closes application and saves current sequence" << std::endl;
}

std::string read_line() {
  std::string line;
  std::getline(std::cin, line);
  return line;
}

std::tuple<std::string, std::istringstream> extract_command(const std::string& line) {
  std::istringstream line_as_stream(line);
  std::string command;
  line_as_stream >> command;
  return std::make_tuple(std::move(command), std::move(line_as_stream));
}

void process_add_command(mathlab::block_sequence& sequence, std::istringstream& after_command) {
  try {
    auto ignored_lines = sequence.append_from(after_command);
    if (!ignored_lines.empty())
      std::cout << "!! Unknown block types:" << std::endl << ignored_lines;
  }
  catch (std::invalid_argument& exception) {
    std::cout << "!! " << exception.what() << std::endl;
  }
}

void process_remove_command(mathlab::block_sequence& sequence, std::istringstream& after_command) {
  int position = -1;
  after_command >> position;
  if (position <= 0)
    std::cout << "!! invalid position" << std::endl;
  else
    sequence.remove_at(static_cast<unsigned>(position - 1));
}

void process_move_command(mathlab::block_sequence& sequence, std::istringstream& after_command) {
  int position = -1;
  after_command >> position;
  if (position <= 0)
    std::cout << "!! invalid position" << std::endl;
  else
    sequence.move_to_beginning(static_cast<unsigned>(position - 1));
}

void process_eval_command(mathlab::block_sequence& sequence, std::istringstream& after_command) {
  double input = 0;
  after_command >> input;
  std::cout << sequence.eval(input) << std::endl;
}

void process_eval_from_file_command(const mathlab::block_sequence& sequence, const std::istringstream& after_command) {
  std::string input_file_name;
  std::copy(std::istream_iterator<char>((std::istream&) after_command), std::istream_iterator<char>(), std::back_inserter(input_file_name));
  if (input_file_name.empty()) {
    std::cout << "!! Invalid file name" << std::endl;
    return;
  }

  const auto current_path = std::filesystem::current_path();
  auto input_path = (current_path / input_file_name).lexically_normal();
  std::ifstream input_stream(input_path);
  if (!input_stream.is_open()) {
    std::cout << "!! Unable to open " << input_path;
    return;
  }
  auto output_file_name = "eval_results.txt";
  auto output_path = (current_path / output_file_name).lexically_normal();
  std::ofstream output_stream(output_path);
  std::transform(
    std::istream_iterator<double>(input_stream),
    std::istream_iterator<double>(),
    std::ostream_iterator<double>(output_stream, "\n"),
    [&sequence](double input) { return sequence.eval(input); });
  std::cout << "Results are written to: " << output_path << std::endl;
}

bool process_command(const std::string& command, std::istringstream& after_command, mathlab::block_sequence& sequence, mathlab::factory& factory) {  
  if (command.empty())
    return true;
  if (command == "x")
    return false;
  if (command == "h")
    dump_usage();
  else if (command == "lr")
    factory.dump_registered(std::cout) << std::endl;
  else if (command == "l")
    sequence.dump(std::cout, true) << std::endl;
  else if (command == "a")
    process_add_command(sequence, after_command);
  else if (command == "r")
    process_remove_command(sequence, after_command);
  else if (command == "m")
    process_move_command(sequence, after_command);
  else if (command == "e")
    process_eval_command(sequence, after_command);
  else if (command == "ef")
    process_eval_from_file_command(sequence, after_command);
  else
    std::cout << "!! Invalid command" << std::endl;
  return true;
}

std::string path_to_sequence_file() {
  const auto current_path = std::filesystem::current_path();
  auto output_path = (current_path / "sequence.txt").lexically_normal();
  return output_path.string();
}

void sequence_to_file(const mathlab::block_sequence& sequence) {
  auto path = path_to_sequence_file();
  std::cout << std::endl << "Saving sequence to file " << path;
  std::ofstream file_stream(path_to_sequence_file());
  sequence.dump(file_stream, false);
}

void sequence_from_file(mathlab::block_sequence& sequence) {
  auto path = path_to_sequence_file();
  std::ifstream file_stream(path_to_sequence_file());
  if (file_stream.is_open()) {
    std::cout << std::endl << "Loading sequence from file " << path << std::endl;
    auto invalid_lines = sequence.load_from(file_stream);
    if (! invalid_lines.empty())
      std::cout <<  "!! Invalid lines in file that are ignored:" << std::endl << invalid_lines << std::endl;
  }
}

int main() {
  auto factory = mathlab::factory();
  mathlab::register_all_blocks(factory);
  auto sequence = mathlab::block_sequence(factory);
  sequence_from_file(sequence);
  dump_usage();
  std::string command;
  std::istringstream after_command;
  do {
    std::cout << ">";
    auto line = read_line();
    std::tie(command, after_command) = extract_command(line);
  } while (process_command(command, after_command, sequence, factory));
  sequence_to_file(sequence);
}
