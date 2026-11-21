# ISP Lite Pipe 0

ISP Lite Pipe 0 processes channel 0 from the ISP In Subsystem and drives Stream Switch input 2. The processing stages run at 297 MHz. The Video Streaming FIFO then crosses the stream into the 148.5 MHz domain. The Avalon-MM fabric crosses from the 100 MHz control clock.

The functions below are the roles of these Video and Vision Processing Suite IP blocks in this pipe. Black-level correction, white balance, demosaic, the color-matrix converter, the scaler and the chroma resampler have control agents. The bits-per-sample adapter, the RGB-to-YCbCr converter and the stream FIFO do not. Register addresses are in [Appendix A](../appendix-a-control-memory-map.md).

**IP in this subsystem**

<table align="center">
  <thead>
    <tr><th>IP</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Black Level Correction</td><td>Subtracts the sensor pedestal from a 2x2 Bayer image and stretches the result back across the full sample range, so the darkest code represents black.</td></tr>
    <tr><td>White Balance Correction</td><td>Applies per-channel gains on the 2x2 Bayer image so a white object is not tinted by the illuminant or by the color-filter response.</td></tr>
    <tr><td>Demosaic</td><td>Reconstructs an RGB sample at every pixel by interpolating the missing colors from the Bayer neighbors.</td></tr>
    <tr><td>Color Space Converter (color matrix)</td><td>A Color Space Converter used as the RGB color-correction matrix. Coefficients can be changed at run time.</td></tr>
    <tr><td>Scaler</td><td>Resizes the field. In this pipe it is the downscale stage. The input size is set through the register map.</td></tr>
    <tr><td>Bits per Color Sample Adapter</td><td>Matches the number of bits in each color sample between the scaler and the following converter. Narrower outputs drop least-significant bits; wider outputs pad zeros.</td></tr>
    <tr><td>Color Space Converter (RGB to YCbCr)</td><td>Transforms the RGB samples into YCbCr for the chroma resampler. This instance has no control agent.</td></tr>
    <tr><td>Chroma Resampler</td><td>Changes YCbCr chroma sampling. The IP supports 4:4:4, 4:2:2 and 4:2:0. In this pipe the output is 4:2:0.</td></tr>
    <tr><td>Video Streaming FIFO</td><td>Buffers the stream and crosses it from 297 MHz to 148.5 MHz. It holds the input when full so samples are not dropped.</td></tr>
  </tbody>
</table>

<br>

<p align="center">
  <img src="../images/figure-21-isp-lite-pipe-0-block-diagram.png" alt="ISP Lite Pipe 0 block diagram">
</p>

<p align="center"><strong>Figure 21. ISP Lite Pipe 0 block diagram.</strong></p>

<br>

---

[← ISP In Subsystem](02-isp-in-subsystem.md) | [Documentation index](../README.md) | [ISP Lite Pipe 1 →](04-isp-lite-pipe-1.md)
