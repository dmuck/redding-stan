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


// forward declaration for function defined in another translation unit
stan::model::model_base &new_model(stan::io::var_context &data_context,
                                   unsigned int seed, std::ostream *msg_stream);


using time_point_t = std::chrono::time_point<std::chrono::system_clock>;

void add_option(std::stringstream& message, const int width,
		 const std::string command, const std::string description) {
  message.width(2);
  message << "";
  message.width(width);
  message << std::left << command
	  << description << std::endl;
}

std::string global_help(int argc, char* argv[]) {
  std::stringstream msg;
  msg << std::endl
      << "Usage: " << argv[0] << " [options]" << std::endl
      << std::endl
      << "Start ReddingStan, a command-line program "
      << "that smuggles log probabilities and " << std::endl
      << "gradients out of a Stan model." << std::endl
      << std::endl;

  const int n = 15;
  msg << "Options:" << std::endl;
  add_option(msg, n, "-h, --help", "Print short help message and exit");
  add_option(msg, n, "--histsize", "Set the history size. Default is 25");

  msg << std::endl
      << "Report bugs at https://github.com/dmuck/redding-stan" << std::endl
      << std::endl;
  
  return msg.str();
}

std::string global_history_size(int argc, char* argv[], int index, int& history_size) {
  // FIXME: this is actually an error and should be treated as such
  // TODO: also check that the next thing is a valid int
  if (argc <= index + 1)
    return "Please provide the history size\n";

  std::stringstream msg;
  msg << "* Setting history size from " << history_size << " to ";
  
  std::istringstream imsg(argv[index + 1]);
  imsg >> history_size;
  msg << history_size << std::endl;
  return msg.str();
}

std::string global_seed(int argc, char* argv[], int index, unsigned int& seed) {
  // FIXME: this is actually an error and should be treated as such
  // TODO: also check that the next thing is a valid int
  if (argc <= index + 1)
    return "Please provide the random seed\n";

  std::stringstream msg;
  msg << "* Setting random seed from " << seed << " to ";
  
  std::istringstream imsg(argv[index + 1]);
  imsg >> seed;
  msg << seed << std::endl;
  return msg.str();
}

