
#[no_mangle]
fn _panic_exit() -> ! {
    libkernel::cpu::wait_forever()
}
