

use aarch64_cpu::asm;

pub use asm::nop;

#[inline(always)]
pub fn wait_forever() -> ! {
    loop {
        asm::wfe()
    }
}

#[cfg(feature = "test_build")]
use qemu_exit::QEMUExit;

#[cfg(feature = "test_build")]
const QEMU_EXIT_HANDLE: qemu_exit::AArch64 = qemu_exit::AArch64::new();

#[cfg(feature = "test_build")]
pub fn qemu_exit_failure() -> ! {
    QEMU_EXIT_HANDLE.exit_failure()
}

#[cfg(feature = "test_build")]
pub fn qemu_exit_success() -> ! {
    QEMU_EXIT_HANDLE.exit_success()
}
