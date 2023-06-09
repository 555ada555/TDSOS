include ../common/docker.mk
include ../common/format.mk
include ../common/operating_system.mk

BSP ?= rpi3

DEV_SERIAL ?= /dev/ttyUSB0

QEMU_MISSING_STRING = "This board is not yet supported for QEMU."

ifeq ($(BSP),rpi3)
    TARGET                 = aarch64-unknown-none-softfloat
    KERNEL_BIN             = kernel8.img
    QEMU_BINARY            = qemu-system-aarch64
    QEMU_MACHINE_TYPE      = raspi3
    QEMU_RELEASE_ARGS      = -serial stdio -display none
    OBJDUMP_BINARY         = aarch64-none-elf-objdump
    NM_BINARY              = aarch64-none-elf-nm
    READELF_BINARY         = aarch64-none-elf-readelf
    LD_SCRIPT_PATH         = $(shell pwd)/src/bsp/raspberrypi
    RUSTC_MISC_ARGS        = -C target-cpu=cortex-a53
    CHAINBOOT_DEMO_PAYLOAD = demo_payload_rpi3.img
else ifeq ($(BSP),rpi4)
    TARGET                 = aarch64-unknown-none-softfloat
    KERNEL_BIN             = kernel8.img
    QEMU_BINARY            = qemu-system-aarch64
    QEMU_MACHINE_TYPE      =
    QEMU_RELEASE_ARGS      = -serial stdio -display none
    OBJDUMP_BINARY         = aarch64-none-elf-objdump
    NM_BINARY              = aarch64-none-elf-nm
    READELF_BINARY         = aarch64-none-elf-readelf
    LD_SCRIPT_PATH         = $(shell pwd)/src/bsp/raspberrypi
    RUSTC_MISC_ARGS        = -C target-cpu=cortex-a72
    CHAINBOOT_DEMO_PAYLOAD = demo_payload_rpi4.img
endif

export LD_SCRIPT_PATH

KERNEL_MANIFEST      = Cargo.toml
KERNEL_LINKER_SCRIPT = kernel.ld
LAST_BUILD_CONFIG    = target/$(BSP).build_config

KERNEL_ELF      = target/$(TARGET)/release/kernel
KERNEL_ELF_DEPS = $(filter-out %: ,$(file < $(KERNEL_ELF).d)) $(KERNEL_MANIFEST) $(LAST_BUILD_CONFIG)

RUSTFLAGS = $(RUSTC_MISC_ARGS)                   \
    -C link-arg=--library-path=$(LD_SCRIPT_PATH) \
    -C link-arg=--script=$(KERNEL_LINKER_SCRIPT)

RUSTFLAGS_PEDANTIC = $(RUSTFLAGS) \
    -D warnings                   \
    -D missing_docs

FEATURES      = --features bsp_$(BSP)
COMPILER_ARGS = --target=$(TARGET) \
    $(FEATURES)                    \
    --release

RUSTC_CMD   = cargo rustc $(COMPILER_ARGS)
DOC_CMD     = cargo doc $(COMPILER_ARGS)
CLIPPY_CMD  = cargo clippy $(COMPILER_ARGS)
OBJCOPY_CMD = rust-objcopy \
    --strip-all            \
    -O binary

EXEC_QEMU          = $(QEMU_BINARY) -M $(QEMU_MACHINE_TYPE)
EXEC_TEST_MINIPUSH = ruby tests/chainboot_test.rb
EXEC_MINIPUSH      = ruby ../common/serial/minipush.rb

DOCKER_CMD            = docker run -t --rm -v $(shell pwd):/work/tutorial -w /work/tutorial
DOCKER_CMD_INTERACT   = $(DOCKER_CMD) -i
DOCKER_ARG_DIR_COMMON = -v $(shell pwd)/../common:/work/common
DOCKER_ARG_DEV        = --privileged -v /dev:/dev

DOCKER_QEMU  = $(DOCKER_CMD_INTERACT) $(DOCKER_IMAGE)
DOCKER_TOOLS = $(DOCKER_CMD) $(DOCKER_IMAGE)
DOCKER_TEST  = $(DOCKER_CMD) $(DOCKER_ARG_DIR_COMMON) $(DOCKER_IMAGE)

