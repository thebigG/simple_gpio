#include "gpio.h"

#include <errno.h>
#include <fcntl.h>
#include <fmt/core.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <exception>
#include <fstream>
#include <iostream>
#include <string>

void SimpleGPIO::export_pin(int pin) {
  std::ofstream export_ofs{};

  // prepare f to throw if failbit gets set
  std::ios_base::iostate exceptionMask =
      export_ofs.exceptions() | std::ios::failbit;
  export_ofs.exceptions(exceptionMask);

  try {
    export_ofs.open("/sys/class/gpio/export");
  } catch (std::system_error& e) {
    std::cerr << e.code().message() << std::endl;
    throw GPIOException{"Error on export_pin"};
  }

  std::cout << fmt::format("Export Pin {}", pin) << std::endl;
  std::string gpio_pin{std::to_string(pin)};
  export_ofs << gpio_pin;
}

void SimpleGPIO::set_direction_of_pin(std::string direction, int pin) {
  std::string gpio_pin{std::to_string(pin)};
  std::string pin_direction_file{
      fmt::format("/sys/class/gpio/gpio{}/direction", gpio_pin)};

  std::ofstream gpio_direction_ofs{};

  // prepare f to throw if failbit gets set
  std::ios_base::iostate exceptionMask =
      gpio_direction_ofs.exceptions() | std::ios::failbit;
  gpio_direction_ofs.exceptions(exceptionMask);

  try {
    gpio_direction_ofs.open(pin_direction_file);
  } catch (std::system_error& e) {
    throw GPIOException{"set_direction_of_pin"};
    std::cerr << e.code().message() << std::endl;
  }
  gpio_direction_ofs << direction;
}

void SimpleGPIO::write_to_pin(int pin, PIN_VALUE value) {
  export_pin(pin);
  set_direction_of_pin("out", pin);

  try {
    SimpleGPIO::validate_pin(pin);
  } catch (GPIOException a) {
    std::cout << a.what() << std::endl;
    return;
  }
  std::string gpio_pin{std::to_string(pin)};
  // Write to pin
  std::string pin_value_file{
      fmt::format("/sys/class/gpio/gpio{}/value", gpio_pin)};

  std::ofstream gpio_pin_value_ofs{};

  // prepare f to throw if failbit gets set
  std::ios_base::iostate exceptionMask =
      gpio_pin_value_ofs.exceptions() | std::ios::failbit;
  gpio_pin_value_ofs.exceptions(exceptionMask);

  try {
    gpio_pin_value_ofs.open(pin_value_file);
  } catch (std::system_error& e) {
    std::cerr << e.code().message() << std::endl;
  }

  try {
    std::string pin_value{std::to_string(static_cast<int>(value))};
    gpio_pin_value_ofs << pin_value << std::endl;
    std::cout << "write to pin:" << pin_value_file << std::endl;
  } catch (std::system_error& e) {
    std::cerr << e.code().message() << std::endl;
  }
}

void SimpleGPIO::validate_pin(int pin) {
  //  for (auto i : GPIO) {
  //    if (i == pin) {
  //      return;
  //    }
  //  }

  //  // TODO:Not sure if exceptions are the best fit here
  //  throw GPIOException{
  //      fmt::format("Invalid PIN:{}"
  //                  "Board info(configured at build time):{}",
  //                  pin, BOARD)};
}

SimpleGPIOConfig SimpleGPIO::get_config() { return config; }
std::vector<int> SimpleGPIO::get_pins() {
  //  std::vector<int> pins{};
  //  for (auto pin : GPIO) {
  //	pins.push_back(pin);
  //  }
  //  return pins;
}

SimpleGPIO::PIN_VALUE SimpleGPIO::get_pin_value_from_int(int val) {
  if (val > 1 || val < 0) {
    throw GPIOException{"{} is out of range for PIN_VALUE type"};
  }

  return static_cast<PIN_VALUE>(val);
}

void SimpleGPIO::load_config(std::string& filename) {
  try {
    SimpleGPIOConfig ds;
    ds.load_xml(filename);
    std::cout << "Success\n";
  } catch (std::exception& e) {
    std::cout << "Error: " << e.what() << "\n";
  }
}
