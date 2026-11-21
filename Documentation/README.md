# Altera® Sensor Link Documentation

Altera® Sensor Link is an edge platform that ingests camera, lidar, and IMU data on one Agilex™ 5 FPGA and delivers it to a host as RoCEv2 streams over 10 GbE. This folder holds the full design documentation. The [top-level README](../README.md) has the short version.

> This release is a proof of concept. It is not a production-ready solution.

## Start here

| Document | What it covers |
| --- | --- |
| [Introduction](01-introduction.md) | What the Sensor Link is and what this release delivers |
| [Scope](02-scope.md) | What is in and out of scope |
| [Architecture](03-architecture.md) | End-to-end data path and the functional block diagrams |
| [Deliverables](04-deliverables.md) | Bill of materials, the provided SOF files, and the host control library |
| [Getting Started](05-getting-started.md) | Wiring, PTP, programming the board, and starting the streams |
| [Data Formats](06-data-formats.md) | RoCEv2 stream layout for video, 2D lidar, 3D lidar, and IMU, plus the timestamp format |
| [FPGA System Description](07-fpga-system-description.md) | Platform Designer subsystems and how they connect |
| [Altera Control Link](08-altera-control-link.md) | The UDP register-access protocol and the host configuration sequence |
| [Terminology and References](09-terminology-and-references.md) | Glossary and external references |
| [Appendix A: Nios V/g Control Memory Map](appendix-a-control-memory-map.md) | Base addresses and register tables for every IP the host can reach |

## Subsystem pages

These pages describe each Platform Designer subsystem in the FPGA design. They are linked from [FPGA System Description](07-fpga-system-description.md).

| Subsystem | Page |
| --- | --- |
| MIPI In | [mipi-in-subsystem](subsystems/01-mipi-in-subsystem.md) |
| ISP In | [isp-in-subsystem](subsystems/02-isp-in-subsystem.md) |
| ISP Lite Pipe 0 | [isp-lite-pipe-0](subsystems/03-isp-lite-pipe-0.md) |
| ISP Lite Pipe 1 | [isp-lite-pipe-1](subsystems/04-isp-lite-pipe-1.md) |
| Video TPG 0 | [video-tpg-0](subsystems/05-video-tpg-0.md) |
| Video TPG 1 | [video-tpg-1](subsystems/06-video-tpg-1.md) |
| Stream Switch | [stream-switch-subsystem](subsystems/07-stream-switch-subsystem.md) |
| Lidar Ingest | [lidar-ingest-subsystem](subsystems/08-lidar-ingest-subsystem.md) |
| Ethernet Ingest | [ethernet-ingest-subsystem](subsystems/09-ethernet-ingest-subsystem.md) |
| External GTS Reset | [external-gts-reset](subsystems/10-external-gts-reset.md) |
| Timestamp | [timestamp-subsystem](subsystems/11-timestamp-subsystem.md) |
| RoCEv2 | [rocev2-subsystem](subsystems/12-rocev2-subsystem.md) |
| 10G Ethernet | [10g-ethernet-subsystem](subsystems/13-10g-ethernet-subsystem.md) |
| Control | [control-subsystem](subsystems/14-control-subsystem.md) |

Images used by these pages are in [`images/`](images/).
