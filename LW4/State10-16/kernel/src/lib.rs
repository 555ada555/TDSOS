
#![allow(clippy::upper_case_acronyms)]
#![allow(incomplete_features)]
#![feature(asm_const)]
#![feature(const_option)]
#![feature(core_intrinsics)]
#![feature(format_args_nl)]
#![feature(generic_const_exprs)]
#![feature(int_roundings)]
#![feature(is_sorted)]
#![feature(linkage)]
#![feature(nonzero_min_max)]
#![feature(panic_info_message)]
#![feature(step_trait)]
#![feature(trait_alias)]
#![feature(unchecked_math)]
#![no_std]
// Testing
#![cfg_attr(test, no_main)]
#![feature(custom_test_frameworks)]
#![reexport_test_harness_main = "test_main"]
#![test_runner(crate::test_runner)]

mod panic_wait;
mod synchronization;

pub mod bsp;
pub mod common;
pub mod console;
pub mod cpu;
pub mod driver;
pub mod exception;
pub mod memory;
pub mod print;
pub mod state;
pub mod time;

//--------------------------------------------------------------------------------------------------
// Public Code
//--------------------------------------------------------------------------------------------------

/// Version string.
pub fn version() -> &'static str {
    concat!(
        env!("CARGO_PKG_NAME"),
        " version ",
        env!("CARGO_PKG_VERSION")
    )
}

//--------------------------------------------------------------------------------------------------
// Testing
//--------------------------------------------------------------------------------------------------

/// The default runner for unit tests.
pub fn test_runner(tests: &[&test_types::UnitTest]) {
    // This line will be printed as the test header.
    println!("Running {} tests", tests.len());

    for (i, test) in tests.iter().enumerate() {
        print!("{:>3}. {:.<58}", i + 1, test.name);

        // Run the actual test.
        (test.test_func)();

        // Failed tests call panic!(). Execution reaches here only if the test has passed.
        println!("[ok]")
    }
}

/// The `kernel_init()` for unit tests.
#[cfg(test)]
#[no_mangle]
unsafe fn kernel_init() -> ! {
    exception::handling_init();
    memory::init();
    bsp::driver::qemu_bring_up_console();

    test_main();

    cpu::qemu_exit_success()
}
