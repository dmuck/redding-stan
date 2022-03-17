# Using ReddingStan from R and Python

This page demonstrates the use of ReddingStan from R and Python. 

## R

To run `ReddingStan` from an R session, ensure that the library [`processx`](https://processx.r-lib.org/) is installed. Here's the same example from the [ReddingStan homepage](../README.md):

```R
library(processx)

filename <- './bernoulli/bernoulli'
redding <- processx::process$new(filename, stdout = "|", stdin = "|", stderr = "|")
redding$read_output_lines()

redding$write_input("load ./bernoulli/bernoulli.data.R\n")
redding$read_output_lines()
redding$write_input("eval 0.2\n")
redding$read_output()
```

It is also possible to utilize RStudio's built-in terminal using the [`rstudioapi`](https://rstudio.github.io/rstudio-extensions/rstudioapi.html) library:

```R
# runs the executable and opens a new terminal
redding <- rstudioapi::terminalExecute("./bernoulli/bernoulli")

# check to see if the model has been instantiated
rstudioapi::terminalSend(redding, "status\n")

# instantiate model with bernoulli data
rstudioapi::terminalSend(redding, "load ./bernoulli/bernoulli.data.R\n")

# clear buffer
rstudioapi::terminalClear(redding)

# evaluate model at parameter value 0.2
rstudioapi::terminalSend(redding, "eval 0.2\n") 

# store the results 
eval02 <- rstudioapi::terminalBuffer(redding)[1:4]
eval02

# end the process
rstudioapi::terminalKill(redding)

```





## Python

Use the [`subprocess`](https://docs.python.org/3/library/subprocess.html) module to run `ReddingStan` from Python.
