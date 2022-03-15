# ReddingStan

ReddingStan is a command-line program that smuggles log probabilities and gradients out of a [Stan](https://mc-stan.org/) model. It utilizes Stan's [Math](https://arxiv.org/abs/1509.07164) library in a way that facilitates algorithmic prototyping and the development of post-processing software. 

# Why Redding?  
Previously the log probabilities, gradients, and Jacobian adjustments for parameters were difficult to extract from a Stan program despite their fundamental role in evaluating probabilistic models. Users interested in these quantities have had to rely on methods developed for the [RStan](https://mc-stan.org/users/interfaces/rstan.html) and [PyStan](https://mc-stan.org/users/interfaces/pystan.html) interaces.  

ReddingStan offers a solution by providing direct access to these quantities from the command-line. In that sense, ReddingStan is a smuggling operation&mdash;not unlike those run by the character played by Morgan Freeman, [Ellis Boyd "Red" Redding](https://shawshank.fandom.com/wiki/Ellis_Boyd_%27Red%27_Redding), in the classic film *The Shawshank Redemption*. We opted for "Redding" instead of "Red" in part because we have no indication that [Stanislaw Ulam](https://en.wikipedia.org/wiki/Stanislaw_Ulam), the namesake of the Stan modeling language, was in fact a Communist.

# Installation

1. Clone the repository.
2. Run `make check` and, if prompted, run `make install`.
3. Execute `bin/stanc --version`. This should show something like `stanc3 v2.29.0-rc2 (Unix)`.
4. Run `make check` to verify that installation was successful. 

# Running ReddingStan

After installation it is straightforward to run ReddingStan. Place the Stan file in the current working directory (or set its path) and run `make MODEL_NAME`, where `MODEL_NAME` is the name of the Stan file preceding the `.stan` extension. The program will compile and build an executable. Once built, the exectuable will be located in the current working directory. The user can run the exectuable with the command `./MODEL_NAME`, which will start ReddingStan. 

Inside of a ReddingStan program, the commands `list` and `help` provide information about the available options and basic functionality. In general, the following steps yield the quantities of interest: 
1. Load data (if necessary) to initialize the model with the `load` command followed by the name of the data file.  
2. Evaluate the model by passing unconstrained parameter values, separated by commas; the program will return the log probability and gradient(s). Users pass the parameter values to one of the three commands, depending on the information they're seeking from the Stan model:
	- `eval_J_true`: computes the log probability density and gradient(s) with a Jacobian adjustment; the returned quantities include the log absolute determinant of the Jacobian. This is the default command and can be called with `eval`.
	- `eval_J_false`: computes the log probability density without the Jacobian adjustment; only the log probability and gradient(s) are returned.
	- `eval_J_only`: computes the log absolute determinant of the Jacobian and returns that value only.
	- `eval`: calls `eval_J_true` by default.
3. The model is evaluated and the output is returned to the console; the output is meant to be parsed from a program: 
	- Line 1: the requested quantity; a scalar value 
	- Line 2: the gradient as a comma-separated string; not applicable for `eval_J_only`
	- Line 3: the evaluation time, in microseconds
	- Line 4+: messages from the Stan program; a string

For more information about how these quantities are evaluated and produced in the underyling C++ code, see the [Redding-Rules](FIXME.link) page for an overview of the [helper.hpp](FIXME.link) file and references to the Stan Math codebase.

See the [Examples](FIXME.link) page for more information on using ReddingStan.

## Using ReddingStan from the command line  

To demonstrate the basic functionality, use the example in the [CmdStan](https://github.com/stan-dev/cmdstan/tree/develop/examples/bernoulli) repository:
1. Ensure that the `bernoulli.stan` and `bernoulli.data.R` files are in the current working directory.
2. Run `make bernoulli` to build the executable.
3. After compiliation, run the executable in the terminal `./bernoulli`.
4. If successful, the user will see the startup message and the prompt, `[redding]$ `; if unsuccessful, the user will see an error message indicating that the program could not start.
5. Inside the program, one can enter `list` to see a list of commands printed to the console: `[redding]$ list`.
6. To obtain the quantities of interest, initialize the model by loading in the data: `[redding]$ load bernoulli.data.R`. The program will return a message indicating success or failure. 
7. Once the model has been initialized, the user can obtain the log probability density and gradient at a given parameter value: `[redding]$ eval 0.2`. The output will print the (1) log probability, (2) the gradient, and (3) the execution time, respectively, line by line. 
8. To exit the program, simply enter quit: `[redding]$ quit`.

# Contributing

To suggest new features, report bugs, or add examples to the documentation, please open a [new issue](https://github.com/dmuck/redding-stan/issues). For those interested in contributing to ReddingStan, please refer to the process described in Stan's [contributing guide](https://github.com/stan-dev/stan/wiki) before submitting a pull request. We welcome contributions of any kind. 


