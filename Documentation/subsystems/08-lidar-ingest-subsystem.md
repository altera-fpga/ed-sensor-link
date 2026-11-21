# Lidar Ingest Subsystem

The Lidar Ingest Subsystem is a Nios V/c-based subsystem. It reads the lidar UART, assembles packets in on-chip RAM, and writes a buffer streamer. The streamer crosses from 100 MHz to 148.5 MHz and emits a 64-bit AXI4-Stream to the RoCEv2 Subsystem.

The JTAG UART is the processor console, so printf output is visible on the host. Orange lines on the figure are interrupts. The UART interrupt crosses from 125 MHz to 100 MHz. There is no Avalon-MM port from the Control Subsystem.

**IP in this subsystem**

<table align="center">
  <thead>
    <tr><th>IP</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Nios V/c</td><td>The lidar processor. It reads the UART and writes the buffer streamer.</td></tr>
    <tr><td>16550 Compatible UART</td><td>Receives the lidar serial stream. The UART clock on the figure is 125 MHz.</td></tr>
    <tr><td>On-Chip Memory</td><td>Working RAM for the lidar processor and for packet assembly.</td></tr>
    <tr><td>Buffer Streamer</td><td>Takes the assembled lidar data and emits the 64-bit stream toward RoCEv2, crossing from 100 MHz to 148.5 MHz.</td></tr>
    <tr><td>JTAG UART</td><td>Host console for the lidar processor.</td></tr>
  </tbody>
</table>

<br>

<p align="center">
  <img src="../images/figure-26-lidar-ingest-subsystem-block-diagram.png" alt="Lidar Ingest Subsystem block diagram">
</p>

<p align="center"><strong>Figure 26. Lidar Ingest Subsystem block diagram.</strong></p>

<br>

---

[← Stream Switch Subsystem](07-stream-switch-subsystem.md) | [Documentation index](../README.md) | [Ethernet Ingest Subsystem →](09-ethernet-ingest-subsystem.md)
