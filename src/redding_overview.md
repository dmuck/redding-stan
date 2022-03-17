# ReddingStan: A Technical Overview

This page provides an overview of the `ReddingStan` for those interested in the implementation details and the relevant functions in the [Stan Math](https://mc-stan.org/math/) library.  

`ReddingStan` smuggles log probabilities and gradients out of a Stan model by accessing the quantities directly from functions in the Stan Math library.  One can find the relevant functions in the `src/redding/helper.hpp` header file; the main source file is `src/main.cpp`. 

There are three primary evaluation functions. The first, `eval_J_true`, is the default. When this command is invoked in ReddingStan, the underlying function returns the log probability density and gradient(s) [with the Jacobian adjustment](https://github.com/stan-dev/stan/blob/develop/src/stan/model/log_prob_propto.hpp), for a given parameter value. The second, `eval_J_false`, returns those same quantities without a Jacobian adjustment. And the third, `eval_J_only`, returns the [log absolute determinant of the Jacobian](https://mc-stan.org/docs/2_29/reference-manual/change-of-variables.html).  

 
