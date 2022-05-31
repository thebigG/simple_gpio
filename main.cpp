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
	if(pin < BASE_GPIO || BASE_GPIO + N_GPIO < pin)
	{
		throw GPIOException(fmt::format("Pin is out of range. Pin must be between {} and {}",
										BASE_GPIO, BASE_GPIO + N_GPIO));
	}
}

void process_program_options(const int argc, const char *const argv[])
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
					->implicit_value(1)
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
	std::cout<< args.at("Pin").as<int>()<<std::endl;
	return;
}

int main(const int argc, const char *const argv[])
{
	process_program_options(argc, argv);
	int base_gpio = BASE_GPIO;
	// Export the desired pin by writing to /sys/class/gpio/export

	std::ofstream ofs{"/sys/class/gpio/export"};

	if(!ofs.is_open())
	{
		throw GPIOException("Error opening file.");
	}

//	ofs

//	if (write(fd, "24", 2) != 2) {
//		perror("Error writing to /sys/class/gpio/export");
//		exit(1);
//	}

//	close(fd);

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
