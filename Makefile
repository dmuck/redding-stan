help: ## First target is the default make target


PHONY: help all install uninstall clean check

STAN_VERSION = v2.29.0-rc2
STANCFLAGS ?= --warn-pedantic

CXXFLAGS = -I stan/src -I stan/lib/stan_math
CXXFLAGS += -I stan/lib/stan_math/lib/eigen_3.3.9 -I stan/lib/stan_math/lib/boost_1.75.0
CXXFLAGS += -I stan/lib/stan_math/lib/sundials_6.0.0/include -I stan/lib/stan_math/lib/tbb_2020.3/include
CXXFLAGS += -std=c++14
CXXFLAGS += -D_REENTRANT

help:
	@echo '--------------------------------------------------------------------------------'
	@echo '--------------------------------------------------------------------------------'


all:
	@echo 'all'

install: bin/stanc stan/src/stan/version.hpp stan/lib/stan_math/stan/math/version.hpp
	@echo
	@echo 'Installation complete' 

uninstall:
	$(RM) bin/stanc
	@echo
	@echo 'uninstall'

clean:
	@echo
	@echo 'clean'

check:
	@ n=0; \
	test -f bin/stanc || { echo 'Error: bin/stanc does not exist'; ((n++)); } ; \
	test -f stan/src/stan/version.hpp || { echo 'Error: Stan submodule needs to be updated'; ((n++)); }; \
	test -f stan/lib/stan_math/stan/math/version.hpp || { echo 'Error: Stan Math submodule needs to be updated'; ((n++)); }; \
	if [ $$n -gt 0 ]; then echo; echo 'Please run `make install`'; echo; exit 1; fi
	@echo
	@echo 'Check: All checks pass'



bin/stanc: OS ?= $(shell uname -s)
bin/stanc: OS_TAG = $(strip $(if $(filter Darwin,$(OS)), mac, linux))
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

%.cpp: %.stan bin/stanc
	bin/stanc $< --o $@


