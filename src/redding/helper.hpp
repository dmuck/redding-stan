#ifndef REDDING_HELPER_HPP
#define REDDING_HELPER_HPP

#include <vector>

// forward declaration for function defined in another translation unit
stan::model::model_base &new_model(stan::io::var_context &data_context,
                                   unsigned int seed, std::ostream *msg_stream);

using time_point_t = std::chrono::time_point<std::chrono::system_clock>;

namespace redding {

  void add_option(std::stringstream& message, const int width,
		  const std::string& command, const std::string& description) {
    message.width(2);
    message << "";
    message.width(width);
    message << std::left << command
	    << description << "\n";
  }

  void add_option(std::stringstream& message, const int width,
		  const std::string& command, const std::vector<std::string> description_lines) {
    message.width(2);
    message << "";
    message.width(width);
    message << std::left << command << "";

    for (int n = 0; n < description_lines.size(); ++n) {
      if (n == 0) {
	message << description_lines[n] << "\n";
      } else {
	message.width(width + 2);
	message << std::left << "" << description_lines[n] << "\n";
      }
    }
  }

  std::string global_help(int argc, char* argv[]) {
    std::stringstream msg;
    msg << "\nUsage: " << argv[0] << " [options]\n\n"
	<< "Start ReddingStan, a command-line program "
	<< "that smuggles log probabilities and \n"
	<< "gradients out of a Stan program.\n"
	<< "\n";

    const int n = 15;
    msg << "Options:\n";
    add_option(msg, n, "-h, --help", "Print short help message and exit");
    add_option(msg, n, "--histsize", "Set the history size. Default is 25");

    msg << "\n"
	<< "Report bugs at https://github.com/dmuck/redding-stan"
	<< "\n\n";

    return msg.str();
  }

  std::string global_history_size(int argc, char* argv[], int index, int& history_size) {
    // FIXME: this is actually an error and should be treated as such
    // TODO: also check that the next thing is a valid int
    if (argc <= index + 1)
      return "Please provide the history size\n";

    int orig_history_size = history_size;

    std::istringstream imsg(argv[index + 1]);
    imsg >> history_size;

    std::stringstream msg;
    msg << "[INFO] Setting history size from " << orig_history_size << " to " << history_size;
    return msg.str();
  }

  std::string global_seed(int argc, char* argv[], int index, unsigned int& seed) {
    // FIXME: this is actually an error and should be treated as such
    // TODO: also check that the next thing is a valid int
    if (argc <= index + 1)
      return "Please provide the random seed\n";

    unsigned int orig_seed = seed;

    std::istringstream imsg(argv[index + 1]);
    imsg >> seed;

    std::stringstream msg;
    msg << "[INFO] Setting random seed from " << orig_seed << " to " << seed;
    return msg.str();
  }

  std::string global_error(int argc, char* argv[]) {
    std::stringstream msg;
    msg << "ReddingStan could not start\n"
	<< "  Please check the arguments\n";

    return msg.str();
  }


  std::string startup_message() {
    return R""""(
ReddingStan version 1.0

ReddingStan is free software and comes with ABSOLUTELY NO WARRANTY.

Type 'help' for some help, 'list' a list of commands.
)"""";
  }

  void echo_prompt() {
    std::cout << "[redding]$ " << std::flush;
    return;
  }

  std::string read_std_cin() {
    std::string input;

    while (true) {
      echo_prompt();
      std::getline(std::cin, input);
      if (!std::cin) {
	std::cin.clear();
	std::clearerr(stdin);
      } else if (input.length() > 0) {
	return input;
      } else {
	std::cout << "Invalid input. Try again." << std::endl;
      }
    }
  }

