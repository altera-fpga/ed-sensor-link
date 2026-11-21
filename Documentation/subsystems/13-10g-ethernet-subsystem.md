# 10G Ethernet Subsystem

The 10G Ethernet Subsystem is the physical Ethernet port for the RoCEv2 line. Transmit and receive are 64-bit Avalon-ST streams to and from the RoCEv2 Subsystem, and both streams pass through the IEEE 1588 PTP interface. It takes the 96-bit time of day from the Timestamp Subsystem and returns the time-of-day load on a second Avalon-ST connection. A PTP sample PLL makes a 114.285714 MHz sample clock for the hard IP from the 100 MHz CSR clock. The Ethernet interface clock generated here is also the RoCEv2 MAC clock and the Control Subsystem Ethernet clock. PMA reset for this MAC comes from the External GTS Reset subsystem, shared with the Ethernet Ingest PHY.

**IP in this subsystem**

<table align="center">
  <thead>
    <tr><th>IP</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>GTS Ethernet Hard IP</td><td>10G MAC, PCS and PMA between the IEEE 1588 interface and the SFP+ serial pins.</td></tr>
    <tr><td>IEEE 1588 PTP</td><td>Sits on the transmit and receive streams. Control base 0x0022F680, 128 bytes. Receives the 96-bit time of day and returns the load.</td></tr>
    <tr><td>PTP Sample PLL</td><td>Takes the 100 MHz CSR clock as reference and produces the 114.285714 MHz sample clock for the hard IP.</td></tr>
    <tr><td>GTS System PLL Clocks</td><td>Produces the transceiver and Ethernet interface clocks for the hard IP.</td></tr>
  </tbody>
</table>

<br>

<p align="center">
  <img src="../images/figure-31-10g-ethernet-subsystem-block-diagram.png" alt="10G Ethernet Subsystem block diagram">
</p>

<p align="center"><strong>Figure 31. 10G Ethernet Subsystem block diagram.</strong></p>

<br>

---

[← RoCEv2 Subsystem](12-rocev2-subsystem.md) | [Documentation index](../README.md) | [Control Subsystem →](14-control-subsystem.md)
