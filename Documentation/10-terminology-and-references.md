# Terminology and References

## Terminology

<p align="center"><strong>Table 1. Terminology.</strong></p>

<table align="center">
  <thead>
    <tr><th>Term</th><th>Description</th></tr>
  </thead>
  <tbody>
    <tr><td>RoCEv2</td><td>RDMA over Converged Ethernet version 2; InfiniBand transport over Ethernet for low-latency host delivery.</td></tr>
    <tr><td>10 GbE</td><td>10 Gigabit Ethernet physical/link between the FPGA and host.</td></tr>
    <tr><td>MIPI CSI-2</td><td>Camera serial interface used by IMX678C modules.</td></tr>
    <tr><td>ISP / ISP Lite</td><td>Image Signal Processing; ISP Lite is the lightweight on-FPGA conditioning path that converts 4K, 12-bit Bayer input to 1080p, 30 FPS, 8-bit YUV 4:2:0 output.</td></tr>
    <tr><td>SOF</td><td>SRAM Object File; FPGA configuration image for development kit programming.</td></tr>
    <tr><td>MAC/PHY</td><td>Ethernet media-access-control and physical-layer functions.</td></tr>
    <tr><td>AXI4</td><td>Advanced eXtensible Interface version 4.</td></tr>
    <tr><td>AXI4-Lite</td><td>AXI4 subset used for low-throughput control and status accesses.</td></tr>
    <tr><td>AXI4-Stream</td><td>AXI4 streaming interface used to transfer video and sensor data.</td></tr>
    <tr><td>CPU</td><td>Central Processing Unit.</td></tr>
    <tr><td>HW</td><td>Hardware.</td></tr>
    <tr><td>IP</td><td>Intellectual Property.</td></tr>
    <tr><td>Rx</td><td>Receive.</td></tr>
    <tr><td>SW</td><td>Software.</td></tr>
    <tr><td>Tx</td><td>Transmit.</td></tr>
    <tr><td>GMSL</td><td>Gigabit Multimedia Serial Link; camera link supported through the MIPI0 input.</td></tr>
    <tr><td>UC</td><td>Unreliable Connection; the RoCEv2 transport mode used for FPGA-to-host transmission in this release.</td></tr>
    <tr><td>QP</td><td>Queue Pair; the RoCEv2 identifier assigned to each transmitted stream.</td></tr>
    <tr><td>YUV 4:2:0</td><td>8-bit video format with 4:2:0 chroma subsampling, produced by the ISP Lite pipeline at 1080p and 30 FPS.</td></tr>
    <tr><td>NV12-like</td><td>Video representation produced by the RoCEv2 chroma repacker for Ethernet transmission.</td></tr>
    <tr><td>RETH</td><td>RoCEv2 extended transport header containing fields used to identify the virtual address and transport metadata.</td></tr>
    <tr><td>JTAG</td><td>Joint Test Action Group interface used to detect and program the FPGA device.</td></tr>
    <tr><td>ToD</td><td>Time of Day; timestamp reference used by the timestamp subsystem.</td></tr>
    <tr><td>Immediate</td><td>RoCEv2 immediate-data packet marking the end of a video frame, lidar reading, or other sensor reading, and carrying timestamp information.</td></tr>
    <tr><td>TPG</td><td>Test Pattern Generator. Video TPG 0 and Video TPG 1 are synthetic sources for the Stream Switch. ISP In also contains a Bayer test pattern.</td></tr>
    <tr><td>CSI-2</td><td>Camera Serial Interface 2. Each IMX678C link is decoded by a MIPI CSI-2 receiver.</td></tr>
    <tr><td>D-PHY</td><td>MIPI D-PHY physical layer that recovers the serial camera links. One dual-link D-PHY serves both cameras.</td></tr>
    <tr><td>VVP</td><td>Video and Vision Processing Suite. The ISP, switch, scaler, chroma and test-pattern blocks are VVP IP.</td></tr>
    <tr><td>Altera® Sensor Link Stream Multiplexer</td><td>Streaming front end of the RoCEv2 Subsystem. It inserts the timestamp, posts work-queue entries to the OmniRoCEv2 IP, and holds the payload memory.</td></tr>
    <tr><td>OmniRoCEv2 IP</td><td>RoCEv2 protocol engine. It builds and checks the RoCEv2 packets between the payload memory and the Ethernet line.</td></tr>
    <tr><td>PSN</td><td>Packet Sequence Number. Initiator PSN numbers packets sent on a queue pair, and target PSN numbers packets received. The host writes both as 0.</td></tr>
    <tr><td><code>R_Key</code></td><td>Remote key. The RDMA memory key for a buffer, programmed per channel in the Altera® Sensor Link Stream Multiplexer.</td></tr>
    <tr><td><code>P_Key</code></td><td>Partition key. The 16-bit InfiniBand partition key in the destination header. The host writes 0xD05E.</td></tr>
    <tr><td><code>Q_Key</code></td><td>Queue key. The 32-bit key in the destination header. The host writes 0xDECEA5ED.</td></tr>
    <tr><td>DMA</td><td>Direct Memory Access. The Control Subsystem uses modular scatter-gather DMA for non-RoCE Ethernet transmit and receive.</td></tr>
    <tr><td>SFP</td><td>Small form-factor pluggable module. The 10 GbE link leaves the board through the SFP+ cage.</td></tr>
    <tr><td>PTP</td><td>Precision Time Protocol. In this design it is the IEEE 1588 interface on the 10G Ethernet port, initialized by the Control firmware at startup.</td></tr>
    <tr><td>IEEE 1588</td><td>The time-synchronization standard used for the 96-bit time of day and for the PTP interface at 0x0022F680.</td></tr>
  </tbody>
</table>

<br>

## Reference Documents

<p align="center"><strong>Table 2. Reference Documents.</strong></p>

<table align="center">
  <thead>
    <tr><th>Document</th><th>Location</th></tr>
  </thead>
  <tbody>
    <tr><td>Video and Vision Processing Suite IP User Guide</td><td><a href="https://docs.altera.com/r/docs/683329/25.1/video-and-vision-processing-suite-ip-user-guide/about-the-video-and-vision-processing-suite">link</a></td></tr>
    <tr><td>VVP Streaming Video Protocol Specification</td><td><a href="https://docs.altera.com/r/docs/683397/current/altera-streaming-video-protocol-specification/about-the-altera-streaming-video-protocol">link</a></td></tr>
  </tbody>
</table>

<br>

---

[← Altera® Host Control Library](09-altera-host-control.md) | [Documentation index](README.md) | [Appendix A: Control Subsystem Memory Map →](appendix-a-control-memory-map.md)
