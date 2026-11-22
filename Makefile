.DEFAULT_GOAL := all
CC = cc
CXX = c++
PYTHON ?= python3
BUILD_DIR ?= build
CPPFLAGS += -Iinclude -D_POSIX_C_SOURCE=200809L
CFLAGS += -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Werror -pthread
CXXFLAGS += -std=c++20 -O2 -g -Wall -Wextra -Wpedantic -Werror -pthread
LDLIBS += -pthread

ifneq ($(SANITIZE),)
CFLAGS += -O1 -fsanitize=$(SANITIZE) -fno-omit-frame-pointer
CXXFLAGS += -O1 -fsanitize=$(SANITIZE) -fno-omit-frame-pointer
LDFLAGS += -fsanitize=$(SANITIZE)
endif

PROGRAMS := record-converter record-access pthread-matrix thread-tree file-sort \
            future-pipeline semaphore-sequencing pipe-echo signal-handshake \
            process-tree template-containers
TARGETS := $(addprefix $(BUILD_DIR)/,$(PROGRAMS))
C_HEADERS := $(wildcard include/*.h)
CXX_HEADERS := $(wildcard include/*.hpp)
CONTAINERS := exercises/cpp/template-containers

.PHONY: all check
all: $(TARGETS)

$(BUILD_DIR):
	mkdir -p "$@"

define c_program
$(BUILD_DIR)/$(1): $(2) $(C_HEADERS) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(2) $(LDFLAGS) $(LDLIBS) -o "$$@"
endef

define cpp_program
$(BUILD_DIR)/$(1): $(2) $(3) $(CXX_HEADERS) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(2) $(LDFLAGS) $(LDLIBS) -o "$$@"
endef

$(eval $(call c_program,record-converter,exercises/io/record-converter/main.c))
$(eval $(call c_program,record-access,exercises/io/record-access/main.c))
$(eval $(call c_program,pthread-matrix,exercises/concurrency/pthread-matrix/main.c))
$(eval $(call c_program,thread-tree,exercises/concurrency/thread-tree/main.c))
$(eval $(call c_program,pipe-echo,exercises/ipc/pipe-echo/main.c))
$(eval $(call c_program,signal-handshake,exercises/processes/signal-handshake/main.c))
$(eval $(call c_program,process-tree,exercises/processes/process-tree/main.c))
$(eval $(call cpp_program,file-sort,exercises/concurrency/file-sort/main.cpp))
$(eval $(call cpp_program,future-pipeline,exercises/concurrency/future-pipeline/main.cpp))
$(eval $(call cpp_program,semaphore-sequencing,exercises/concurrency/semaphore-sequencing/main.cpp))
$(eval $(call cpp_program,template-containers,$(CONTAINERS)/main.cpp,$(wildcard $(CONTAINERS)/*.h)))

$(BUILD_DIR)/test-containers: tests/containers.cpp $(wildcard $(CONTAINERS)/*.h) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(CONTAINERS) $< $(LDFLAGS) $(LDLIBS) -o "$@"

check: all $(BUILD_DIR)/test-containers
	$(PYTHON) tests/check.py "$(BUILD_DIR)"
