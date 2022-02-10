PHONY: help all install uninstall clean check

STAN_VERSION=v2.29.0-rc2

help:
	@echo '--------------------------------------------------------------------------------'
	@echo '--------------------------------------------------------------------------------'

all:
	@echo 'all'

install: bin/stanc
	@echo 'install'

uninstall:
	@echo 'uninstall'

clean:
	@echo 'clean'

check:
	@echo 'check'



bin/stanc: OS ?= $(shell uname -s)
bin/stanc: OS_TAG = $(strip $(if $(filter Darwin,$(OS)), mac, linux))
bin/stanc: URL = github.com/stan-dev/stanc3/releases/download
bin/stanc:
	@mkdir -p $(dir $@)
	curl -L https://$(URL)/$(STAN_VERSION)/$(OS_TAG)-stanc -o $@
	chmod +x $@
