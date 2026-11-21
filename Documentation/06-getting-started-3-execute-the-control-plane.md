# Getting Started: Execute the Control Plane

This is part 3 of 3 of Getting Started: [Hardware Connection](04-getting-started-1-hardware-connection.md), [Configure the Board](05-getting-started-2-configure-the-board.md), [Execute the Control Plane](06-getting-started-3-execute-the-control-plane.md).

Once the FPGA is programmed with the SOF, run the control plane program on the host. Clone and build the [host-control-library](https://github.com/altera-fpga/host-control-library) repository, then run its example. You should see:

<p align="center">
  <img src="images/figure-15-altera-control-link-execution.png" alt="Altera® Control Link Execution">
</p>

<p align="center"><strong>Figure 15. Altera® Control Link Execution.</strong></p>

<br>

You should then see the UDP transaction packets, for example in Wireshark:

<p align="center">
  <img src="images/figure-16-altera-control-link-udp-packets.png" alt="Altera® Control Link UDP packets">
</p>

<p align="center"><strong>Figure 16. Altera® Control Link UDP packets.</strong></p>

<br>

After the configuration is complete, you can see the RoCEv2 packets:

<p align="center">
  <img src="images/figure-17-streaming-after-fpga-configuration.png" alt="Streaming after FPGA configuration">
</p>

<p align="center"><strong>Figure 17. Streaming after FPGA configuration.</strong></p>

<br>

---

[← Getting Started: Configure the Board](05-getting-started-2-configure-the-board.md) | [Documentation index](README.md) | [Data Formats →](07-data-formats.md)
