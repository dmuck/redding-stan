# Walking through `main.cpp`

This document walks through a prototype of `Redding Stan`; the source file (temporarily) lives [here](https://github.com/dmuck/redding-stan/blob/issue-5-main/src/main.cpp) on the `issue-5-main` branch.

## 1. Include

```c++
#include <stan/model/model_header.hpp>
#include <stan/model/model_base.hpp>
#include <stan/io/dump.hpp>
```

The first header file contains a bunch of include statements, and it lives [here](https://github.com/stan-dev/stan/blob/9f7361a87d575ae5977bd84fc8614b1e9eb60e81/src/stan/model/model_header.hpp); whereas the last file reads data from an `RDump` format for the executable, which is [explained](https://mc-stan.org/docs/2_29/cmdstan-guide/rdump.html) in the `CmdStan` documentation. The second, [`model_base.hpp`](https://github.com/stan-dev/stan/blob/9f7361a87d575ae5977bd84fc8614b1e9eb60e81/src/stan/model/model_base.hpp), defines a number of methods for Markov chain Monte Carlo sampling and other Stan services (e.g., optimization, variational inference). It is in this file that we find the `log_prob` functionality, among other things, that is of primary interest for `redding-stan`. 

Next, we see a forward declaration that refers to the `model_base` class

```c++
stan::model::model_base &new_model(stan::io::var_context &data_context,
                                   unsigned int seed, std::ostream *msg_stream);
```

which we need to instantiate the Stan model with the data. The data are assembled into proper format using the `stan::io::var_context` [class](https://github.com/stan-dev/stan/blob/develop/src/stan/io/var_context.hpp). The last argument, `*msg_stream`, is a pointer that allows for the model's details to be printed to the console. 

## 2. Inside Main

Now we're inside the program. First, we output a couple lines to the console, one of which displays the number of arguments our program takes. One of those arguments is the data file, which in this particular case was `bernoulli.data.R`, the one we called from the command line. The name of that file gets printed to the screen with 

```c++
if (argc > 1) {
    std::cout << "data file = " << argv[1] << std::endl;
    stream = std::fstream(argv[1], std::fstream::in);
  }
stan::io::dump dump(stream);
```

after which we call the `dump()` [constructor](https://github.com/stan-dev/stan/blob/develop/src/stan/io/dump.hpp#L627) in the following line to transform the data in a format suitable for a Stan model. After that, we have what we need to instantiate the model

```C++
stan::model::model_base &model = new_model(dump, 0L, &std::cout);
std::cout << "Model has been instantiated" << std::endl;
```

## 2.5. Stepping into  the `model_base` class

Before we get to what the model gives us back, let's look at some of the functionality that was briefly mentioned above regarding the `model_base.hpp` header file. 

*NB: where is `new_model()` function? 

The `model_base` constructor instantiates a model object with a set number of unconstrained, real-valued parameters. This class comes with a number of pure virtual functions, some of which are called in the following lines in our `main.cpp` program

```c++
std::string model_name = model.model_name();
std::vector<std::string> model_compile_info = model.model_compile_info();
size_t num_params_r = model.num_params_r();
size_t num_params_i = model.num_params_i();
```

The `model.model_name()` call is self-explanatory. The following line calls the function `model_compile_info()`, which returns a vector containing names of the parameters in the parameters, transformed parameters, and generated quantities blocks. The last two lines are the number of unconstrained (`_r` denotes "real-valued") and constrained parameters (`_i` denotes integers), which are called using [functions](https://github.com/stan-dev/stan/blob/develop/src/stan/model/prob_grad.hpp#L57) defined in the base class, [`prob_grad`](https://github.com/stan-dev/stan/blob/9f7361a87d575ae5977bd84fc8614b1e9eb60e81/src/stan/model/model_base.hpp#L33). 

The `model_base` class also contains functions that return the log density for the unconstrained parameters, with and without Jacobian corrections for constraints and normalizing constants. These are what we'll call to smuggle out our gradients and log probability density for the parameters. 

## 3. Getting Log-Prob and Gradients

Finally, we've come to the point where we can collect and output our quantities of interest. We start with generating random starting values for each of our parameters and printing them to the console

```c++
Eigen::Matrix<stan::math::var, -1, 1> parameters = Eigen::MatrixXd::Random(num_params_r, 1);
std::cout << " * parameters = " << parameters << std::endl;
```

The first line uses the `Eigen::Matrix` class, where the first template parameter is a scalar type defined in the [`var.hpp`](https://github.com/stan-dev/math/blob/develop/stan/math/rev/core/var.hpp#L1079) file in the Stan math library. The last template parameter indicates the number of columns. 

****NB: what does -1 in second template parameter indicate? Something to do with `Eigen::Dynamic`?

The `Eigen::MatrixXd::Random()` creates a matrix of random values with dimensions equal to `(num_params_r, 1)`, which is just a column vector of random initialization points for each of the unconstrained parameters in our model. 

Now, to get the log probability density and gradients, we first call the `log_prob_propto_jacobian()` [function](https://github.com/stan-dev/stan/blob/9f7361a87d575ae5977bd84fc8614b1e9eb60e81/src/stan/model/model_base.hpp#L510) from the `model_base` class and assign the return value to `lp`, which is of type `math::var`, as described above.

```c++
stan::math::var lp = model.log_prob_propto_jacobian(parameters, &msg);
```

 We can then extract the `log_prob` from our `lp` object and compute the associated gradients.

```c++
double lp_val = lp.val();
stan::math::grad(lp.vi_);
Eigen::VectorXd gradient = parameters.adj();
```

The variable `lp_val` contains the log probability density of our parameters; the `stan::math::grad(lp.vi_)` computes the gradient, which is defined in the [`grad.hpp` file](https://github.com/stan-dev/math/blob/develop/stan/math/rev/core/grad.hpp#L26); and the last line returns the adjoint and stores it in the vector `gradient`. 

Lastly, we print out our smuggled goods to the console.

```C++
std::cout << " * log prob propto jacobian of parameters: " << lp_val << std::endl;
std::cout << " * gradient! " << gradient << std::endl;
std::cout << " * message while evaluating log prob: \"" << msg.str() << "\"" << std::endl;
```

The full repository, which contains build instructions, can be accessed [here](https://github.com/dmuck/redding-stan).

---------

*Last updated: Feb. 16, 2022 @ 2:29pm EST*

