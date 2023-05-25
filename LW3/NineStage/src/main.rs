#![allow(clippy::upper_case_acronyms)]
#![feature(asm_const)]
#![feature(const_option)]
#![feature(format_args_nl)]
#![feature(nonzero_min_max)]
#![feature(panic_info_message)]
#![feature(trait_alias)]
#![feature(unchecked_math)]
#![no_main]
#![no_std]

mod bsp;
mod console;
mod cpu;
mod driver;
mod exception;
mod panic_wait;
mod print;
mod synchronization;
mod time;
unsafe fn kernel_init() -> ! {
    if let Err(x) = bsp::driver::init() {
        panic!("Error initializing BSP driver subsystem: {}", x);
    }
    driver::driver_manager().init_drivers();
    kernel_main()
}
fn kernel_main() -> ! {
    use console::console;
    use core::time::Duration;

    info!(
        "{} version {}",
        env!("CARGO_PKG_NAME"),
        env!("CARGO_PKG_VERSION")
    );
    info!("Booting on: {}", bsp::board_name());

    let (_, privilege_level) = exception::current_privilege_level();
    info!("Current privilege level: {}", privilege_level);

    info!("Exception handling state:");
    exception::asynchronous::print_state();

    info!(
        "Architectural timer resolution: {} ns",
        time::time_manager().resolution().as_nanos()
    );

    info!("Drivers loaded:");
    driver::driver_manager().enumerate();

    info!("Timer test, spinning for 1 second");
    time::time_manager().spin_for(Duration::from_secs(1));

    info!("Echoing input now");
    console().clear_rx();
    loop {
        let c = console().read_char();
        console().write_char(c);
    }
}
