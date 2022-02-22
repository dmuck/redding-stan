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

As a simple example: 
1. Copy the `bernoulli.stan` and `bernoulli.data.R` files from the CmdStan example folder into the current working directory. Also en
2. Run `make bernoulli && ./bernoulli bernoulli.data.R`
3. The program should execute and print out the quantities to the console
