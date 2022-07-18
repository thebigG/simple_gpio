/*

Example of programming GPIO from C or C++ using the sysfs interface on
a Raspberry Pi.

Will toggle GPIO24 (physical pin 18) at a 100 millisecond rate for 10
seconds and then exit.

Jeff Tranter <jtranter@ics.com>

*/

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

void write_to_pin(int i) {
  // Export the desired pin by writing to /sys/class/gpio/export
  //
  std::string gpio_pin{std::to_string(i)};

  {
    std::ofstream export_ofs{};

    // prepare f to throw if failbit gets set
    std::ios_base::iostate exceptionMask =
        export_ofs.exceptions() | std::ios::failbit;
    export_ofs.exceptions(exceptionMask);

    try {
      export_ofs.open("/sys/class/gpio/export");
    } catch (std::system_error& e) {
      std::cerr << e.code().message() << std::endl;
    }

    std::cout << fmt::format("Export Pin {}", i) << std::endl;
    std::string gpio_pin{std::to_string(i)};
    export_ofs << gpio_pin;
  }

  // Set the pin to be an output by writing "out" to
  // /sys/class/gpio/gpio24/direction

  {
    std::string pin_direction_file{
        fmt::format("/sys/class/gpio/gpio{}/direction", gpio_pin)};

    std::ofstream gpio_direction_ofs{};

    try {
      gpio_direction_ofs.open(pin_direction_file);
    } catch (std::system_error& e) {
      std::cerr << e.code().message() << std::endl;
    }
    std::string direction{"out"};
    gpio_direction_ofs << direction;
  }

  {
    // Write to pin
    std::string pin_value_file{
        fmt::format("/sys/class/gpio/gpio{}/value", gpio_pin)};

    std::ofstream gpio_pin_value_ofs{};

    try {
      gpio_pin_value_ofs.open(pin_value_file);
    } catch (std::system_error& e) {
      std::cerr << e.code().message() << std::endl;
    }

    try {
      std::string value{"1"};
      gpio_pin_value_ofs << value << std::endl;
      std::cout << "write 1 to pin:" << pin_value_file << std::endl;
    } catch (std::system_error& e) {
      std::cerr << e.code().message() << std::endl;
    }
  }
}

void write_to_all_pins() {
  for (int i = BASE_GPIO; i <= N_GPIO; i++) {
    write_to_pin(i);
  }
}

void validate_pin(int pin) {
  for (auto i : GPIO) {
    if (i == pin) {
      return;
    }
  }

  // TODO:Not sure if exceptions are the best fit here
  throw GPIOException{
      fmt::format("Invalid PIN:{}"
                  "Board info(configured at build time):{}",
                  pin, BOARD)};
}
