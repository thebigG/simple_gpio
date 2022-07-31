#include <errno.h>
#include <fcntl.h>
#include <fmt/core.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include "gpio.h"

struct Args {
  int pin;
  SimpleGPIO::PIN_VALUE value;
};

namespace po = boost::program_options;

void show_help(const po::options_description& desc,
               const std::string& topic = "") {
  std::cout << desc << '\n';
  std::string pins{"["};
  for (auto pin : SimpleGPIO::get_pins()) {
    pins += std::to_string(pin);
    pins += ",";
  }

  pins += "]";
  std::cout << fmt::format(
                   "Simple front-end for linux GPIO driver.\n"
                   "Configured for:\"{}\" \n"
                   "Pins available:{}",
                   SimpleGPIO::get_board(), pins)
            << std::endl;
  if (topic != "") {
    std::cout << "You asked for help on: " << topic << '\n';
  }
  exit(EXIT_SUCCESS);
}
void process_program_options(const int argc, const char* const argv[],
                             Args& in_args) {
  po::options_description desc1("Write to pin.");
  desc1.add_options()("Pin,p",
                      po::value<int>()
                          ->notifier([&desc1](const int& pin) {
                            SimpleGPIO::validate_pin(pin);
                          })
                          ->composing(),
                      "GPIO PIN.");

  po::variables_map args;

  desc1.add_options()("Value,v",
                      po::value<int>()
                          ->notifier([&desc1](const int& pin) {
                            SimpleGPIO::SimpleGPIO::get_pin_value_from_int(pin);
                          })
                          ->composing(),
                      "GPIO PIN value:{1, 0}.");

  po::options_description desc3("Usage");
  desc3.add_options()(
      "help,h",
      po::value<std::string>()->implicit_value("")->notifier(
          [&desc3](const std::string& topic) { show_help(desc3, topic); }),
      "Show help. If given, show help on the specified topic.");

  desc3.add(desc1);

  if (argc == 1) {
    show_help(desc3);  // does not return
  }

  try {
    po::store(po::parse_command_line(argc, argv, desc3), args);
  }

  catch (po::error const& e) {
    std::cerr << e.what() << '\n';
    exit(EXIT_FAILURE);
  }

  po::notify(args);
  in_args.pin = args.at("Pin").as<int>();
  in_args.value = SimpleGPIO::SimpleGPIO::get_pin_value_from_int(
      args.at("Value").as<int>());
  return;
}

int main(const int argc, const char* const argv[]) {
  Args args{};
  process_program_options(argc, argv, args);
  SimpleGPIO::write_to_pin(args.pin, args.value);
  return 0;
}
