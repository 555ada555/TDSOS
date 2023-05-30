
#[no_mangle]
fn _panic_exit() -> ! {
    libkernel::cpu::qemu_exit_success()
}
