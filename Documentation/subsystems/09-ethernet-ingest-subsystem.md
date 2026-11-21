# Ethernet Ingest Subsystem

The Ethernet Ingest Subsystem is a separate Nios V/g, not the Control Subsystem processor. It connects to a Livox MID-360S through SGMII/MDIO and the carrier 88E2110 on J4 Port B (Lower). The Triple-Speed Ethernet MAC receives the sensor frames, and the processor writes two buffer streamers. Streamer 0 forwards the raw 64-bit point cloud to RoCEv2 channel 3, queue pair 0x5. Streamer 1 forwards the raw 64-bit IMU to RoCEv2 channel 4, queue pair 0x6.

Both streamers cross from 100 MHz to 148.5 MHz. The MAC TX/RX clock is 125 MHz and the PCS clock is 62.5 MHz. The processor runs at 100 MHz. The JTAG UART is a processor console. There is no Avalon-MM port from the Control Subsystem. The PMA reset grant comes from the external GTS reset.

**IP in this subsystem**

| IP | Function |
| --- | --- |
| Nios V/g | The Ethernet Ingest processor at 100 MHz. It receives the Livox packets and writes the two buffer streamers. |
| Triple-Speed Ethernet MAC | `eth_tse`. SGMII/MDIO to the 88E2110 on J4 Port B. The TX/RX clock is 125 MHz and the PCS clock is 62.5 MHz. |
| mSGDMA | RX and TX paths between the MAC and memory, clocked at 100 MHz. |
| Buffer Streamer 0 | Raw 64-bit point cloud to RoCEv2 channel 3, queue pair 0x5. Crosses from 100 MHz to 148.5 MHz. |
| Buffer Streamer 1 | Raw 64-bit IMU to RoCEv2 channel 4, queue pair 0x6. Crosses from 100 MHz to 148.5 MHz. |
| JTAG UART | Host console for the Ethernet Ingest processor. |

![Ethernet Ingest Subsystem block diagram](../images/figure-27-ethernet-ingest-subsystem-block-diagram.png)

*Figure 27. Ethernet Ingest Subsystem block diagram.*

---

[← Lidar Ingest Subsystem](08-lidar-ingest-subsystem.md) | [Documentation index](../README.md) | [External GTS Reset →](10-external-gts-reset.md)
