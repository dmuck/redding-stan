help: ## First target is the default make target


PHONY: help install uninstall clean check

STAN_VERSION = v2.29.1
STANCFLAGS ?= --warn-pedantic

CXXFLAGS = -I src
CXXFLAGS += -isystem stan/src -isystem stan/lib/stan_math
CXXFLAGS += -isystem stan/lib/stan_math/lib/eigen_3.3.9 -isystem stan/lib/stan_math/lib/boost_1.75.0
CXXFLAGS += -isystem stan/lib/stan_math/lib/sundials_6.0.0/include -isystem stan/lib/stan_math/lib/tbb_2020.3/include
CXXFLAGS += -std=c++14
CXXFLAGS += -D_REENTRANT
CXXFLAGS += -O3

MATH := stan/lib/stan_math

OS ?= $(shell uname -s)
OS_TAG ?= $(strip $(if $(filter Darwin,$(OS)), mac, linux))
LIBRARY_SUFFIX = $(if $(filter linux,$(OS_TAG)),so.2,dylib)
TBB_LIBRARIES = $(if $(filter linux,$(OS_TAG)),tbb,tbb tbbmalloc tbbmalloc_proxy)
TBB_LIBRARIES := $(TBB_LIBRARIES:%=$(MATH)/lib/tbb/lib%.$(LIBRARY_SUFFIX))

LDLIBS = src/main.o $(TBB_LIBRARIES) -Wl,-L,$(MATH)/lib/tbb -Wl,-rpath,$(MATH)/lib/tbb
LINK.o = $(LINK.cpp)

ifeq (,$(filter $(MAKECMDGOALS),help install uninstall clean check))
-include src/main.d
endif

help:
	@echo
	@echo 'ReddingStan is a command-line program that smuggles log'
	@echo 'probabilities and gradients out of a Stan program.'
	@echo
	@echo
	@echo '*** Installation ***'
	@echo
	@echo '1. Check installation by running: '
	@echo '   > make check'
	@echo 
	@echo '2. Install by running: '
	@echo '   > make install'
	@echo
	@echo '   Note: to build using multiple cores, use the -j '
	@echo '   option to make, e.g. for 4 cores: '
	@echo
	@echo '   > make install -j4'
	@echo
	@echo
	@echo '*** Building a ReddingStan executable ***'
	@echo
	@echo '  Given a Stan program at path/program.stan, build an executable by typing'
	@echo '  (remove the .stan from the full path):'
	@echo
	@echo '  > make path/program'
	@echo
	@echo
	@echo '*** Running the ReddingStan executable ***'
	@echo
	@echo '  Run the executable generated. There is help within the program itself:'
	@echo
	@echo '  > ./path/program'
	@echo

install: bin/stanc stan/src/stan/version.hpp $(MATH)/stan/math/version.hpp src/main.o
install: $(TBB_LIBRARIES)
install:
	@echo
	@echo 'Installation complete'

uninstall:
	$(RM) bin/stanc src/main.o src/main.d
	$(MAKE) -C $(MATH) clean-libraries
	@echo
	@echo 'uninstall'

clean:
	@echo
	@echo 'clean'

check:
	@ n=0; \
	test -f bin/stanc || { echo '[ERROR] bin/stanc does not exist'; ((n++)); false; } && echo '* bin/stanc ok' ; \
	test -f stan/src/stan/version.hpp || { echo '[ERROR] Stan submodule needs to be updated'; ((n++)); false; } && echo '* Stan submodule ok' ; \
	test -f stan/lib/stan_math/stan/math/version.hpp || { echo '[ERROR] Stan Math submodule needs to be updated'; ((n++)); false; } && echo '* Stan Math submodule ok' ; \
	if [ $$n -gt 0 ]; then echo; echo 'Please run `make install`'; echo; exit 1; fi
	@echo
	@echo 'Check: All checks pass'



bin/stanc: URL = github.com/stan-dev/stanc3/releases/download
bin/stanc:
	@echo '* downloading stanc $(STAN_VERSION) for $(OS_TAG)'
	@echo ''
	@mkdir -p $(dir $@)
	@curl -L https://$(URL)/$(STAN_VERSION)/$(OS_TAG)-stanc -o $@
	@chmod +x $@


stan/src/stan/version.hpp:
	git submodule update --init --depth 1

stan/lib/stan_math/stan/math/version.hpp: stan/src/stan/version.hpp
	cd stan && git submodule update --init --depth 1

%.cpp: %.stan bin/stanc src/main.o
	bin/stanc $< --o $@

%.d: %.cpp
	@set -e; rm -f $@; \
         $(CXX) -M -MT "$*.o $@" $(CXXFLAGS) $< > $@

$(MATH)/lib/tbb/lib%: LIB = $(patsubst $(MATH)/%,%,$@)
$(MATH)/lib/tbb/lib%: $(MATH)/stan/math/version.hpp
	$(MAKE) -C $(MATH) $(LIB)

