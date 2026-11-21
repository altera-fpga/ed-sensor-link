# Bill of Materials

The following lists the equipment needed to run this release. The provided SOF files and the link to the host control library are in the [top-level README](../README.md#provided-sof).

| Item | Qty | Link | Comment |
| --- | --- | --- | --- |
| MK-A5E065BB32AES1: Agilex™ 5 FPGA and SoC E-Series Modular Development Kit (ES).<br>Or<br>MK-A5E065BB32AEA: Agilex® 5 FPGA and SoC E-Series 065B Modular Development Kit (Group B Device) | 1 | [link](https://www.altera.com/products/devkit/po-3001/agilex-5-fpga-and-soc-e-series-modular-development-kit-es)<br>or<br>[link](https://www.altera.com/products/devkit/po-3274/agilex-5-fpga-and-soc-e-series-065b-modular-development-kit) | “ES” version selected due to board availability<br>Production board supported. |
| Framos FSM:GO IMX678C Camera Modules | 2 | [link](https://www.framos.com/en/fsmgo)<br>[link2](https://www.mouser.co.uk/en/c/?q=FRAMOS%20FSM%3AGO-IMX678C) | Select the appropriate lens |
| Slamtec RPLIDAR A1M8 2D 360 Degree 12 Meters Scanning Radius LIDAR Sensor Scanner | 1 | [link](https://www.amazon.com/Slamtec-RPLIDAR-Scanning-Avoidance-Navigation/dp/B07TJW5SXF) | 2D lidar over UART. Requires cable rework. |
| 150mm Flex/Ribbon Cables for Framos Modules | 2 | [link](https://www.mouser.co.uk/ProductDetail/FRAMOS/FMA-FC-150-60-V1A?qs=GedFDFLaBXGCmWApKt5QIQ%3D%3D&_gl=1*d93qim*_ga*MTkyOTE4MjMxNy4xNzQxMTcwMzQy*_ga_15W4STQT4T*MTc0MTE3MDM0Mi4xLjEuMTc0MTE3MDQ5OS40NS4wLjA) | For direct connection to the board |
| (Optional) Framos GMSL3 5m modules (Including Power Supply) | 1 | [link](https://www.mouser.co.uk/ProductDetail/FRAMOS/FFA-GMSL3-Kit-5m?qs=%252BHhoWzUJg4IkLHv%2F6fzsXQ%3D%3D) | Connect optionally only to MIPI0 |
| Lidar Power Supply | 1 | [link](https://www.amazon.co.uk/dp/B09SG46W44) | Needed to provide power to the Lidar as the board cannot supply 5V |
| Additional Cabling for Lidar (see [Lidar cable re-work](05-getting-started-1-hardware-connection.md#lidar-cable-re-work)) | - | - | - |
| Livox MID-360S Lidar + Cabling | 1 | [link](https://www.livoxtech.com/mid-360s) | 3D Lidar Over Ethernet (+IMU) |

---

[← Architecture](03-architecture.md) | [Documentation index](README.md) | [Getting Started: Hardware Connection →](05-getting-started-1-hardware-connection.md)
