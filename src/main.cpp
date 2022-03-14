#include <chrono>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stan/model/model_header.hpp>
#include <stan/model/model_base.hpp>
#include <stan/io/dump.hpp>
#include <stan/io/empty_var_context.hpp>
#include <redding/helper.hpp>


int main(int argc, char* argv[]) {
  const std::chrono::time_point<std::chrono::system_clock> start_time
    = std::chrono::system_clock::now();
  int history_size = 25;
  unsigned int seed = 0;
  std::string data_filename;
  stan::model::model_base *model = nullptr;

  // parse startup options
  if (argc >= 2) {
    for (int ii = 1; ii < argc; ++ii) {
      if (strcmp(argv[ii], "--help") == 0
	  || strcmp(argv[ii], "-h") == 0) {
	std::cout << redding::global_help(argc, argv) << std::endl;
	return 0;
      } else if (strcmp(argv[ii], "--histsize") == 0) {
	std::cout << redding::global_history_size(argc, argv, ii, history_size) << std::endl;
	ii++;
      } else if (strcmp(argv[ii], "--seed") == 0) {
	std::cout << redding::global_seed(argc, argv, ii, seed) << std::endl;
	ii++;
      } else {
	std::cout << redding::global_error(argc, argv) << std::endl;
	return 1;
      }
    }
  }

  std::cout << redding::startup_message() << std::endl;

  int count = 0;
  std::deque<std::string> history(history_size);
  try {
    stan::io::empty_var_context empty;
    model = &new_model(empty, seed, &std::cout);
    std::cout << "* model initialized with no data. Ready." << std::endl << std::endl;
  } catch (const std::exception& e) {
    //no op
  }

  while (true) {
    std::string line = redding::read();
    std::string message = redding::eval(line, count, history,
					data_filename,
					&model,
					start_time,
					seed);

    std::cout << message << std::endl;

    ++count;
    history.pop_front();
    history.push_back(line);
  }
  return 0;
}
