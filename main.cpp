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
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <fmt/core.h>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "gpio.h"

struct Args
{
	int pin;
};

class GPIOException : public std::exception
{
public:
	GPIOException(std::string msg)
	{
		newMsg = msg;
	}
private:
	std::string newMsg;
  virtual const char* what() const throw()
  {
	return newMsg.c_str();
  }
};


namespace po = boost::program_options;


void show_help(const po::options_description& desc, const std::string& topic = "")
{
	std::cout << desc << '\n';
	if (topic != "") {
		std::cout << "You asked for help on: " << topic << '\n';
	}
	exit( EXIT_SUCCESS );
}

void validate_pin(const po::options_description& desc, const int& pin)
{
	std::cout<<"pin:"<<pin<<std::endl;
	if(pin < BASE_GPIO || BASE_GPIO + N_GPIO -1 < pin)
	{
		throw GPIOException(fmt::format("Pin {} is out of range. Pin must be between {} and {}",
										pin, BASE_GPIO, BASE_GPIO + N_GPIO-1));
	}
}

void process_program_options(const int argc, const char *const argv[], Args& in_args)
{
	po::options_description desc("Usage");
	desc.add_options()
		(
			"help,h",
			po::value< std::string >()
				->implicit_value("")
				->notifier(
					[&desc](const std::string& topic) {
						show_help(desc, topic);
					}
				),
			"Show help. If given, show help on the specified topic."
		)(
				"Pin,p",
				po::value< int >()
					->notifier(
						[&desc](const int& pin) {
							validate_pin(desc, pin);
						}
					),
				"GPIO PIN."
			)
	;

	if (argc == 1) {
		show_help(desc); // does not return
	}

	po::variables_map args;

	try {
		po::store(
			po::parse_command_line(argc, argv, desc),
			args
		);
	}
	catch (po::error const& e) {
		std::cerr << e.what() << '\n';
		exit( EXIT_FAILURE );
	}
	po::notify(args);
	in_args.pin = args.at("Pin").as<int>();
//	std::cout<< args.at("Pin").as<int>()<<std::endl;
	return;
}

int main(const int argc, const char *const argv[])
{
	Args args{};
	process_program_options(argc, argv, args);
	int base_gpio = BASE_GPIO;
	// Export the desired pin by writing to /sys/class/gpio/export

	std::ofstream export_ofs{"/sys/class/gpio/export"};

	if(!export_ofs.is_open())
	{
		throw GPIOException("Error opening file.");
	}
	std::cout<<fmt::format("Export Pin {}", args.pin)<<std::endl;
	std::string gpio_pin{std::to_string(args.pin)};
	export_ofs<<gpio_pin;

	std::string pin_file{fmt::format("/sys/class/gpio/gpio{}/direction", gpio_pin)};

	std::ofstream gpio_direction_ofs{pin_file};

	if(!gpio_direction_ofs.is_open())
	{
		throw GPIOException(fmt::format("Error opening {}", pin_file));
	}
	export_ofs<<"out";


//    // Set the pin to be an output by writing "out" to /sys/class/gpio/gpio24/direction

//    fd = open("/sys/class/gpio/gpio24/direction", O_WRONLY);
//    if (fd == -1) {
//        perror("Unable to open /sys/class/gpio/gpio24/direction");
//        exit(1);
//    }

//    if (write(fd, "out", 3) != 3) {
//        perror("Error writing to /sys/class/gpio/gpio24/direction");
//        exit(1);
//    }

//    close(fd);

//    fd = open("/sys/class/gpio/gpio24/value", O_WRONLY);
//    if (fd == -1) {
//        perror("Unable to open /sys/class/gpio/gpio24/value");
//        exit(1);
//    }

//    // Toggle LED 50 ms on, 50ms off, 100 times (10 seconds)

//    for (int i = 0; i < 100; i++) {
//        if (write(fd, "1", 1) != 1) {
//            perror("Error writing to /sys/class/gpio/gpio24/value");
//            exit(1);
//        }
//        usleep(50000);

//        if (write(fd, "0", 1) != 1) {
//            perror("Error writing to /sys/class/gpio/gpio24/value");
//            exit(1);
//        }
//        usleep(50000);
//    }

//    close(fd);

//    // Unexport the pin by writing to /sys/class/gpio/unexport

//    fd = open("/sys/class/gpio/unexport", O_WRONLY);
//    if (fd == -1) {
//        perror("Unable to open /sys/class/gpio/unexport");
//        exit(1);
//    }

//    if (write(fd, "24", 2) != 2) {
//        perror("Error writing to /sys/class/gpio/unexport");
//        exit(1);
//    }

//    close(fd);

    // And exit
    return 0;
}
