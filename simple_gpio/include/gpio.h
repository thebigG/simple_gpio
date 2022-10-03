// At the moment these defines assume linux is the target platform.
#ifndef GPIO_H_
#define GPIO_H_
#include <array>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <exception>
#include <iostream>
#include <set>
#include <string>
#include <vector>
namespace pt = boost::property_tree;

class GPIOException : public std::exception {
 public:
  GPIOException(std::string msg) { newMsg = msg; }
  virtual const char *what() const throw() { return newMsg.c_str(); }

 private:
  std::string newMsg;
};

struct SimpleGPIOConfig {
  std::string board;
  bool all_pins{false};
  std::vector<std::string> pins{};

  void load_xml(const std::string &filename) {
    // Create empty property tree object
    pt::ptree tree;

    // Parse the XML into the property tree.
    pt::read_xml(filename, tree);

    // Use the throwing version of get to find the debug filename.
    // If the path cannot be resolved, an exception is thrown.
    board = tree.get<std::string>("ping_config.board_config");

    // Use the default-value version of get to find the debug level.
    // Note that the default value is used to deduce the target type.
    all_pins = tree.get("ping_config.all_pins", false);

    // Use get_child to find the node containing the modules, and iterate over
    // its children. If the path cannot be resolved, get_child throws.

    for (pt::ptree::value_type &v : tree.get_child("ping_config.pins")) {
      pins.push_back(v.second.data());
    }
  }
  void save(const std::string &filename) {
    // TODO
    //		// Create an empty property tree object.
    //		pt::ptree tree;

    //		// Put the simple values into the tree. The integer is
    // automatically
    //		// converted to a string. Note that the "debug" node is
    // automatically
    //		// created if it doesn't exist.
    //		tree.put("debug.filename", m_file);
    //		tree.put("debug.level", m_level);

    //		// Add all the modules. Unlike put, which overwrites existing
    //nodes, add
    //		// adds a new node at the lowest level, so the "modules" node
    //will have
    //		// multiple "module" children.

    //		for(auto &name: m_modules)
    //		{
    //			tree.add("debug.modules.module", name);
    //		}

    //		// Write property tree to XML file
    //		pt::write_xml(filename, tree);
  }
};

// This is ugly, but I'm not sure that I have much of a choice.
// I really don't want to make SimpleGPIO an instance class and keep track
// of "state".
static SimpleGPIOConfig config{};

class SimpleGPIO {
 public:
  enum class PIN_VALUE { ON = 1, OFF = 0 };

  static PIN_VALUE get_pin_value_from_int(int val);

  static void write_to_pin(int pin, PIN_VALUE value);
  static void validate_pin(int pin);
  static void load_config(std::string &filename);

  static SimpleGPIOConfig get_config();
  static std::vector<int> get_pins();

 private:
  static void export_pin(int pin);
  static void set_direction_of_pin(std::string direction, int pin);
};
#endif