  std::string eval_list() {
    std::stringstream message;
    constexpr int n = 15;
    message << "\nList of all commands:\n";
    add_option(message, n, "list", "list all commands");
    add_option(message, n, "help", "prints basic usage");
    add_option(message, n, "status", "status of the current run");
    add_option(message, n, "N", "number of parameters in the model");
    add_option(message, n, "load", "loads data");
    add_option(message, n, "unload", "unloads data");

    const std::vector<std::string> eval_description = {
      "evaluate log probability of the model at an unconstrained parameter value",
      "dropping constants; equivalent to `eval_J_true`"
    };
    add_option(message, n, "eval", eval_description);

    const std::vector<std::string> eval_description_J_true = {
      "evaluate log probability of the model at an unconstrained parameter value",
      "with Jacobian adjustment, dropping constants. Output:",
      "  Line 1:  log prob",
      "  Line 2:  gradient; length N",
      "  Line 3:  evaluation time (μs)",
      "  Line 4+: messages from the program"
    };
    add_option(message, n, "eval_J_true", eval_description_J_true);

    const std::vector<std::string> eval_description_J_false = {
      "evaluate log probability of the model at an unconstrained parameter value",
      "excluding Jacobian adjustment, dropping constants. Output format is the same",
      "as `eval_J_true`."
    };
    add_option(message, n, "eval_J_false", eval_description_J_false);

    const std::vector<std::string> eval_description_J_only = {
      "evaluate the log absolute determinant of the Jacobian; does not include",
      "a gradient. Output:",
      "  Line 1:  log prob",
      "  Line 2:  evaluation time (μs)",
      "  Line 3+: messages from the program"
    };
    add_option(message, n, "eval_J_only", eval_description_J_only);
    //add_option(message, n, "constrain", "prints constrained values from parameter values");
    //add_option(message, n, "unconstrain", "prints parameter values from a constrained value");
    add_option(message, n, "history", "prints history");
    //add_option(message, n, "clear", "clear history");
    add_option(message, n, "quit", "quit");
    return message.str();
  }

