// At the moment these defines assume linux is the target platform.
#ifndef GPIO_H_
#define GPIO_H_
#include <array>
#include <exception>
#include <string>
#include <vector>
#ifdef ZEDBOARD
#define BASE_GPIO (906)
#define N_GPIO (118)
#define BOARD "ZEDBOARD"
std::array<int, 4> GPIO{};
#endif
#ifdef PI4_B
#define BOARD "Raspberry Pi 4 Model B"
#define N_GPIO (13)
std::array<int, N_GPIO> GPIO{5, 6, 16, 17, 19, 20, 21, 27, 22, 23, 24, 25, 26};
#endif

class GPIOException : public std::exception {
 public:
  GPIOException(std::string msg) { newMsg = msg; }
  virtual const char* what() const throw() { return newMsg.c_str(); }

 private:
  std::string newMsg;
};

class SimpleGPIO {
 public:
  enum class PIN_VALUE { ON = 1, OFF = 0 };

  static PIN_VALUE get_pin_value_from_int(int val);

  static void write_to_pin(int pin, PIN_VALUE value);
  static void validate_pin(int pin);

  static std::string get_board();
  static std::vector<int> get_pins();

 private:
  static void export_pin(int pin);
  static void set_direction_of_pin(std::string direction, int pin);
};
#endif
