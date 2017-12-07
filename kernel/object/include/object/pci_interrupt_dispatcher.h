// Copyright 2016 The Fuchsia Authors
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once
#if WITH_DEV_PCIE

#include <dev/pcie_irqs.h>
#include <zircon/types.h>
#include <object/interrupt_dispatcher.h>
#include <object/pci_device_dispatcher.h>
#include <sys/types.h>

class PciDeviceDispatcher;

class PciInterruptDispatcher final : public InterruptDispatcher {
public:

    static constexpr uint32_t MASKABLE = (1u << 0);
    static constexpr uint32_t LEVEL_TRIGGERED = (1u << 1);
    static constexpr uint32_t FLAGS_MASK = (MASKABLE | LEVEL_TRIGGERED);

    static zx_status_t Create(const fbl::RefPtr<PcieDevice>& device,
                              uint32_t irq_id,
                              uint32_t flags,
                              zx_rights_t* out_rights,
                              fbl::RefPtr<Dispatcher>* out_interrupt);

    ~PciInterruptDispatcher() final;

    zx_status_t Bind(uint32_t slot, uint32_t vector, uint32_t options) final;
    zx_status_t Unbind(uint32_t slot) final;
    zx_status_t WaitForInterrupt(zx_time_t deadline, uint64_t& out_slots) final;
    zx_status_t WaitForInterruptWithTimeStamp(zx_time_t deadline, uint32_t& out_slot,
                                              zx_time_t& out_timestamp) final;
    zx_status_t UserSignal() final;

private:
    static pcie_irq_handler_retval_t IrqThunk(const PcieDevice& dev,
                                              uint irq_id,
                                              void* ctx);
    PciInterruptDispatcher(uint32_t irq_id, uint32_t flags)
        : irq_id_(irq_id),
          flags_(flags) { }

    const uint32_t irq_id_;
    const uint32_t flags_;
    fbl::RefPtr<PcieDevice> device_;
};

#endif  // if WITH_DEV_PCIE
