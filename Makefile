PHONY: help all install uninstall clean check

STAN_VERSION=v2.29.0-rc2

help:
	@echo '--------------------------------------------------------------------------------'
	@echo '--------------------------------------------------------------------------------'

all:
	@echo 'all'

install: bin/stanc stan/src/stan/version.hpp stan/lib/stan_math/stan/math/version.hpp
	@echo
	@echo 'Installation complete' 

uninstall:
	@echo 'uninstall'

clean:
	$(RM) bin/stanc

check:
	@echo 'check'


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
