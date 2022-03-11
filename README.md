# ReddingStan

ReddingStan is a command-line program that smuggles log probabilities and gradients out of a [Stan](https://mc-stan.org/) model. It utilizes Stan's [math library](https://arxiv.org/abs/1509.07164) in a way that facilitates algorithmic prototyping and the development of post-processing software. 

# Why Redding?

Previously the log probabilities, gradients, and Jacobians for parameters were difficult to extract from a Stan program. Users interested in these quantities have had to rely on methods developed for the [RStan](https://mc-stan.org/users/interfaces/rstan.html) and [PyStan](https://mc-stan.org/users/interfaces/pystan.html) interaces. 

ReddingStan offers a solution by providing direct access to these quantities from the command-line. In that sense, ReddingStan is a smuggling operation---not unlike those run by the character played by Morgan Freeman, [Ellis Boyd "Red" Redding](https://shawshank.fandom.com/wiki/Ellis_Boyd_%27Red%27_Redding), in the classic film *The Shawshank Redemption*. We opted for "Redding" instead of "Red" in part because we have no indication that [Stanislaw Ulam](https://en.wikipedia.org/wiki/Stanislaw_Ulam), the namesake of the Stan modeling language, was in fact a Communist.

# Installation

1. Clone the repository
2. Run `make check` and, if prompted, run `make install`
3. Execute `bin/stanc --version`. This should show something like `stanc3 v2.29.0-rc2 (Unix)`
4. Run `make check` and it should succeed 

# Running ReddingStan

### From the command line

To demonstrate the basic functionality, use the example in the [CmdStan](https://github.com/stan-dev/cmdstan/tree/develop/examples/bernoulli) repository:
1. Ensure that the `bernoulli.stan` and `bernoulli.data.R` files are in the current working directory.
2. Run `make bernoulli` to build the executable.
3. After compiliation, run the executable in the terminal `./bernoulli`.
4. If successful, the user will see the startup message and the prompt, `[redding]$ `; if unsuccessful, the user will see an error message indicating that the program could not start.
5. Inside the program, one can enter `list` to see a list of commands printed to the console: `[redding]$ list`.
6. To obtain the quantities of interest, initialize the model by loading in the data: `[redding]$ load bernoulli.data.R`. The program will return a message indicating success or failure. 
7. Once the model has been initialized, the user can obtain the log probability density and gradient at a given parameter value: `[redding]$ eval 0.2`. The output will print the (1) log probability, (2) the gradient, and (3) the execution time, respectively, line by line. 
8. To exit the program, simply enter quit: `[redding]$ quit`.
