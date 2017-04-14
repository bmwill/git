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
#CFLAGS = -g -Wall -O2

# Test Makefile options
#GIT_TEST_OPTS = --verbose --debug
GIT_PROVE_OPTS= --timer -j$(CORES)

# Build tags by default
OTHER_PROGRAMS += .mytags
.mytags: FORCE
	@$(RM) tags
	@$(FIND_SOURCE_FILES) | xargs ctags -aw --fields=+l
