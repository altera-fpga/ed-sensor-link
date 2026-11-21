# Bill of Materials

The following lists the equipment needed to run this release. The provided SOF files and the link to the host control library are in the [top-level README](../README.md#provided-fpga-binaries-sof).

<table align="center">
  <thead>
    <tr><th>Item</th><th>Qty</th><th>Link</th><th>Comment</th></tr>
  </thead>
  <tbody>
    <tr><td>MK-A5E065BB32AES1: Agilex™ 5 FPGA and SoC E-Series Modular Development Kit (ES).<br>Or<br>MK-A5E065BB32AEA: Agilex™ 5 FPGA and SoC E-Series 065B Modular Development Kit (Group B Device)</td><td>1</td><td><a href="https://www.altera.com/products/devkit/po-3001/agilex-5-fpga-and-soc-e-series-modular-development-kit-es">link</a><br>or<br><a href="https://www.altera.com/products/devkit/po-3274/agilex-5-fpga-and-soc-e-series-065b-modular-development-kit">link</a></td><td>“ES” version selected due to board availability<br>065B (Group B Device) production kit also supported.</td></tr>
    <tr><td>Framos FSM:GO IMX678C Camera Modules</td><td>2</td><td><a href="https://www.framos.com/en/fsmgo">link</a><br><a href="https://www.mouser.co.uk/en/c/?q=FRAMOS%20FSM%3AGO-IMX678C">link (Mouser)</a></td><td>Select the appropriate lens</td></tr>
    <tr><td>Slamtec RPLIDAR A1M8 2D 360 Degree 12 Meters Scanning Radius LIDAR Sensor Scanner</td><td>1</td><td><a href="https://www.slamtec.com/en/lidar/a1">link</a></td><td>2D lidar over UART. Requires cable rework.</td></tr>
    <tr><td>150mm Flex/Ribbon Cables for Framos Modules</td><td>2</td><td><a href="https://www.mouser.co.uk/ProductDetail/FRAMOS/FMA-FC-150-60-V1A?qs=GedFDFLaBXGCmWApKt5QIQ%3D%3D&_gl=1*d93qim*_ga*MTkyOTE4MjMxNy4xNzQxMTcwMzQy*_ga_15W4STQT4T*MTc0MTE3MDM0Mi4xLjEuMTc0MTE3MDQ5OS40NS4wLjA">link</a></td><td>For direct connection to the board</td></tr>
    <tr><td>(Optional) Framos GMSL3 5m modules (Including Power Supply)</td><td>1</td><td><a href="https://www.mouser.co.uk/ProductDetail/FRAMOS/FFA-GMSL3-Kit-5m?qs=%252BHhoWzUJg4IkLHv%2F6fzsXQ%3D%3D">link</a></td><td>Connect optionally only to MIPI0</td></tr>
    <tr><td>Lidar Power Supply</td><td>1</td><td><a href="https://www.amazon.co.uk/dp/B09SG46W44">link</a></td><td>Needed to power the lidar, because the board cannot supply 5 V</td></tr>
    <tr><td>Additional Cabling for Lidar (see <a href="04-getting-started-1-hardware-connection.md#lidar-cable-rework">Lidar cable rework</a>)</td><td>-</td><td>-</td><td>-</td></tr>
    <tr><td>Livox MID-360S Lidar + Cabling</td><td>1</td><td><a href="https://www.livoxtech.com/mid-360s">link</a></td><td>3D lidar over Ethernet (with IMU)</td></tr>
    <tr><td>Host 10 GbE network interface card</td><td>1</td><td>-</td><td>Connects the host to the board's SFP+ link</td></tr>
    <tr><td>SFP+ module and cable</td><td>1</td><td>-</td><td>Connects the development kit's SFP+ cage to the host network card</td></tr>
    <tr><td>USB cable for JTAG programming</td><td>1</td><td>-</td><td>Connects the development kit to the host for the Quartus Programmer</td></tr>
  </tbody>
</table>

<br>

---

[← Architecture](02-architecture.md) | [Documentation index](README.md) | [Getting Started: Hardware Connection →](04-getting-started-1-hardware-connection.md)
