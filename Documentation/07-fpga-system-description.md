# FPGA System Description

The design is implemented as the Quartus Prime project top, whose Platform Designer system is `top_qsys`, on the Agilex 5 device A5ED065BB32AE6SR0/ A5ED065BB32AE4S. The FPGA is partitioned into the Platform Designer subsystems shown in Figure 18. Each box in the figure is one subsystem in that project. Clock and reset trees are shown at a high level; the IP inside each subsystem is described on the subsystem pages listed in the table below.

The video path starts with two IMX678C camera inputs. The MIPI In Subsystem recovers the CSI-2 links and presents two 16-bit Bayer AXI4-Stream interfaces to the ISP In Subsystem. ISP In performs source selection and remosaic, then feeds two parallel ISP Lite pipes that convert 4K 12-bit Bayer video to 1080p, 30 FPS, 8-bit YUV 4:2:0. Two VVP test-pattern generators provide synthetic video of the same format. The Stream Switch Subsystem selects, for each of the two video streams, either the corresponding ISP Lite pipe or the corresponding test-pattern generator. The selected streams leave the switch as 24-bit YUV 4:2:0 AXI4-Stream and enter the RoCEv2 Subsystem.

The lidar path is independent of the video pipeline. The Lidar Ingest Subsystem captures the RPLIDAR A1M8 over UART and forwards the raw scan data as a 64-bit AXI4-Stream into the RoCEv2 Subsystem. The Ethernet Ingest Subsystem brings in a Livox MID-360S through SGMII/MDIO and the carrier 88E2110 on J4 Port B, and forwards two raw 64-bit streams, the point cloud and the IMU, into the RoCEv2 Subsystem. The Timestamp Subsystem maintains an IEEE 1588 96-bit time of day, which can be reset from GPIO (optionally, not relevant when PTP is enabled). It supplies that timestamp to the RoCEv2 Subsystem so that each video frame, lidar reading, and Ethernet ingest stream can be marked on the wire. It also broadcasts the 96-bit time of day to the 10G Ethernet MAC on one Avalon-ST connection and receives the time-of-day load on a second Avalon-ST connection.

Inside the RoCEv2 Subsystem the video streams are chroma-repacked from YUV 4:2:0 to the NV12-like format used on Ethernet, each stream is bridged onto a RoCEv2 queue pair, and the timestamp is inserted. Queue pairs 0x2 and 0x3 carry the two video streams, queue pair 0x4 carries the RPLIDAR stream, queue pair 0x5 carries the Livox MID-360S point cloud, and queue pair 0x6 carries the IMU from that same lidar. The point cloud and the IMU stay raw. The resulting RoCEv2 packets are handed to the 10G Ethernet Subsystem, which implements the GTS Ethernet hard IP, and leave the board on the SFP+ link to the host. One external GTS reset sequencer is shared by that 10G MAC and the Ethernet ingest PHY.

The Control Subsystem is a Nios V/g processor that configures the other subsystems over an Avalon-MM interconnect. It also exchanges non-RoCEv2 Ethernet traffic with the RoCEv2 Subsystem in both directions, using msgDMA transmit and receive channels. The Board Subsystem brings in the 100 MHz oscillator and the push-button reset. The Clock Subsystem multiplies that reference through an IOPLL and distributes clocks and resets to every subsystem: 100 MHz for CSR and CPU domains, including the Ethernet Ingest Nios V/g, 297 MHz for MIPI and ISP video, 148.5 MHz for the VVP AXI4-Stream fabric and the Ethernet Ingest main clock, 125 MHz shared by the lidar UART and Ethernet Ingest TX/RX, 62.5 MHz for the Ethernet Ingest PCS, and the Ethernet interface clock from the GTS hard IP.

![Altera Sensor Link - FPGA subsystem block diagram](images/figure-18-altera-sensor-link-fpga-subsystem-block-diagram.png)

*Figure 18. Altera Sensor Link - FPGA subsystem block diagram.*

The following table lists the subsystems in Figure 18 with the corresponding Platform Designer instance names in `top_qsys`.

| Subsystem | Quartus instance | Role |
| --- | --- | --- |
| [MIPI In Subsystem](subsystems/01-mipi-in-subsystem.md) | `mipi_in_subsystem` | MIPI D-PHY, dual CSI-2, pixel-format conversion, camera and board I2C |
| [ISP In Subsystem](subsystems/02-isp-in-subsystem.md) | `isp_in_subsystem` | Bayer source selection and remosaic |
| [ISP Lite Pipe 0](subsystems/03-isp-lite-pipe-0.md) | `isp_lite_subsystem` | 4K 12-bit Bayer to 1080p 8-bit YUV 4:2:0 |
| [ISP Lite Pipe 1](subsystems/04-isp-lite-pipe-1.md) | `isp_lite_subsystem_1` | 4K 12-bit Bayer to 1080p 8-bit YUV 4:2:0 |
| [Video TPG 0](subsystems/05-video-tpg-0.md) | `tpg_subsystem` | Synthetic video source |
| [Video TPG 1](subsystems/06-video-tpg-1.md) | `tpg_1_subsystem` | Synthetic video source |
| [Stream Switch Subsystem](subsystems/07-stream-switch-subsystem.md) | `switch_subsystem` | Selects ISP Lite or TPG for each of the two video streams |
| [Lidar Ingest Subsystem](subsystems/08-lidar-ingest-subsystem.md) | `lidar_ingest_subsystem` | Nios V UART capture and raw lidar streaming |
| [Ethernet Ingest Subsystem](subsystems/09-ethernet-ingest-subsystem.md) | `eth_ingest_subsystem` | TSE and Nios V/g; raw Livox point cloud and IMU |
| [Timestamp Subsystem](subsystems/11-timestamp-subsystem.md) | `tod_subsystem` | IEEE 1588 96-bit time of day, external reset, and broadcast to the 10G MAC |
| [RoCEv2 Subsystem](subsystems/12-rocev2-subsystem.md) | `rocev2_subsystem` | Five-channel VVP-to-ABR v1.1 bridge, chroma repack, and RoCEv2 transmit |
| [10G Ethernet Subsystem](subsystems/13-10g-ethernet-subsystem.md) | `gts_eth_subsystem` | GTS Ethernet hard IP, MAC, PCS, PMA, and IEEE 1588 |
| [External GTS Reset](subsystems/10-external-gts-reset.md) | `ext_gts_rst_subsystem` | Shared PMA reset for the 10G MAC and the ethernet ingest PHY |
| [Control Subsystem](subsystems/14-control-subsystem.md) | `control_subsystem` | Nios V configuration master and non-RoCEv2 TX/RX |
| Clock Subsystem | `clock_subsystem` | IOPLL and per-domain reset synchronization |
| Board Subsystem | `board_subsystem` | Board clock bridge and push-button reset |

---

[← Data Formats](06-data-formats.md) | [Documentation index](README.md) | [MIPI In Subsystem →](subsystems/01-mipi-in-subsystem.md)
