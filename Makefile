BUILD_DIR := build

CMAKE := cmake
CMAKE_FLAGS := \
	       -S . -B $(BUILD_DIR) \
	       -DCMAKE_TOOLCHAIN_FILE=sys/cmake/toolchain-D1.cmake \
	       -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

FEL := ./xfel/xfel
FEL_ADDR = 0x40000000

RM := rm -f
MAKEFLAGS += --no-print-directory

.PHONY: config
config:
	cmake $(CMAKE_FLAGS)

.PHONY: bins
bins: config
	cmake --build $(BUILD_DIR)

.PHONY: fel-exec
# Run with sudo
fel-exec:
	$(FEL) version
	$(FEL) ddr d1
	$(FEL) write $(FEL_ADDR) $(BUILD_DIR)/$(NAME).bin
	$(FEL) exec $(FEL_ADDR)

.PHONY: clean
clean:
	$(RM) -r build/

.PHONY: re
re:
	$(MAKE) clean
	$(MAKE) bins
