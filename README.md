# Altera® Sensor Link

Altera® Sensor Link is an edge platform that brings several sensors into one Agilex™ 5 FPGA and delivers them to a host over a 10 GbE link as separate RoCEv2 streams. Data is tagged with a common IEEE 1588 96-bit timestamp, and a UDP-based library (Altera Control Link) lets the host read and write any IP register in the design.

![Altera Sensor Link functional block diagram](Documentation/images/figure-01-altera-sensor-link-functional-block-diagram.png)

> This release is a proof of concept. It is not a production-ready solution.

## Streams

| Queue pair | Stream | Source |
| --- | --- | --- |
| 0x2 | Video 0, 1080p30 | Framos IMX678C camera or test pattern |
| 0x3 | Video 1, 1080p30 | Framos IMX678C camera or test pattern |
| 0x4 | 2D lidar | Slamtec RPLIDAR A1M8, about 5.5 Hz |
| 0x5 | 3D lidar point cloud | Livox MID-360S, about 2.1 kHz |
| 0x6 | 3D lidar IMU | Livox MID-360S, about 133 Hz |

The transport is unidirectional RoCEv2 (Unreliable Connection) from the FPGA to the host.

## Hardware

- Agilex 5 FPGA and SoC E-Series Modular Development Kit (ES or 065B production device)
- Two Framos FSM:GO IMX678C camera modules (MIPI0 can use an optional Framos GMSL3 module)
- Slamtec RPLIDAR A1M8 (UART, needs a cable rework and a 5 V supply)
- Livox MID-360S (Ethernet)
- A host with a 10 GbE link

The full bill of materials is in [Deliverables](Documentation/04-deliverables.md).

## Quick start

1. Wire the sensors as described in [Getting Started](Documentation/05-getting-started.md).
2. Run PTP on the host as a master. The FPGA PTP engine supports secondary mode only.
3. Power on the lidar and the GMSL module, then the development kit.
4. Program the SOF that matches your board with Quartus Programmer 26.1.
5. Build and run the Altera Control Link example on the host. No RoCEv2 packets are sent until the host configures the FPGA.
6. Capture the RoCEv2 stream on the host, for example with Wireshark.

## Documentation

The full documentation is in the [Documentation](Documentation/README.md) folder.

- [Introduction](Documentation/01-introduction.md) and [Scope](Documentation/02-scope.md)
- [Architecture](Documentation/03-architecture.md)
- [Deliverables](Documentation/04-deliverables.md)
- [Getting Started](Documentation/05-getting-started.md)
- [Data Formats](Documentation/06-data-formats.md)
- [FPGA System Description](Documentation/07-fpga-system-description.md)
- [Altera Control Link](Documentation/08-altera-control-link.md)
- [Terminology and References](Documentation/09-terminology-and-references.md)
- [Appendix A: Control Memory Map](Documentation/appendix-a-control-memory-map.md)
