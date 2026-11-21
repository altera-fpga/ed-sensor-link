# Control Subsystem

The Control Subsystem is the Nios V/g that configures the other subsystems. It runs at 100 MHz from on-chip memory and reaches every exported slave through the Avalon-MM master shown as Control out. The System ID peripheral, the JTAG console and the 4-bit GPIO sit on the same fabric.

Non-RoCE Ethernet is a separate path. Receive packets cross from the Ethernet MAC clock into 100 MHz, and a modular scatter-gather DMA writes them into memory. Transmit reads them back out and crosses into the MAC clock. The IRQ bridge collects the input PIO, the external board and camera I2C interrupts, and the DMA interrupts. The JTAG UART and the DMA engines also interrupt through that bridge.

PTP initialization is enabled in this firmware. It runs at startup and programs the IEEE 1588 interface in the 10G Ethernet Subsystem. The hardware-initialization task is left commented out, so the cameras, RoCE, and the VVP-to-ABR bridge are programmed over UDP.

**IP in this subsystem**

| IP | Function |
| --- | --- |
| Nios V/g | Control processor. Configures the exported slaves in the other subsystems and services non-RoCE Ethernet. |
| On-Chip Memory | Firmware RAM, plus the system-description memory on the same fabric. |
| System ID Peripheral | Read-only system identifier and build timestamp. |
| JTAG UART | Host console for the control processor. |
| PIO | One input PIO and one output PIO, 4 bits each, for board GPIO. |
| IRQ Bridge | Collects local and exported interrupts and presents them to the processor. Shown as the IRQ mapper on the figure. |
| Avalon-MM Pipeline Bridge | The exported master, shown as Control out. This is how the processor reaches the other subsystems. |
| Modular Scatter-Gather DMA | Two engines. One writes received non-RoCE packets into memory. The other reads transmit packets back out. |
| Avalon-ST Dual Clock FIFO | Two FIFOs. One crosses receive traffic from the Ethernet MAC clock to 100 MHz. The other crosses transmit traffic back. |

![Control Subsystem block diagram](../images/figure-32-control-subsystem-block-diagram.png)

*Figure 32. Control Subsystem block diagram.*

---

[← 10G Ethernet Subsystem](13-10g-ethernet-subsystem.md) | [Documentation index](../README.md) | [Altera Control Link →](../08-altera-control-link.md)
