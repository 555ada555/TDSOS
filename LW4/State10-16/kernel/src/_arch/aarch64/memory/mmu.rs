

use crate::{
    bsp, memory,
    memory::{mmu::TranslationGranule, Address, Physical},
};
use aarch64_cpu::{asm::barrier, registers::*};
use core::intrinsics::unlikely;
use tock_registers::interfaces::{ReadWriteable, Readable, Writeable};

struct MemoryManagementUnit;

pub type Granule512MiB = TranslationGranule<{ 512 * 1024 * 1024 }>;
pub type Granule64KiB = TranslationGranule<{ 64 * 1024 }>;

#[allow(dead_code)]
pub mod mair {
    pub const DEVICE: u64 = 0;
    pub const NORMAL: u64 = 1;
}

static MMU: MemoryManagementUnit = MemoryManagementUnit;

impl<const AS_SIZE: usize> memory::mmu::AddressSpace<AS_SIZE> {
    pub const fn arch_address_space_size_sanity_checks() {
        assert!((AS_SIZE % Granule512MiB::SIZE) == 0);

        assert!(AS_SIZE <= (1 << 48));
    }
}

impl MemoryManagementUnit {
    #[inline(always)]
    fn set_up_mair(&self) {
        MAIR_EL1.write(
            MAIR_EL1::Attr1_Normal_Outer::WriteBack_NonTransient_ReadWriteAlloc +
        MAIR_EL1::Attr1_Normal_Inner::WriteBack_NonTransient_ReadWriteAlloc +

        MAIR_EL1::Attr0_Device::nonGathering_nonReordering_EarlyWriteAck,
        );
    }
    #[inline(always)]
    fn configure_translation_control(&self) {
        let t1sz = (64 - bsp::memory::mmu::KernelVirtAddrSpace::SIZE_SHIFT) as u64;

        TCR_EL1.write(
            TCR_EL1::TBI1::Used
                + TCR_EL1::IPS::Bits_40
                + TCR_EL1::TG1::KiB_64
                + TCR_EL1::SH1::Inner
                + TCR_EL1::ORGN1::WriteBack_ReadAlloc_WriteAlloc_Cacheable
                + TCR_EL1::IRGN1::WriteBack_ReadAlloc_WriteAlloc_Cacheable
                + TCR_EL1::EPD1::EnableTTBR1Walks
                + TCR_EL1::A1::TTBR1
                + TCR_EL1::T1SZ.val(t1sz)
                + TCR_EL1::EPD0::DisableTTBR0Walks,
        );
    }
}

pub fn mmu() -> &'static impl memory::mmu::interface::MMU {
    &MMU
}

use memory::mmu::MMUEnableError;

impl memory::mmu::interface::MMU for MemoryManagementUnit {
    unsafe fn enable_mmu_and_caching(
        &self,
        phys_tables_base_addr: Address<Physical>,
    ) -> Result<(), MMUEnableError> {
        if unlikely(self.is_enabled()) {
            return Err(MMUEnableError::AlreadyEnabled);
        }

        
        if unlikely(!ID_AA64MMFR0_EL1.matches_all(ID_AA64MMFR0_EL1::TGran64::Supported)) {
            return Err(MMUEnableError::Other(
                "Translation granule not supported in HW",
            ));
        }

       
        self.set_up_mair();

       
        TTBR1_EL1.set_baddr(phys_tables_base_addr.as_usize() as u64);

        self.configure_translation_control();

        barrier::isb(barrier::SY);

       
        SCTLR_EL1.modify(SCTLR_EL1::M::Enable + SCTLR_EL1::C::Cacheable + SCTLR_EL1::I::Cacheable);

        barrier::isb(barrier::SY);

        Ok(())
    }

    #[inline(always)]
    fn is_enabled(&self) -> bool {
        SCTLR_EL1.matches_all(SCTLR_EL1::M::Enable)
    }
}
