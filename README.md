# Altera® Sensor Link

Altera® Sensor Link is an edge platform that brings several sensors into one Agilex™ 5 FPGA and delivers them to a host over a high-speed network link as separate RoCEv2 streams. Data is tagged with a common IEEE 1588 96-bit timestamp, and a UDP-based control plane, provided by the Altera® Host Control library, allows the host to read and write any IP register in the design.

![Altera® Sensor Link functional block diagram](Documentation/images/figure-01-altera-sensor-link-functional-block-diagram.png)

The reference design ingests four physical sensors and turns them into five heterogeneous data streams, listed in [Streams in this Reference Design](#streams-in-this-reference-design).

Every stream is published as RoCEv2 traffic over the 10 GbE link, and each one has its own queue pair (QP). The host tells the streams apart by QP number, so it can handle each data type on its own path. The host can consume the streams in one of three ways:

- **Raw packet parsing:** capture the RoCEv2 packets and decode the headers and payloads directly. This gives full control and needs no RDMA stack.
- **Software RoCE receiver:** run a software implementation of RoCEv2 on a standard NIC, for example Linux SoftRoCE (`rdma_rxe`), and receive the streams through the RDMA verbs API.
- **RDMA transactions:** use an RDMA-capable NIC and the verbs API. The FPGA writes each frame into host memory buffers, so the host registers those buffers and reads them as they fill. Each frame ends with an Immediate packet that carries the timestamp.

Details for each stream are in [Data Formats](Documentation/07-data-formats.md).

> This release is a Reference Design. It is not a production-ready solution.

## Hardware

- [Agilex™ 5 FPGA and SoC E-Series Modular Development Kit (ES)](https://www.altera.com/products/devkit/po-3001/agilex-5-fpga-and-soc-e-series-modular-development-kit-es) (MK-A5E065BB32AES1), or [Agilex™ 5 FPGA and SoC E-Series 065B Modular Development Kit (Group B Device)](https://www.altera.com/products/devkit/po-3274/agilex-5-fpga-and-soc-e-series-065b-modular-development-kit) (MK-A5E065BB32AEA)
- Two Framos [FSM:GO IMX678C](https://www.framos.com/en/fsmgo) camera modules (MIPI0 can use an optional [Framos GMSL3](https://www.mouser.co.uk/ProductDetail/FRAMOS/FFA-GMSL3-Kit-5m?qs=%252BHhoWzUJg4IkLHv%2F6fzsXQ%3D%3D) module)
- [Slamtec RPLIDAR A1M8](https://www.slamtec.com/en/lidar/a1) (UART, needs a cable rework and a 5 V supply)
- [Livox MID-360S](https://www.livoxtech.com/mid-360s) (Ethernet)
- A host with a 10 GbE link

The full list with purchase links is in the [Bill of Materials](Documentation/03-bill-of-materials.md).

## Provided FPGA Binaries (SOF)

The SOF files are published on the [BIN_REL_1_0 release](https://github.com/altera-fpga/ed-sensor-link/releases/tag/BIN_REL_1_0).

- **[`DUAL-CAM_LIDAR2D_LIDAR3D_ES.sof`](https://github.com/altera-fpga/ed-sensor-link/releases/download/BIN_REL_1_0/DUAL-CAM_LIDAR2D_LIDAR3D_ES.sof)**: Provides the hardware to configure two cameras over direct MIPI connection and/or one GMSL module connected to the MIPI0 connector. The user selects the configuration through the control interface. This SOF includes a 2D lidar ingest and a 3D lidar (point cloud and IMU) for the Agilex™ 5 FPGA and SoC E-Series Modular Development Kit (ES) (MK-A5E065BB32AES1).
- **[`DUAL-CAM_LIDAR2D_LIDAR3D_PROD.sof`](https://github.com/altera-fpga/ed-sensor-link/releases/download/BIN_REL_1_0/DUAL-CAM_LIDAR2D_LIDAR3D_PROD.sof)**: Provides the hardware to configure two cameras over direct MIPI connection and/or one GMSL module connected to the MIPI0 connector. The user selects the configuration through the control interface. This SOF includes a 2D lidar ingest and a 3D lidar (point cloud and IMU) for the Agilex™ 5 FPGA and SoC E-Series 065B Modular Development Kit (Group B Device) (MK-A5E065BB32AEA).

## Altera® Host Control Library

The Altera® Host Control Library is published in its own repository: [altera-fpga/host-control-library](https://github.com/altera-fpga/host-control-library). This library is used for initializing and configuring the FPGA IP exposed through the control plane. You can find example host applications to support the SOFs listed above in [`host-examples`](host-examples).

## Quick start

> [!NOTE]
> Run PTP on the host as a master before programming the board. The FPGA internal PTP engine only supports secondary mode.

1. Wire the sensors as described in [Getting Started: Hardware Connection](Documentation/04-getting-started-1-hardware-connection.md).
2. Power on the lidar and the GMSL module, then the development kit.
3. Program the SOF that matches your board with Quartus Programmer 26.1. See [Getting Started: Configure the Board](Documentation/05-getting-started-2-configure-the-board.md).
4. Build and run the corresponding Altera® Host Control example application from the [host-examples](host-examples) directory. No RoCEv2 packets are sent until the host configures the FPGA. See [Getting Started: Execute the Control Plane](Documentation/06-getting-started-3-execute-the-control-plane.md).

## Streams in this Reference Design

<table align="center">
  <thead>
    <tr><th>Queue pair</th><th>Stream</th><th>Source</th></tr>
  </thead>
  <tbody>
    <tr><td>0x2</td><td>Video 0, 1080p30</td><td>Framos IMX678C camera or test pattern</td></tr>
    <tr><td>0x3</td><td>Video 1, 1080p30</td><td>Framos IMX678C camera or test pattern</td></tr>
    <tr><td>0x4</td><td>2D lidar</td><td>Slamtec RPLIDAR A1M8, about 5.5 Hz</td></tr>
    <tr><td>0x5</td><td>3D lidar point cloud</td><td>Livox MID-360S, about 2.1 kHz</td></tr>
    <tr><td>0x6</td><td>3D lidar IMU</td><td>Livox MID-360S, about 133 Hz</td></tr>
  </tbody>
</table>

<br>

The transport is unidirectional RoCEv2 (Unreliable Connection) from the FPGA to the host. For the layout of each stream, including the timestamp format, see [Data Formats](Documentation/07-data-formats.md).

## Documentation

The full documentation is in the [Documentation](Documentation/README.md) folder.

- [Introduction and Scope](Documentation/01-introduction-and-scope.md)
- [Architecture](Documentation/02-architecture.md)
- [Bill of Materials](Documentation/03-bill-of-materials.md)
- Getting Started: [Hardware Connection](Documentation/04-getting-started-1-hardware-connection.md), [Configure the Board](Documentation/05-getting-started-2-configure-the-board.md), [Execute the Control Plane](Documentation/06-getting-started-3-execute-the-control-plane.md)
- [Data Formats](Documentation/07-data-formats.md)
- [FPGA System Description](Documentation/08-fpga-system-description.md)
- [Altera® Host Control Library](Documentation/09-altera-host-control.md)
- [Terminology and References](Documentation/10-terminology-and-references.md)
- [Appendix A: Control Subsystem Memory Map](Documentation/appendix-a-control-memory-map.md)

## Notices & Disclaimers

Altera® Corporation technologies may require enabled hardware, software or service activation. No product or component can be absolutely secure. Performance varies by use, configuration and other factors. Your costs and results may vary. You may not use or facilitate the use of this document in connection with any infringement or other legal analysis concerning Altera® or products described herein. You agree to grant Altera® Corporation a non-exclusive, royalty-free license to any patent claim thereafter drafted which includes subject matter disclosed herein. No license (express or implied, by estoppel or otherwise) to any intellectual property rights is granted by this document, with the sole exception that you may publish an unmodified copy. You may create software implementations based on this document and in compliance with the foregoing that are intended to execute on the Altera® product(s) referenced in this document. No rights are granted to create modifications or derivatives of this document. The products described may contain design defects or errors known as errata which may cause the product to deviate from published specifications. Current characterized errata are available on request. Altera® disclaims all express and implied warranties, including without limitation, the implied warranties of merchantability, fitness for a particular purpose, and non-infringement, as well as any warranty arising from course of performance, course of dealing, or usage in trade. You are responsible for safety of the overall system, including compliance with applicable safety-related requirements or standards.

© Altera® Corporation. Altera®, the Altera® logo, and other Altera® marks are trademarks of Altera® Corporation. Other names and brands may be claimed as the property of others.

OpenCL\* and the OpenCL\* logo are trademarks of Apple Inc. used by permission of the Khronos Group™.
