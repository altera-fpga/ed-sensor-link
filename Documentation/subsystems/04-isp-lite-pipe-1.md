# ISP Lite Pipe 1

ISP Lite Pipe 1 is the same chain for channel 1. Its output feeds Stream Switch input 3. Clocking matches Pipe 0: 297 MHz through the processing stages, then 148.5 MHz after the stream FIFO.

The IP and the order are the same as Pipe 0, with a separate Avalon-MM register space so the two pipes are programmed independently. See [Appendix A](../appendix-a-control-memory-map.md) for the Pipe 1 base addresses.

**IP in this subsystem**

<table align="center">
  <thead>
    <tr><th>IP</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Black Level Correction</td><td>Same function as Pipe 0, on channel 1.</td></tr>
    <tr><td>White Balance Correction</td><td>Same function as Pipe 0, on channel 1.</td></tr>
    <tr><td>Demosaic</td><td>Same function as Pipe 0, on channel 1.</td></tr>
    <tr><td>Color Space Converter (color matrix)</td><td>Same function as Pipe 0, with its own coefficients.</td></tr>
    <tr><td>Scaler</td><td>Same function as Pipe 0, with its own output size.</td></tr>
    <tr><td>Bits per Color Sample Adapter</td><td>Same function as Pipe 0. No control agent.</td></tr>
    <tr><td>Color Space Converter (RGB to YCbCr)</td><td>Same function as Pipe 0. No control agent.</td></tr>
    <tr><td>Chroma Resampler</td><td>Same function as Pipe 0, with its own sampling settings.</td></tr>
    <tr><td>Video Streaming FIFO</td><td>Same 297 MHz to 148.5 MHz crossing as Pipe 0.</td></tr>
  </tbody>
</table>

<br>

<p align="center">
  <img src="../images/figure-22-isp-lite-pipe-1-block-diagram.png" alt="ISP Lite Pipe 1 block diagram">
</p>

<p align="center"><strong>Figure 22. ISP Lite Pipe 1 block diagram.</strong></p>

<br>

---

[← ISP Lite Pipe 0](03-isp-lite-pipe-0.md) | [Documentation index](../README.md) | [Video TPG 0 →](05-video-tpg-0.md)
