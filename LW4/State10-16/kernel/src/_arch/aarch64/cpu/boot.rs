

use crate::{memory, memory::Address};
use aarch64_cpu::{asm, registers::*};
use core::arch::global_asm;
use tock_registers::interfaces::Writeable;


global_asm!(
    include_str!("boot.s"),
    CONST_CURRENTEL_EL2 = const 0x8,
    CONST_CORE_ID_MASK = const 0b11
);

#[inline(always)]
unsafe fn prepare_el2_to_el1_transition(
    virt_boot_core_stack_end_exclusive_addr: u64,
    virt_kernel_init_addr: u64,
) {

    CNTHCTL_EL2.write(CNTHCTL_EL2::EL1PCEN::SET + CNTHCTL_EL2::EL1PCTEN::SET);

    CNTVOFF_EL2.set(0);

    HCR_EL2.write(HCR_EL2::RW::EL1IsAarch64);

    SPSR_EL2.write(
        SPSR_EL2::D::Masked
            + SPSR_EL2::A::Masked
            + SPSR_EL2::I::Masked
            + SPSR_EL2::F::Masked
            + SPSR_EL2::M::EL1h,
    );

    ELR_EL2.set(virt_kernel_init_addr);

    SP_EL1.set(virt_boot_core_stack_end_exclusive_addr);
}

#[no_mangle]
pub unsafe extern "C" fn _start_rust(
    phys_kernel_tables_base_addr: u64,
    virt_boot_core_stack_end_exclusive_addr: u64,
    virt_kernel_init_addr: u64,
) -> ! {
    prepare_el2_to_el1_transition(
        virt_boot_core_stack_end_exclusive_addr,
        virt_kernel_init_addr,
    );

    let addr = Address::new(phys_kernel_tables_base_addr as usize);
    memory::mmu::enable_mmu_and_caching(addr).unwrap();
    asm::eret()
}
