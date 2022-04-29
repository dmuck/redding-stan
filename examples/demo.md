# Using ReddingStan in R and Python

This page provides information on using ReddingStan inside R and Python environments. Extended examples can be found in the [`bernoulli`](./bernoulli) and [`eight-schools`](./eight-schools) sub-folders. 

## R

For a set of helper functions that assist with using ReddingStan in R, make sure to source the following script into the R environment [`src/helpers.R`](../src/helpers.R).

### The `processx` library

To run `ReddingStan` from an R session, ensure that the library [`processx`](https://processx.r-lib.org/) is installed. Here's the same example from the [ReddingStan homepage](../README.md):

```R
library(processx)

filename <- './bernoulli/bernoulli'
redding <- processx::process$new(filename, stdout = "|", stdin = "|", stderr = "|")
redding$read_output()

redding$write_input("load ./bernoulli/bernoulli.data.R\n")
redding$read_output()
redding$write_input("eval 0.2\n")
redding$read_output()
```

### The `rstudioapi` library

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

# end the process
rstudioapi::terminalKill(redding)

```



## Python

To use ReddingStan in a Python environment, it is recommended to use the [`subprocess`](https://docs.python.org/3/library/subprocess.html) module.

