.DEFAULT_GOAL := all
CC = cc
CXX = c++
BUILD_DIR ?= build
CPPFLAGS += -D_POSIX_C_SOURCE=200809L
CFLAGS += -std=c11 -Wall -Wextra -pthread
CXXFLAGS += -std=c++20 -Wall -Wextra -pthread
LDLIBS += -pthread

PROGRAMS := record-converter record-access pthread-matrix thread-tree file-sort \
            future-pipeline semaphore-sequencing pipe-echo signal-handshake \
            process-tree template-containers
TARGETS := $(addprefix $(BUILD_DIR)/,$(PROGRAMS))
CONTAINERS := exercises/cpp/template-containers

.PHONY: all
all: $(TARGETS)

$(BUILD_DIR):
	mkdir -p "$@"

define c_program
$(BUILD_DIR)/$(1): $(2) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(2) $(LDFLAGS) $(LDLIBS) -o "$$@"
endef

define cpp_program
$(BUILD_DIR)/$(1): $(2) $(3) | $(BUILD_DIR)
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
