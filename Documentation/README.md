# Altera® Sensor Link Documentation

Altera® Sensor Link is an edge platform that ingests camera, lidar, and IMU data on one [Agilex™ 5 FPGA](https://www.altera.com/products/fpga/agilex-5) and delivers it to a host as RoCEv2 streams over 10 GbE. This folder holds the full design documentation. The [top-level README](../README.md) has the short version.

> This release is a reference design. It is not a production-ready solution.

## Start here

<table align="center">
  <thead>
    <tr><th>Document</th><th>What it covers</th></tr>
  </thead>
  <tbody>
    <tr><td><a href="01-introduction-and-scope.md">Introduction and Scope</a></td><td>What the Sensor Link is, what this release delivers, and what is in and out of scope</td></tr>
    <tr><td><a href="02-architecture.md">Architecture</a></td><td>End-to-end data path and the functional block diagrams</td></tr>
    <tr><td><a href="03-bill-of-materials.md">Bill of Materials</a></td><td>Equipment needed to run this release (the SOF files and a link to the host control library are in the <a href="../README.md#provided-fpga-binaries-sof">top-level README</a>)</td></tr>
    <tr><td><a href="04-getting-started-1-hardware-connection.md">Getting Started: Hardware Connection</a></td><td>Hardware configurations, sensor wiring, and the lidar cable rework</td></tr>
    <tr><td><a href="05-getting-started-2-configure-the-board.md">Getting Started: Configure the Board</a></td><td>Programming the SOF</td></tr>
    <tr><td><a href="06-getting-started-3-execute-the-control-plane.md">Getting Started: Execute the Control Plane</a></td><td>Running the Altera® Control Link and starting the streams</td></tr>
    <tr><td><a href="07-data-formats.md">Data Formats</a></td><td>RoCEv2 stream layout for video, 2D lidar, 3D lidar, and IMU, plus the timestamp format</td></tr>
    <tr><td><a href="08-fpga-system-description.md">FPGA System Description</a></td><td>Platform Designer and/or Visual Designer Studio subsystems and how they connect</td></tr>
    <tr><td><a href="09-altera-control-link.md">Altera® Control Link</a></td><td>The UDP register-access protocol and the host configuration sequence</td></tr>
    <tr><td><a href="10-terminology-and-references.md">Terminology and References</a></td><td>Glossary and external references</td></tr>
    <tr><td><a href="appendix-a-control-memory-map.md">Appendix A: Control Subsystem Memory Map</a></td><td>Base addresses and register tables for every IP the host can reach</td></tr>
  </tbody>
</table>

<br>

## Subsystem pages

These pages describe each Platform Designer and/or Visual Designer Studio subsystem in the FPGA design. They are linked from [FPGA System Description](08-fpga-system-description.md).

<table align="center">
  <thead>
    <tr><th>Subsystem</th><th>Page</th></tr>
  </thead>
  <tbody>
    <tr><td>MIPI In</td><td><a href="subsystems/01-mipi-in-subsystem.md">mipi-in-subsystem</a></td></tr>
    <tr><td>ISP In</td><td><a href="subsystems/02-isp-in-subsystem.md">isp-in-subsystem</a></td></tr>
    <tr><td>ISP Lite Pipe 0</td><td><a href="subsystems/03-isp-lite-pipe-0.md">isp-lite-pipe-0</a></td></tr>
    <tr><td>ISP Lite Pipe 1</td><td><a href="subsystems/04-isp-lite-pipe-1.md">isp-lite-pipe-1</a></td></tr>
    <tr><td>Video TPG 0</td><td><a href="subsystems/05-video-tpg-0.md">video-tpg-0</a></td></tr>
    <tr><td>Video TPG 1</td><td><a href="subsystems/06-video-tpg-1.md">video-tpg-1</a></td></tr>
    <tr><td>Stream Switch</td><td><a href="subsystems/07-stream-switch-subsystem.md">stream-switch-subsystem</a></td></tr>
    <tr><td>Lidar Ingest</td><td><a href="subsystems/08-lidar-ingest-subsystem.md">lidar-ingest-subsystem</a></td></tr>
    <tr><td>Ethernet Ingest</td><td><a href="subsystems/09-ethernet-ingest-subsystem.md">ethernet-ingest-subsystem</a></td></tr>
    <tr><td>External GTS Reset</td><td><a href="subsystems/10-external-gts-reset.md">external-gts-reset</a></td></tr>
    <tr><td>Timestamp</td><td><a href="subsystems/11-timestamp-subsystem.md">timestamp-subsystem</a></td></tr>
    <tr><td>RoCEv2</td><td><a href="subsystems/12-rocev2-subsystem.md">rocev2-subsystem</a></td></tr>
    <tr><td>10G Ethernet</td><td><a href="subsystems/13-10g-ethernet-subsystem.md">10g-ethernet-subsystem</a></td></tr>
    <tr><td>Control</td><td><a href="subsystems/14-control-subsystem.md">control-subsystem</a></td></tr>
  </tbody>
</table>

<br>

Images used by these pages are in [`images/`](images/).