  std::string eval_help() {
    std::stringstream message;

    message << "\n"
	    << "Welcome to ReddingStan.\n"
	    << "\n"
	    << "This is a command-line program that smuggles log probabilities\n"
	    << "and gradients out of a Stan program.\n"
	    << "\n"
	    << "Typical steps:\n"
	    << "1. Load a dataset (if required). See `load`.\n"
	    << "2. Evaluate! Pass in unconstrained parameter values (comma separated),\n"
	    << "             get out log probability distribution function and gradients.\n"
	    << "\n"
	    << "   `eval_J_true`:  includes the log absolute determinant of the Jacobian\n"
	    << "   `eval_J_false`: computes the lpdf without the Jacobian adjustment\n"
	    << "   `eval_J_only`:  computes the log absolute determinant of the Jacobian\n"
	    << "   `eval`:         is an alias for `eval_J_true`.\n"
	    << "\n"
	    << "   The output from these commands are meant to be parsed from a program:\n"
	    << "      Line 1:  the scalar value of what's being requested\n"
	    << "      Line 2:  the gradient as a comma-separated string;\n"
	    << "               not applicable for `eval_J_only`\n"
	    << "      Line 3:  evaluation time in microseconds\n"
	    << "      Line 4+: messages from the Stan program\n"
	    << "\n";
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
      msg << history[ii] << "\n";
    }
    return msg.str();
  }

  std::string eval_status(const stan::model::model_base* model, const int count,
			  const time_point_t& start_time, const std::string& data_filename) {
    std::stringstream msg;
    time_point_t now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(start_time);
    int seconds = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();

    msg << "[INFO] ReddingStan started at " << std::ctime(&time) << "\n"
	<< "   elapsed time:              "
	<< seconds / 60 << " minutes " << seconds % 60 << " seconds\n"
	<< "   number of commands called: " << count << "\n\n";
    if (model == nullptr) {
      msg << "The model is uninitialized. Please use the 'load' command.\n";
    } else {
      msg << "The model is initialized\n"
	  << "  - data file:                          \"" << data_filename << "\"\n"
	  << "  - number of unconstrained parameters: " << model->num_params_r() << "\n";
    }
    return msg.str();
  }

  std::string eval_N(const stan::model::model_base* model) {
    if (model == nullptr) {
      return "[ERROR] The model is uninitialized. Please use the 'load' command.\n";
    }
    return std::to_string(model->num_params_r());
  }

  std::string eval_load(std::istringstream& ss, stan::model::model_base** model,
			std::string& data_filename, const unsigned int seed) {
    std::string filename;
    ss >> filename;

    if (filename == "") {
      return "Please provide a filename\n";
    }

    if (*model != nullptr) {
      delete *model;
      *model = nullptr;
    }

    data_filename = filename;
    std::stringstream msg;
    try {
      std::fstream stream(data_filename, std::fstream::in);

      stan::io::dump var_context(stream);
      *model = &new_model(var_context, seed, &std::cout);
      msg << "[INFO] model initialized with data from \"" << data_filename << "\"\n";
    } catch (const std::exception& e) {
      msg << "[ERROR] model could not be initialized. See error message from Stan: "
	  << "\n\n"
	  << e.what() << "\n";
    }
    return msg.str();
  }

  std::string eval_unload(std::istringstream& ss, stan::model::model_base** model,
			  std::string& data_filename, const unsigned int seed) {
    
    if (*model != nullptr) {
      delete *model;
      *model = nullptr;
    }
    data_filename = "";
    return "[INFO] model uninitialized";
  }

  // precondition: model is not nullptr
  Eigen::Matrix<stan::math::var, -1, 1> parse_parameters(std::istringstream& ss,
							 stan::model::model_base* model) {
    const int N = model->num_params_r();
    Eigen::Matrix<stan::math::var, -1, 1> theta(N);
    std::stringstream msg;

    for (int n = 0; n < N; ++n) {
      if (!ss.good()) {
	stan::math::recover_memory();
	msg << "[ERROR] only " << n << " parameter values provided. Needs " << N << ".";
	throw std::invalid_argument(msg.str());
      }

      std::string substr;
      std::getline(ss, substr, ',');
      try {
	theta[n] = std::stod(substr);
      } catch (std::exception& e) {
	stan::math::recover_memory();
	msg << "[ERROR] issue parsing parameter value from string.\n"
	    << "\"" << e.what() << "\"";
	throw std::invalid_argument(msg.str());
      }
    }
    if (ss.good()) {
      stan::math::recover_memory();
      msg << "[ERROR] too many parameters provided. Expecting only " << N << " values.";
      throw std::invalid_argument(msg.str());
    }
    return theta;
  }


  // precondition: model is not nullptr
  Eigen::Matrix<double, -1, 1> parse_parameters_double(std::istringstream& ss,
						       stan::model::model_base* model) {
    const int N = model->num_params_r();
    Eigen::Matrix<double, -1, 1> theta(N);
    std::stringstream msg;

    for (int n = 0; n < N; ++n) {
      if (!ss.good()) {
	msg << "[ERROR] only " << n << " parameter values provided. Needs " << N << ".";
	throw std::invalid_argument(msg.str());
      }

      std::string substr;
      std::getline(ss, substr, ',');
      try {
	theta[n] = std::stod(substr);
      } catch (std::exception& e) {
	msg << "[ERROR] issue parsing parameter value from string.\n"
	    << "\"" << e.what() << "\"";
	throw std::invalid_argument(msg.str());
      }
    }
    if (ss.good()) {
      stan::math::recover_memory();
      msg << "[ERROR] too many parameters provided. Expecting only " << N << " values.";
      throw std::invalid_argument(msg.str());
    }
    return theta;
  }

  std::string trim(const std::string& s) {
    const std::string WHITESPACE = " \n\r\t\f\v";
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
  }

  std::string format_output(const double& value, const Eigen::VectorXd& gradient,
			    const std::chrono::steady_clock::time_point& start,
			    const std::chrono::steady_clock::time_point& end,
			    const std::string& message) {
    Eigen::IOFormat transpose_column_vector(Eigen::StreamPrecision, 0, " ", " ");
    std::stringstream msg;
    msg << value << "\n"
	<< gradient.format(transpose_column_vector) << "\n" 
	<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "\n"
	<< "\"" << trim(message) << "\"";
    return msg.str();
  }

  std::string format_output(const double& value,
			    const std::chrono::steady_clock::time_point& start,
			    const std::chrono::steady_clock::time_point& end,
			    const std::string& message) {
    std::stringstream msg;
    msg << value << "\n"
	<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "\n"
	<< "\"" << trim(message) << "\"";
    return msg.str();
  }

  std::string eval_eval_J_true(std::istringstream& ss, stan::model::model_base* model) {
    if (model == nullptr) {
      return "[ERROR] the model is uninitialized. Please use the 'load' command.\n";
    }

    Eigen::Matrix<stan::math::var, -1, 1> theta;
    try {
      theta = parse_parameters(ss, model);
    } catch (const std::invalid_argument& e) {
      return e.what();
    }

    std::stringstream msg;
    std::stringstream log_prob_message;
    try {
      std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
      stan::math::var lp = model->log_prob_propto_jacobian(theta, &log_prob_message);
      stan::math::grad(lp.vi_);
      Eigen::VectorXd gradient = theta.adj();
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      msg << format_output(lp.val(), gradient, start, end, log_prob_message.str());
    } catch (std::exception& e) {
      msg << "[ERROR] evaluating at the parameter throws exception\n"
	  << log_prob_message.str() << "\n"
	  << e.what();
    }
    stan::math::recover_memory();
    return msg.str();
  }

  std::string eval_eval(std::istringstream& ss, stan::model::model_base* model) {
    return eval_eval_J_true(ss, model);
  }

  std::string eval_eval_J_false(std::istringstream& ss, stan::model::model_base* model) {
    if (model == nullptr) {
      return "[ERROR] the model is uninitialized. Please use the 'load' command.\n";
    }

    Eigen::Matrix<stan::math::var, -1, 1> theta;
    try {
      theta = parse_parameters(ss, model);
    } catch (const std::invalid_argument& e) {
      return e.what();
    }

    std::stringstream log_prob_message;
    std::stringstream msg;
    try {
      std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
      stan::math::var lp = model->log_prob_propto(theta, &log_prob_message);
      stan::math::grad(lp.vi_);
      Eigen::VectorXd gradient = theta.adj();
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

      msg << format_output(lp.val(), gradient, start, end, log_prob_message.str());
    } catch (std::exception& e) {
      msg << "[ERROR] evaluating at the parameter throws exception\n"
	  << log_prob_message.str() << "\n"
	  << e.what();
    }
    stan::math::recover_memory();
    return msg.str();
  }

  std::string eval_eval_J_only(std::istringstream& ss, stan::model::model_base* model) {
    if (model == nullptr) {
      return "[ERROR] the model is uninitialized. Please use the 'load' command.\n";
    }

    Eigen::Matrix<double, -1, 1> theta;
    try {
      theta = parse_parameters_double(ss, model);
    } catch (const std::invalid_argument& e) {
      return e.what();
    }

    std::stringstream log_prob_message;
    std::stringstream msg;
    try {
      std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
      double lp_jacobian_true = model->log_prob_propto_jacobian(theta, &log_prob_message);
      log_prob_message.clear();
      double lp_jacobian_false = model->log_prob_propto(theta, &log_prob_message);
      double jacobian = lp_jacobian_true - lp_jacobian_false;
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

      msg << format_output(jacobian, start, end, log_prob_message.str());
    } catch (std::exception& e) {
      msg << "[ERROR] evaluating at the parameter throws exception\n"
	  << log_prob_message.str() << "\n"
	  << e.what();
    }
    return msg.str();
  }


  std::string eval(std::string& line, const int count, const std::deque<std::string>& history,
		   std::string& data_filename,
		   stan::model::model_base** model, const time_point_t& start_time,
		   const unsigned int seed) {
    std::string command;
    std::istringstream ss(line);
    std::stringstream message;
    ss >> command;

    if (command == "list") {
      return eval_list();
    } else if (command == "help") {
      return eval_help();
    } else if (command == "status") {
      return eval_status(*model, count, start_time, data_filename);
    } else if (command == "N") {
      return eval_N(*model);
    } else if (command == "load") {
      return eval_load(ss, model, data_filename, seed);
    } else if (command == "unload") {
      return eval_unload(ss, model, data_filename, seed);
    } else if (command == "eval") {
      return eval_eval(ss, *model);
    } else if (command == "eval_J_true") {
      return eval_eval_J_true(ss, *model);
    } else if (command == "eval_J_false") {
      return eval_eval_J_false(ss, *model);
    } else if (command == "eval_J_only") {
      return eval_eval_J_only(ss, *model);
      /*
      // FIXME: implement constrain and unconstrain
      } else if (command == "constrain") {
      return "FIXME constrain";
      } else if (command == "unconstrain") {
      return "FIXME unconstrain";
      */
    } else if (command == "history") {
      return eval_history(count, history);
      /*
      // TODO: implement clear
      } else if (command == "clear") {
      return "FIXME clear";
      */
    } else if (command == "quit") {
      exit(0);
    }

    message << "[ERROR] unknown command \"" << command << "\"";
    return message.str();
  }
}
#endif
