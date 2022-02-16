#include <iostream>
#include <fstream>
#include <stan/model/model_header.hpp>
#include <stan/model/model_base.hpp>
#include <stan/io/dump.hpp>

// forward declaration for function defined in another translation unit
stan::model::model_base &new_model(stan::io::var_context &data_context,
                                   unsigned int seed, std::ostream *msg_stream);

int main(int argc, char* argv[]) {
  std::cout << "inside main" << std::endl << std::endl;

  std::cout << "number of arguments: " << argc << std::endl;


  std::fstream stream;
  if (argc > 1) {
    std::cout << "data file = " << argv[1] << std::endl;
    stream = std::fstream(argv[1], std::fstream::in);
  }
  stan::io::dump dump(stream);

  // INSTANTIATED MODEL!!!
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
  std::cout << " * gradient! " << gradient << std::endl;
  std::cout << " * message while evaluating log prob: \"" << msg.str() << "\"" << std::endl;

  
  return 0;
}
