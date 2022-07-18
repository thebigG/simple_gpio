/*

Example of programming GPIO from C or C++ using the sysfs interface on
a Raspberry Pi.

Will toggle GPIO24 (physical pin 18) at a 100 millisecond rate for 10
seconds and then exit.

Jeff Tranter <jtranter@ics.com>

*/

#include <errno.h>
#include <fcntl.h>
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
};

namespace po = boost::program_options;

void show_help(const po::options_description& desc,
               const std::string& topic = "") {
  std::cout << desc << '\n';
  if (topic != "") {
    std::cout << "You asked for help on: " << topic << '\n';
  }
  exit(EXIT_SUCCESS);
}
void process_program_options(const int argc, const char* const argv[],
                             Args& in_args) {
  po::options_description desc1("Write to pin.");
  desc1.add_options()(
      "Pin,p",
      po::value<int>()
          ->notifier([&desc1](const int& pin) { validate_pin(pin); })
          ->composing(),
      "GPIO PIN.");

  po::variables_map args;

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
  return;
}

int main(const int argc, const char* const argv[]) {
  Args args{};
  process_program_options(argc, argv, args);
  try {
    validate_pin(args.pin);
  } catch (GPIOException a) {
    std::cout << a.what() << std::endl;
    return -1;
  }
  write_to_pin(args.pin);
  return 0;
}