std::string global_error(int argc, char* argv[]) {
  std::stringstream msg;
  msg << "ReddingStan could not start" << std::endl
      << "  Please check the arguments" << std::endl
      << std::endl;

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


std::string eval_list() {
  std::stringstream message;
  constexpr int n = 15;
  message << std::endl
	  << "List of all commands:" << std::endl;
  add_option(message, n, "list", "list all commands");
  add_option(message, n, "help", "prints basic usage");
  add_option(message, n, "status", "status of the current run");
  add_option(message, n, "N", "number of parameters in the model");
  add_option(message, n, "load", "loads data");
  add_option(message, n, "unload", "unloads data");
  add_option(message, n, "eval", "evaluate model at an unconstrained parameter value");
  add_option(message, n, "constrain", "prints constrained values from parameter values");
  add_option(message, n, "unconstrain", "prints parameter values from a constrained value");
  add_option(message, n, "history", "prints history");
  add_option(message, n, "clear", "clear history");
  add_option(message, n, "quit", "quit");
  return message.str();
}

std::string eval_history(const int count, const std::deque<std::string>& history) {
  std::stringstream msg;

  msg << std::endl;
  for (int ii = 0; ii < history.size(); ++ii) {
    msg.width(2);
    msg << "";
    msg.width(6);
    msg << std::left << ii;
    msg << history[ii] << std::endl;
  }

  return msg.str();
}

std::string eval_status(const stan::model::model_base* model, const int count,
			const time_point_t& start_time) {
  std::stringstream msg;
  time_point_t now = std::chrono::system_clock::now();
  std::time_t time = std::chrono::system_clock::to_time_t(start_time);
  int seconds = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();

  msg << std::endl;
  msg << "ReddingStan started at " << std::ctime(&time) << std::endl
      << "   elapsed time: " << seconds / 60 << " minutes " << seconds % 60 << " seconds"<< std::endl
      << "   number of commands called: " << count << std::endl
      << std::endl;

  if (model == nullptr) {
    msg << "The model has not been initialized." << std::endl;
  } else {
    msg << "The model was initialized with data file: " << "..." << std::endl
	<< "  * number of unconstrained parameters: " << "..." << std::endl;
  }
  
  return msg.str();
}

std::string eval_N(std::string& line) {
  return "... N";
}

std::string eval_load(std::string& load) {
  return "... load";
}

std::string eval(std::string& line, const int count, const std::deque<std::string>& history,
		 const stan::model::model_base* model, const time_point_t& start_time) {
  std::string command;
  std::istringstream ss(line);
  std::stringstream message;
  ss >> command;
  
  if (command == "list") {
    return eval_list();
  } else if (command == "history") {
    return eval_history(count, history);
  } else if (command == "status") {
    return eval_status(model, count, start_time);
  } else if (command == "N") {
    return eval_N(line);
  } else if (command == "load") {
    return eval_load(line);
  } else {
    message << command;
  }
  return message.str();
}

void print(std::string& message) {
  std::cout << message << std::endl;
  return;
}

int main(int argc, char* argv[]) {
  const time_point_t start_time = std::chrono::system_clock::now();
  int history_size = 25;
  unsigned int seed = 0;
  stan::model::model_base *model = nullptr;
    
  if (argc >= 2) {
    for (int ii = 1; ii < argc; ++ii) {
      if (strcmp(argv[ii], "--help") == 0
	  || strcmp(argv[ii], "-h") == 0) {
	std::cout << global_help(argc, argv);
	return 0;
      } else if (strcmp(argv[ii], "--histsize") == 0) {
	std::cout << global_history_size(argc, argv, ii, history_size);
	ii++;
      } else if (strcmp(argv[ii], "--seed") == 0) {
	std::cout << global_seed(argc, argv, ii, seed);
	ii++;
      } else {
	std::cout << global_error(argc, argv);
	return 1;
      }
    }
  }
  std::cout << startup();

  int count = 0;
  std::deque<std::string> history(history_size);
  try {
    stan::io::empty_var_context empty;
    model = &new_model(empty, seed, &std::cout);
    std::cout << "* model initialized with no data. Ready." << std::endl << std::endl;
  } catch (const std::exception& e) {
  }

  while (true) {
    echo_prompt();
    std::string line = read();
    std::string message = eval(line, count, history, model,
			       start_time);
    print(message);

    ++count;
    history.pop_front();
    history.push_back(line);
  }
  return 0;
  /*
  std::cout << "------------------------------------------------------------" << std::endl;
  std::cout << "\nWelcome to ReddingStan" << std::endl << std::endl;
  std::cout << "Number of arguments provided: " << argc << std::endl;
  std::cout << " * argv[0] is the executable: " << argv[0] << std::endl;

  std::fstream stream;
  if (argc > 1) {
    std::cout << " * argv[1] is the data file: " << argv[1] << std::endl;
    stream = std::fstream(argv[1], std::fstream::in);
  }
  stan::io::dump dump(stream);

  // Instantiate the model
  stan::model::model_base &model
      = new_model(dump, 0L, &std::cout);

  std::cout << "Model has been instantiated" << std::endl;

  std::string model_name = model.model_name();
  std::vector<std::string> model_compile_info = model.model_compile_info();
  size_t num_params_r = model.num_params_r();
  size_t num_params_i = model.num_params_i();

  std::cout << " * model_name: " << model_name << std::endl;
  std::cout << " * model_compile_info: " << std::endl;
  for (std::string info: model_compile_info) {
    std::cout << "    - " << info << std::endl;
  }
  std::cout << " * num_params_r: " << num_params_r << std::endl;
  std::cout << " * num_params_i: " << num_params_i << std::endl;

  // logic from Math's stan/math/rev/functor/gradient.hpp
  Eigen::Matrix<stan::math::var, -1, 1> parameters = Eigen::MatrixXd::Random(num_params_r, 1);
  
  std::cout << " * parameters = " << parameters << std::endl;

  std::stringstream msg;
  stan::math::var lp = model.log_prob_propto_jacobian(parameters, &msg);

  double lp_val = lp.val();
  stan::math::grad(lp.vi_);
  Eigen::VectorXd gradient = parameters.adj();

  std::cout << "------------------------------------------------------------" << std::endl;
  std::cout << " * log prob propto jacobian of parameters: " << lp_val << std::endl;
  std::cout << " * gradient: " << gradient << std::endl;
  std::cout << " * message while evaluating log prob: \"" << msg.str() << "\"" << std::endl;
  std::cout << "------------------------------------------------------------" << std::endl;  

  
  return 0;
  */
}
