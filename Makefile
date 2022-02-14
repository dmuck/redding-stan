help: ## First target is the default make target


PHONY: help all install uninstall clean check

STAN_VERSION = v2.29.0-rc2
STANCFLAGS ?= --warn-pedantic

CXXFLAGS = -I stan/src -I stan/lib/stan_math
CXXFLAGS += -I stan/lib/stan_math/lib/eigen_3.3.9 -I stan/lib/stan_math/lib/boost_1.75.0
CXXFLAGS += -I stan/lib/stan_math/lib/sundials_6.0.0/include -I stan/lib/stan_math/lib/tbb_2020.3/include
CXXFLAGS += -std=c++14
CXXFLAGS += -D_REENTRANT
CXXFLAGS += -O3


MATH := stan/lib/stan_math

OS ?= $(shell uname -s)
OS_TAG ?= $(strip $(if $(filter Darwin,$(OS)), mac, linux))
TBB_LIBRARIES = $(if $(filter linux,$(OS_TAG)),tbb,tbb tbbmalloc tbbmalloc_proxy)
TBB_LIBRARIES := $(TBB_LIBRARIES:%=$(MATH)/lib/tbb/lib%.dylib)

LDLIBS = src/main.o $(TBB_LIBRARIES) -Wl,-L,$(MATH)/lib/tbb -Wl,-rpath,$(MATH)/lib/tbb
LINK.o = $(LINK.cpp)

help:
	@echo '--------------------------------------------------------------------------------'
	@echo '--------------------------------------------------------------------------------'


all:
	@echo 'all'

#ifeq ($(OS),Darwin)
#  TBB_LIBRARIES ?= tbb tbbmalloc tbbmalloc_proxy
#else
#  TBB_LIBRARIES ?= tbb
#endif

install: bin/stanc stan/src/stan/version.hpp $(MATH)/stan/math/version.hpp src/main.o
install: $(TBB_LIBRARIES)
install:
	@echo
	@echo 'Installation complete'

uninstall:
	$(RM) bin/stanc
	$(MAKE) -C $(MATH) clean-libraries
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

$(MATH)/lib/tbb/lib%.dylib: LIB = $(patsubst $(MATH)/%,%,$@)
$(MATH)/lib/tbb/lib%.dylib: $(MATH)/stan/math/version.hpp
	$(MAKE) -C $(MATH) $(LIB)

