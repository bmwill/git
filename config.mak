# Build using theads based on the number of cores on the machine
ifeq ($(shell uname), Linux)
	CORES=$(shell grep -c processor /proc/cpuinfo)
else
	CORES=$(shell sysctl -n hw.logicalcpu)
endif
MAKEFLAGS += -j$(CORES)

DEVELOPER=1 # turn on developer build settings
NO_TCLTK=1  # turn off building of the GUI
#NO_PTHREADS=1
CFLAGS += -O0

# Test Makefile options
#GIT_TEST_OPTS = --verbose --debug
GIT_PROVE_OPTS= --timer --shuffle -j$(CORES)
override T = $(sort $(wildcard t[0-8][0-9][0-9][0-9]-*.sh))

# Build tags by default
OTHER_PROGRAMS += .mytags
.mytags: FORCE
	@$(RM) tags
	@$(FIND_SOURCE_FILES) | xargs ctags -aw --fields=+l

#style-commit: FORCE
.style: FORCE
	@if [ "`git diff`" ]; \
	then \
		git clang-format --style file --diff --extensions c,h; \
	else \
		git clang-format --style file --diff --extensions c,h HEAD^ HEAD; \
	fi

.style-commit-apply: FORCE
	git clang-format --style file --extensions c,h HEAD^
