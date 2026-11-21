# RoCEv2 Subsystem

The RoCEv2 Subsystem sends five streams to the host over Ethernet. Channels 0 and 1 carry the video streams on queue pairs 0x2 and 0x3. Channel 2 carries the RPLIDAR stream on queue pair 0x4. Channel 3 carries the Livox MID-360S point cloud on queue pair 0x5, and channel 4 carries the IMU from that lidar on queue pair 0x6. The VVP-to-ABR bridge is v1.1. This is the lightweight streaming use of the Altera Base RoCEv2 IP: application logic and a payload memory sit in front of the IP, and the IP builds and checks the RoCEv2 packets with little work left to the Control Nios.

Each video stream is 4:2:0 and passes through a 420 Chroma Repack IP, which interleaves Cb and Cr inside one packet, the NV12-like layout shown on the figure. The RPLIDAR, point-cloud, and IMU streams are raw 64-bit channels and do not pass through a chroma repack. The VVP-to-ABR bridge v1.1 inserts the 96-bit time of day, posts transmit and receive work-queue entries to the Base RoCEv2 IP, and holds the payload memory the IP reads and writes. The work-queue entry id is 5 bits wide. The IP returns a completion when that work finishes. On the figure the IP is labelled OmniRoCE.

As an initiator, the Base RoCEv2 IP takes a transmit work-queue entry, reads the payload from local memory, and sends it as one or more Ethernet packets with the InfiniBand transport, IP and UDP headers. When the entry is complete it returns a completion-queue entry. Receive work-queue entries describe buffers for data that arrives from the host. The line interface is 64-bit Avalon-ST to and from the 10G Ethernet Subsystem, on the Ethernet MAC clock.

Packets that are not RoCEv2, such as ARP and ping, are separated onto a second Avalon-ST pair and handled by the Control Subsystem. The bridge and the RoCEv2 IP are configured over Avalon-MM at 100 MHz. Pause, `init_done` and irq are omitted from the figure.

The VVP-to-ABR bridge register map programs each channel with a type, a line count, a queue pair, Num Buffers, Load Buffer, the remote key and address where the payload is written, a chroma-plane offset, and a separate host address for the timestamp. Channel type 0 is VVP 4:2:0 video and type 1 is a raw stream. The firmware table sets one buffer on every channel. A line count or chroma offset of zero selects the defaults of 1080 lines and a chroma offset of 1920 x 1080. The bridge also measures the video clock and the Ethernet clock against the 100 MHz control clock.

**IP in this subsystem**

| IP | Function |
| --- | --- |
| 420 Chroma Repack | Two instances, one for each video channel. Repacks 4:2:0 chroma so Cb and Cr are interleaved in the same packet. The three raw channels bypass it. |
| VVP to ABR Bridge | Up to 8 channels. Inserts the timestamp, posts transmit and receive work-queue entries, and provides the payload memory. Channels 0 and 1 are 24-bit video. Channels 2, 3, and 4 are 64-bit raw streams. |
| Base RoCEv2 IP | Offloads the RoCEv2 transport and the IP and Ethernet headers between the payload memory and the Ethernet line. Shown as OmniRoCE on the figure. |

![RoCEv2 Subsystem block diagram](../images/figure-30-rocev2-subsystem-block-diagram.png)

*Figure 30. RoCEv2 Subsystem block diagram.*

---

[← Timestamp Subsystem](11-timestamp-subsystem.md) | [Documentation index](../README.md) | [10G Ethernet Subsystem →](13-10g-ethernet-subsystem.md)
