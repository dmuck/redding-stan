# helpers.R

count_unconstrained_params <- function() { 
  # call process and return number of parameters;
  # assume here that the process is named `redding`
  
  redding$write_input("N \n")   
  redding$poll_io(2000) 
  n_param <- redding$read_output()
  N <- strsplit(n_param, split="\n")[[1]][1]
  
  return(as.numeric(N))
}

get_gradient <- function(val, command = "eval_J_true") {
  # call process and return the gradient at a given parameter value
  
  redding$write_input(paste(command, paste0(val, "\n")))
  redding$poll_io(5000)
  out <- redding$read_output()
  grad <- strsplit(out, split="\n")[[1]][2]
  
  return(as.numeric(grad))
}

extract_redding_quantities <- function(val, command = "eval_J_true") {
  # call process and return log prob, gradient, and execution time
  
  redding$write_input(paste(command, paste0(val, "\n")))
  redding$poll_io(5000)
  out <- redding$read_output()
  olist <- strsplit(out, split="\n")[[1]]
  
  # assume no message to return from Stan
  quantities <- olist[1:(length(olist)-2)] 
  
  return(as.numeric(quantities))
}

parse_redding_eval <- function(val, command = "eval_J_true") {
  # call process and return character vector for a given eval command
  
  redding$write_input(paste(command, paste0(val, "\n")))
  redding$poll_io(5000)
  out <- redding$read_output()
  ochar <- strsplit(out, split="\n")[[1]]
  
  return(ochar)
}


is_model_initialized <- function() {
  # call process and return status
  
  redding$write_input("status\n")
  redding$poll_io(2000)
  lst <- strsplit(redding$read_output(), split="\n")[[1]]
  return(any(grepl("The model is initialized", lst)))
}

