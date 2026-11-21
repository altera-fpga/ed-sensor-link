# ISP In Subsystem

The ISP In Subsystem chooses the Bayer source for the two ISP Lite pipes. The live inputs are the two 12-bit Bayer streams from the MIPI In Subsystem. The Switch IP routes three sources onto the two pipe inputs.

The third source is internal. The Test-Pattern Generator produces a video field without a camera, and the Remosaic IP converts that RGB image to a 2x2 Bayer stream so it matches the camera format. All three blocks are configured over Avalon-MM.

**IP in this subsystem**

<table align="center">
  <thead>
    <tr><th>IP</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Test-Pattern Generator</td><td>Produces one fixed test image per field, on request, so the ISP path can be exercised without a camera. Resolution, color space and pattern are runtime settings.</td></tr>
    <tr><td>Remosaic</td><td>Converts the test-pattern RGB image to 2x2 Bayer, which is the format the ISP Lite pipes expect from a camera.</td></tr>
    <tr><td>Switch</td><td>Crosspoint for the two camera streams and the remosaiced test pattern. Each of the two outputs is sent to one ISP Lite pipe.</td></tr>
  </tbody>
</table>

<br>

<p align="center">
  <img src="../images/figure-20-isp-in-subsystem-block-diagram.png" alt="ISP In Subsystem block diagram">
</p>

<p align="center"><strong>Figure 20. ISP In Subsystem block diagram.</strong></p>

<br>

---

[← MIPI In Subsystem](01-mipi-in-subsystem.md) | [Documentation index](../README.md) | [ISP Lite Pipe 0 →](03-isp-lite-pipe-0.md)
