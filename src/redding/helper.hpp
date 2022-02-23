#include <iomanip>
#include <iostream>
#include <sstream>

std::string global_help() {
  return "";
}

std::string global_error(int argc, char* argv[]) {
  std::stringstream msg;
  msg << "ReddingStan" << std::endl;

  return msg.str();
}

std::string startup() {
  std::stringstream msg;
  msg << "ReddingStan version 0.1 alpha" << std::endl
      << std::endl
      << "ReddingStan is free software and comes with ABSOLUTELY NO WARRANTY." << std::endl
      << std::endl
      << "Type 'help' for some help, 'list' a list of commands." << std::endl
      << std::endl;
  return msg.str();
}

void echo_prompt() {
  std::cout << "[redding]$ ";
  return;
}

std::string read() {
  std::string line;
  std::getline(std::cin, line);
  return line;
}

void add_command(std::stringstream& message, const int width,
		 const std::string command, const std::string description) {
  message.width(2);
  message << "";
  message.width(width);
  message << std::left << command
	  << description << std::endl;
}

std::string eval_list() {
  std::stringstream message;
  constexpr int n = 15;
  message << std::endl
	  << "List of all commands:" << std::endl;
  add_command(message, n, "list", "list all commands");
  add_command(message, n, "help", "prints basic usage");
  add_command(message, n, "status", "status of the current run");
  add_command(message, n, "N", "number of parameters in the model");
  add_command(message, n, "load", "loads data");
  add_command(message, n, "unload", "unloads data");
  add_command(message, n, "eval", "evaluate model at an unconstrained parameter value");
  add_command(message, n, "constrain", "prints constrained values from parameter values");
  add_command(message, n, "unconstrain", "prints parameter values from a constrained value");
  add_command(message, n, "history", "prints history");
  add_command(message, n, "clear", "clear history");
  add_command(message, n, "quit", "quit");
  return message.str();
}


std::string eval(std::string& line) {
  std::string command;
  std::istringstream ss(line);
  std::stringstream message;
  ss >> command;
  
  if (command == "list") {
    return eval_list();
  } else {
    message << command;
  }
  
  
  return message.str();
}

void print(std::string& message) {
  std::cout << message << std::endl;
  return;
}