ifeq ($(shell uname -s),Linux)
    DOCKER_CMD_DEV = $(DOCKER_CMD_INTERACT) $(DOCKER_ARG_DEV)

    DOCKER_CHAINBOOT = $(DOCKER_CMD_DEV) $(DOCKER_ARG_DIR_COMMON) $(DOCKER_IMAGE)
endif

.PHONY: all doc qemu chainboot clippy clean readelf objdump nm check

all: $(KERNEL_BIN)

$(LAST_BUILD_CONFIG):
	@rm -f target/*.build_config
	@mkdir -p target
	@touch $(LAST_BUILD_CONFIG)

$(KERNEL_ELF): $(KERNEL_ELF_DEPS)
	$(call color_header, "Compiling kernel ELF - $(BSP)")
	@RUSTFLAGS="$(RUSTFLAGS_PEDANTIC)" $(RUSTC_CMD)

$(KERNEL_BIN): $(KERNEL_ELF)
	$(call color_header, "Generating stripped binary")
	@$(OBJCOPY_CMD) $(KERNEL_ELF) $(KERNEL_BIN)
	$(call color_progress_prefix, "Name")
	@echo $(KERNEL_BIN)
	$(call color_progress_prefix, "Size")
	$(call disk_usage_KiB, $(KERNEL_BIN))

doc:
	$(call color_header, "Generating docs")
	@$(DOC_CMD) --document-private-items --open

ifeq ($(QEMU_MACHINE_TYPE),) # QEMU is not supported for the board.

qemu qemuasm:
	$(call color_header, "$(QEMU_MISSING_STRING)")

else # QEMU is supported.

qemu: $(KERNEL_BIN)
	$(call color_header, "Launching QEMU")
	@$(DOCKER_QEMU) $(EXEC_QEMU) $(QEMU_RELEASE_ARGS) -kernel $(KERNEL_BIN)

qemuasm: $(KERNEL_BIN)
	$(call color_header, "Launching QEMU with ASM output")
	@$(DOCKER_QEMU) $(EXEC_QEMU) $(QEMU_RELEASE_ARGS) -kernel $(KERNEL_BIN) -d in_asm

endif

chainboot: $(KERNEL_BIN)
	@$(DOCKER_CHAINBOOT) $(EXEC_MINIPUSH) $(DEV_SERIAL) $(CHAINBOOT_DEMO_PAYLOAD)

clippy:
	@RUSTFLAGS="$(RUSTFLAGS_PEDANTIC)" $(CLIPPY_CMD)

clean:
	rm -rf target $(KERNEL_BIN)

readelf: $(KERNEL_ELF)
	$(call color_header, "Launching readelf")
	@$(DOCKER_TOOLS) $(READELF_BINARY) --headers $(KERNEL_ELF)

objdump: $(KERNEL_ELF)
	$(call color_header, "Launching objdump")
	@$(DOCKER_TOOLS) $(OBJDUMP_BINARY) --disassemble --demangle \
                --section .text   \
                --section .rodata \
                $(KERNEL_ELF) | rustfilt

nm: $(KERNEL_ELF)
	$(call color_header, "Launching nm")
	@$(DOCKER_TOOLS) $(NM_BINARY) --demangle --print-size $(KERNEL_ELF) | sort | rustfilt

.PHONY: test test_boot

ifeq ($(QEMU_MACHINE_TYPE),) # QEMU is not supported for the board.

test_boot test:
	$(call color_header, "$(QEMU_MISSING_STRING)")

else # QEMU is supported.

test_boot: $(KERNEL_BIN)
	$(call color_header, "Boot test - $(BSP)")
	@$(DOCKER_TEST) $(EXEC_TEST_MINIPUSH) $(EXEC_QEMU) $(QEMU_RELEASE_ARGS) \
		-kernel $(KERNEL_BIN) $(CHAINBOOT_DEMO_PAYLOAD)

test: test_boot

endif
