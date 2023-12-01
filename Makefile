MAKES=$(shell echo [0-9]*/Makefile )
.DEFAULT_GOAL = all

%:
	for i in $(MAKES); do make -C $$(dirname $$i) $@; done
