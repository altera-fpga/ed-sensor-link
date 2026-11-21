# Terminology and References

## Terminology

*Table 1. Terminology*

| Term | Description |
| --- | --- |
| RoCEv2 | RDMA over Converged Ethernet v2; IB transport over Ethernet for low-latency host delivery |
| 10 GbE | 10 Gigabit Ethernet physical/link between the FPGA and host. |
| MIPI CSI-2 | Camera serial interface used by IMX678C modules |
| ISP / ISP Lite | Image Signal Processing; ISP Lite is the lightweight on-FPGA conditioning path that converts 4K, 12-bit Bayer input to 1080p, 30 FPS, 8-bit YUV420 output. |
| MDT | Modular Design Toolkit; Quartus methodology for partitioned FPGA integration |
| SOF | SRAM Object File; FPGA configuration image for development kit programming |
| MAC/PHY | Ethernet media-access-control and physical-layer functions. |
| AXI4 | Advanced eXtensible Interface version 4 |
| AXI4-Lite | AXI4 subset used for low-throughput control and status accesses. |
| AXI4-Stream | AXI4 streaming interface used to transfer video and sensor data. |
| CPU | Central Processing Unit |
| HW | Hardware |
| IP | Intellectual Property |
| Rx | Receive |
| SW | Software |
| TBC | To Be Confirmed |
| TBD | To Be Decided |
| Tx | Transmit |
| GMSL | Gigabit Multimedia Serial Link; camera link supported through the MIPI0 input. |
| UC | Unreliable Connection; the RoCEv2 transport mode used for FPGA-to-host transmission in this release. |
| QP | Queue Pair; the RoCEv2 identifier assigned to each transmitted stream. |
| YUV420 | 8-bit 4:2:0 video format produced by the ISP Lite pipeline at 1080p and 30 FPS. |
| NV12-like | Video representation produced by the RoCEv2 chroma repacker for Ethernet transmission. |
| RETH | RoCEv2 extended transport header containing fields used to identify the virtual address and transport metadata. |
| JTAG | Joint Test Action Group interface used to detect and program the FPGA device. |
| ToD | Time of Day; timestamp reference used by the timestamp subsystem. |
| Immediate | RoCEv2 immediate-data packet marking the end of a video frame, lidar reading, or other sensor field and carrying timestamp information. |
| TPG | Test Pattern Generator. Video TPG 0 and Video TPG 1 are synthetic sources for the Stream Switch. ISP In also contains a Bayer test pattern. |
| CSI-2 | Camera Serial Interface 2. Each IMX678C link is decoded by a MIPI CSI-2 receiver. |
| D-PHY | MIPI D-PHY physical layer that recovers the serial camera links. One dual-link D-PHY serves both cameras. |
| VVP | Video and Vision Processing Suite. The ISP, switch, scaler, chroma and test-pattern blocks are VVP IP. |
| ABR | Base RoCEv2 IP. The VVP to ABR Bridge posts work-queue entries to it and holds the payload memory. |
| PSN | Packet Sequence Number. Initiator PSN numbers packets sent on a queue pair, and target PSN numbers packets received. The host writes both as 0. |
| `R_Key` | Remote key. The RDMA memory key for a buffer, programmed per channel in the VVP to ABR Bridge. |
| `P_Key` | Partition key. The 16-bit InfiniBand partition key in the destination header. The host writes 0xD05E. |
| `Q_Key` | Queue key. The 32-bit key in the destination header. The host writes 0xDECEA5ED. |
| DMA | Direct Memory Access. The Control Subsystem uses modular scatter-gather DMA for non-RoCE Ethernet transmit and receive. |
| SFP | Small form-factor pluggable module. The 10 GbE link leaves the board through the SFP+ cage. |
| PTP | Precision Time Protocol. In this design it is the IEEE 1588 interface on the 10G Ethernet port, initialised by the Control firmware at startup. |
| IEEE 1588 | The time-synchronization standard used for the 96-bit time of day and for the PTP interface at 0x0022F680. |

## Reference Documents

*Table 2. Reference Documents.*

| Document | Document<br>No./Location |
| --- | --- |
| Video and Vision Processing Suite IP User Guide | <https://docs.altera.com/r/docs/683329/25.1/video-and-vision-processing-suite-ip-user-guide/about-the-video-and-vision-processing-suite> |
| VVP Streaming Video Protocol Specification | <https://docs.altera.com/r/docs/683397/current/altera-streaming-video-protocol-specification/about-the-altera-streaming-video-protocol> |

---

[← Altera Control Link](08-altera-control-link.md) | [Documentation index](README.md) | [Appendix A: Nios V/g Control Memory Map →](appendix-a-control-memory-map.md)
