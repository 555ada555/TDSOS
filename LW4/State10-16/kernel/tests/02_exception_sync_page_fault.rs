

#![feature(format_args_nl)]
#![no_main]
#![no_std]

mod panic_exit_success;

use libkernel::{bsp, cpu, exception, info, memory, println};

#[no_mangle]
unsafe fn kernel_init() -> ! {
    exception::handling_init();
    memory::init();
    bsp::driver::qemu_bring_up_console();

    println!("Testing synchronous exception handling by causing a page fault");

    info!("Writing to bottom of address space to address 1 GiB...");
    let big_addr: u64 = 1024 * 1024 * 1024;
    core::ptr::read_volatile(big_addr as *mut u64);

    cpu::qemu_exit_failure()
}
