# Video TPG 1

Video TPG 1 is the synthetic source for Stream Switch input 1. The IP is the same as Video TPG 0, with its own throttle registers.

**IP in this subsystem**

<table align="center">
  <thead>
    <tr><th>IP</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Test-Pattern Generator</td><td>Same function as Video TPG 0. No control agent.</td></tr>
    <tr><td>Video Rate Control</td><td>Same throttle function as Video TPG 0, with its own Avalon-MM agent.</td></tr>
  </tbody>
</table>

<br>

<p align="center">
  <img src="../images/figure-24-video-tpg-1-block-diagram.png" alt="Video TPG 1 block diagram">
</p>

<p align="center"><strong>Figure 24. Video TPG 1 block diagram.</strong></p>

<br>

---

[← Video TPG 0](05-video-tpg-0.md) | [Documentation index](../README.md) | [Stream Switch Subsystem →](07-stream-switch-subsystem.md)
