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
	po::options_description desc1("Write to pin.");
	desc1.add_options()(
				"Pin,p",
				po::value< int >()
					->notifier(
						[&desc1](const int& pin) {
							validate_pin(desc1, pin);
						}
					)->composing(),
				"GPIO PIN."
			);

	po::variables_map args;




	po::options_description desc2("Write to all pins.");
	desc2.add_options()
			(
				"all,a",
				po::value< bool >()
					->notifier(
						[&desc2](const bool& all_pins) {
							validate_pin(desc2, all_pins);
						}
					),
				"Write to all pins."
			)
	;

	po::options_description desc3("Usage");
	desc3.add_options()
			(
						"help,h",
						po::value< std::string >()
							->implicit_value("")
							->notifier(
								[&desc3](const std::string& topic) {
									show_help(desc3, topic);
								}
							),
						"Show help. If given, show help on the specified topic."
					)
	;

	desc3.add(desc1);
	desc3.add(desc2);

	if (argc == 1) {
		show_help(desc3); // does not return
	}

	try {
		po::store(
			po::parse_command_line(argc, argv, desc3),
			args
		);
	}

	catch (po::error const& e) {
		std::cerr << e.what() << '\n';
		exit( EXIT_FAILURE );
	}

	po::notify(args);
	in_args.pin = args.at("Pin").as<int>();
	return;
}

void write_to_pin(int i)
{
	// Export the desired pin by writing to /sys/class/gpio/export
	//
	std::string gpio_pin{std::to_string(i)};

	{
	std::ofstream export_ofs{};

	//prepare f to throw if failbit gets set
	std::ios_base::iostate exceptionMask = export_ofs.exceptions() | std::ios::failbit;
	export_ofs.exceptions(exceptionMask);

	try {
		export_ofs.open("/sys/class/gpio/export");
	} catch (std::system_error& e) {
		std::cerr << e.code().message() << std::endl;
	}

	std::cout<<fmt::format("Export Pin {}", i)<<std::endl;
	std::string gpio_pin{std::to_string(i)};
	export_ofs<<gpio_pin;
	}

	// Set the pin to be an output by writing "out" to /sys/class/gpio/gpio24/direction

	{
	std::string pin_direction_file{fmt::format("/sys/class/gpio/gpio{}/direction", gpio_pin)};

	std::ofstream gpio_direction_ofs{};

	try {
		gpio_direction_ofs.open(pin_direction_file);
	} catch (std::system_error& e) {
		std::cerr << e.code().message() << std::endl;
	}
	std::string direction{"out"};
	gpio_direction_ofs<<direction;
	}

	{
	//Write to pin
	std::string pin_value_file{fmt::format("/sys/class/gpio/gpio{}/value", gpio_pin)};

	std::ofstream gpio_pin_value_ofs{};

	try {
		gpio_pin_value_ofs.open(pin_value_file);
	} catch (std::system_error& e) {
		std::cerr << e.code().message() << std::endl;
	}

	try {
		std::string value{"1"};
		gpio_pin_value_ofs<<value<<std::endl;
		std::cout<<"write 1 to pin:"<<pin_value_file<<std::endl;
	} catch (std::system_error& e) {
		std::cerr << e.code().message() << std::endl;
	}
	}
}

void write_to_all_pins()
{
	for(int i = BASE_GPIO;i<=N_GPIO;i++)
	{
		write_to_pin(i);
	}
}

void validate_pin(int pin)
{
	for(auto i: GPIO)
	{
		if(i == pin)
		{
			return;
		}
	}

	//TODO:Not sure if exceptions are the best fit here
	throw GPIOException{fmt::format(
									"Invalid PIN:{}"
									"Board info(configured at build time):{}",
									pin, BOARD)};
}

int main(const int argc, const char *const argv[])
{
	Args args{};
	process_program_options(argc, argv, args);
	try
	{
		validate_pin(args.pin);
	}
	catch (GPIOException)
	{
		std::cout<<fmt::format(
					   "Invalid PIN:{}\n"
					   "Board info(configured at build time):{}",
					   args.pin, BOARD)<<std::endl;
		return -1;
	}
	write_to_pin(args.pin);
    return 0;
}
