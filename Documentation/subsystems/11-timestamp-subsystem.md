# Timestamp Subsystem

The Timestamp Subsystem keeps the 96-bit time of day used by RoCEv2 and by the 10G MAC. The master counter runs at 156.25 MHz, on the MAC `clk_tx_div`. A broadcast sends that time of day to the 10G MAC and to the synchronizer. The MAC returns the time-of-day load to the master on a second Avalon-ST connection. The synchronizer samples the master count into the slave clock domain, and the slave presents that time to RoCEv2.

An external reset is debounced and applied to the counters (not relevant when running PTP). The master, slave, and sampling clocks are the same 156.25 MHz MAC clock. Solid orange lines on the figure are reset. Dashed lines are Avalon-MM at 100 MHz. The synchronizer and the broadcast have no control agent.

**IP in this subsystem**

<table align="center">
  <thead>
    <tr><th>IP</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Ethernet IEEE 1588 Time of Day Clock</td><td>Two instances, a time-of-day master counter and a time-of-day slave. The master counts at 156.25 MHz on the MAC clock and accepts the time-of-day load from the 10G MAC. The slave holds the sampled value used as the RoCEv2 timestamp.</td></tr>
    <tr><td>Ethernet IEEE 1588 TOD Broadcast</td><td>Sends the master's 96-bit time of day to the 10G MAC and to the synchronizer.</td></tr>
    <tr><td>Ethernet IEEE 1588 TOD Synchronizer</td><td>Samples the master count into the slave clock domain.</td></tr>
    <tr><td>Reset Debouncer</td><td>Cleans the external reset before it is applied to the counters.</td></tr>
  </tbody>
</table>

<br>

<p align="center">
  <img src="../images/figure-29-timestamp-subsystem-block-diagram.png" alt="Timestamp Subsystem block diagram">
</p>

<p align="center"><strong>Figure 29. Timestamp Subsystem block diagram.</strong></p>

<br>

---

[← External GTS Reset](10-external-gts-reset.md) | [Documentation index](../README.md) | [RoCEv2 Subsystem →](12-rocev2-subsystem.md)
