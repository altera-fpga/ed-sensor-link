# Appendix A: Control Subsystem Memory Map

Addresses in this section are byte addresses in the Control Subsystem Nios V data space. The processor reaches the other subsystems through the Avalon-MM export bridge, so these bases are already the addresses software uses. The register address is the IP base plus the offset in the register table.

Altera® VVP video IP shares the register block below. Core-specific registers are listed with the first subsystem that uses that core. A later instance of the same core uses the same offsets at its own base address.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x000</td><td>Vendor and product ID</td><td>Read-only vendor ID in [31:16] and product ID in [15:0]</td></tr>
    <tr><td>0x004</td><td>Version</td><td>IP version and register-map version</td></tr>
    <tr><td>0x008</td><td>Compile-time parameters</td><td>Read-only core parameters. Core-specific parameters follow, through 0x0FC</td></tr>
    <tr><td>0x100</td><td>Interrupt control</td><td>Enables for the core interrupt sources</td></tr>
    <tr><td>0x104</td><td>Interrupt status</td><td>Pending core interrupts</td></tr>
    <tr><td>0x120</td><td>Image width</td><td>Active width, in pixels, of the current field</td></tr>
    <tr><td>0x124</td><td>Image height</td><td>Active height, in lines, of the current field</td></tr>
    <tr><td>0x128</td><td>Interlace</td><td>Interlace mode of the current field</td></tr>
    <tr><td>0x12C</td><td>Bits per sample</td><td>Sample precision of the current field</td></tr>
    <tr><td>0x130</td><td>Color space</td><td>Color space of the current field</td></tr>
    <tr><td>0x134</td><td>Subsampling</td><td>Chroma subsampling of the current field</td></tr>
    <tr><td>0x138</td><td>Cositing</td><td>Chroma sample position of the current field</td></tr>
    <tr><td>0x13C</td><td>Field count</td><td>Read-only field counter, full mode only</td></tr>
    <tr><td>0x140</td><td>Run-time base</td><td>First core-specific run-time register. Later offsets are listed with each core</td></tr>
  </tbody>
</table>

<br>

## Control Subsystem (Internal)

These slaves are inside the Control Subsystem. CPU memory, the debug module and the descriptor memory are memory windows, not control registers.

<table align="center">
  <thead>
    <tr><th>IP</th><th>Base address</th><th>Span</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>CPU memory</td><td>0x00000000</td><td>1 MB</td><td>Nios V instruction and data memory</td></tr>
    <tr><td>CPU debug module</td><td>0x00200000</td><td>64 KB</td><td>Debug-module access for the JTAG debugger</td></tr>
    <tr><td>Shared descriptor memory</td><td>0x00210000</td><td>8 KB</td><td>On-chip memory shared with the message DMA</td></tr>
    <tr><td>CPU timer</td><td>0x00212400</td><td>64 bytes</td><td>Nios V machine timer</td></tr>
    <tr><td>mSGDMA TX prefetcher</td><td>0x00212440</td><td>32 bytes</td><td>Fetches transmit descriptors from memory</td></tr>
    <tr><td>mSGDMA TX</td><td>0x00212460</td><td>32 bytes</td><td>Reads non-RoCE packets from memory</td></tr>
    <tr><td>mSGDMA RX prefetcher</td><td>0x00212480</td><td>32 bytes</td><td>Fetches receive descriptors from memory</td></tr>
    <tr><td>mSGDMA RX</td><td>0x002124A0</td><td>32 bytes</td><td>Writes received non-RoCE packets into memory</td></tr>
    <tr><td>System ID register block</td><td>0x002124D0</td><td>8 bytes</td><td>Hardware identification</td></tr>
    <tr><td>Processor JTAG UART</td><td>0x002124D8</td><td>8 bytes</td><td>Processor console</td></tr>
    <tr><td>Control input PIO</td><td>0x00212500</td><td>16 bytes</td><td>4-bit input with rising-edge capture</td></tr>
    <tr><td>Control output PIO</td><td>0x00212520</td><td>16 bytes</td><td>4-bit output</td></tr>
  </tbody>
</table>

<br>

PIO registers. Control input PIO uses data, interrupt mask and edge capture. Control output PIO uses data.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x00</td><td>Data</td><td>Input pins, output pins, or both, depending on how the PIO was generated</td></tr>
    <tr><td>0x04</td><td>Direction</td><td>Bit value 1 drives the pin as an output. Present on bidirectional PIOs</td></tr>
    <tr><td>0x08</td><td>Interrupt mask</td><td>Per-bit interrupt enable. Present when the PIO has an interrupt</td></tr>
    <tr><td>0x0C</td><td>Edge capture</td><td>Latched edge event. Write 1 to a bit to clear it. Present when edge capture is enabled</td></tr>
    <tr><td>0x10</td><td>Set bits</td><td>Write 1 to set the corresponding output bit</td></tr>
    <tr><td>0x14</td><td>Clear bits</td><td>Write 1 to clear the corresponding output bit</td></tr>
  </tbody>
</table>

<br>

## MIPI In Subsystem

<table align="center">
  <thead>
    <tr><th>IP</th><th>Base address</th><th>Span</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>MIPI D-PHY</td><td>0x00228000</td><td>4 KB</td><td>Dual-link MIPI D-PHY, both camera links</td></tr>
    <tr><td>Bidirectional MIPI input PIO</td><td>0x00229000</td><td>16 bytes</td><td>32-bit bidirectional PIO</td></tr>
    <tr><td>Input-only MIPI input PIO</td><td>0x00229010</td><td>16 bytes</td><td>32-bit input PIO</td></tr>
    <tr><td>Camera I2C controller</td><td>0x00229200</td><td>64 bytes</td><td>Camera I2C controller</td></tr>
    <tr><td>Board I2C controller</td><td>0x00229300</td><td>64 bytes</td><td>Board I2C controller</td></tr>
    <tr><td>Pixel-packing converter for camera link 0</td><td>0x00229600</td><td>512 bytes</td><td>Pixel packing for camera link 0</td></tr>
    <tr><td>Pixel-packing converter for camera link 1</td><td>0x00229800</td><td>512 bytes</td><td>Pixel packing for camera link 1</td></tr>
    <tr><td>CSI-2 receiver for camera link 0</td><td>0x0022A000</td><td>4 KB</td><td>CSI-2 receiver for camera link 0</td></tr>
    <tr><td>CSI-2 receiver for camera link 1</td><td>0x0022B000</td><td>4 KB</td><td>CSI-2 receiver for camera link 1</td></tr>
  </tbody>
</table>

<br>

Both I2C controllers use this map. The camera I2C controller is at 0x00229200 and the board I2C controller is at 0x00229300.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x00</td><td>Transfer command</td><td>Start, stop, 7-bit address and the read/write data bit</td></tr>
    <tr><td>0x04</td><td>Receive data</td><td>Oldest byte in the receive FIFO</td></tr>
    <tr><td>0x08</td><td>Control</td><td>Controller enable, bus speed and FIFO thresholds</td></tr>
    <tr><td>0x0C</td><td>Interrupt enable</td><td>Enables for receive overflow, arbitration loss and transfer events</td></tr>
    <tr><td>0x10</td><td>Interrupt status</td><td>Pending I2C interrupts. Write 1 to clear</td></tr>
    <tr><td>0x14</td><td>Status</td><td>Bus busy and FIFO state</td></tr>
    <tr><td>0x18</td><td>Command FIFO level</td><td>Number of entries in the transfer-command FIFO</td></tr>
    <tr><td>0x1C</td><td>Receive FIFO level</td><td>Number of bytes in the receive FIFO</td></tr>
    <tr><td>0x20</td><td>SCL low count</td><td>SCL low period, in clock cycles</td></tr>
    <tr><td>0x24</td><td>SCL high count</td><td>SCL high period, in clock cycles</td></tr>
    <tr><td>0x28</td><td>SDA hold</td><td>SDA hold time, in clock cycles</td></tr>
  </tbody>
</table>

<br>

Both pixel-packing converters use the VVP common map plus these registers. The camera link 0 converter is at 0x00229600 and the camera link 1 converter is at 0x00229800.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x008</td><td>lite mode</td><td><code>Lite_mode</code> register</td></tr>
    <tr><td>0x00C</td><td>debug enabled</td><td><code>Debug_enabled</code> register</td></tr>
    <tr><td>0x140</td><td>status</td><td>Run-time status register</td></tr>
  </tbody>
</table>

<br>

The two PIO instances use the PIO map above. The first is bidirectional, so it also has the direction register. The second is input only. The D-PHY and the two CSI-2 receivers occupy the 4 KB windows in the table. Their bit fields are the Altera® MIPI D-PHY and MIPI CSI-2 register files and are not reproduced here.

## ISP In Subsystem

<table align="center">
  <thead>
    <tr><th>IP</th><th>Base address</th><th>Span</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Internal Bayer test-pattern generator</td><td>0x0022E400</td><td>512 bytes</td><td>Internal Bayer test-pattern generator</td></tr>
    <tr><td>Internal test-pattern remosaic</td><td>0x0022E600</td><td>256 bytes</td><td>Remosaic of the internal test pattern</td></tr>
    <tr><td>Bayer source switch</td><td>0x0022E800</td><td>512 bytes</td><td>Selects the Bayer source for each ISP Lite pipe</td></tr>
  </tbody>
</table>

<br>

Test-pattern generator at 0x0022E400. Offsets are added to that base, and the VVP common registers are also present.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x008</td><td>lite mode</td><td><code>Lite_mode</code> register</td></tr>
    <tr><td>0x00C</td><td>debug enabled</td><td><code>Debug_enabled</code> register</td></tr>
    <tr><td>0x010</td><td>num patterns</td><td>Number of patterns register</td></tr>
    <tr><td>0x014</td><td>ctrl in enabled</td><td><code>Ctrl_in_enabled</code> register</td></tr>
    <tr><td>0x018</td><td>bps</td><td>Bits per color sample register</td></tr>
    <tr><td>0x01C</td><td>pip</td><td>Pixels per beat register</td></tr>
    <tr><td>0x020</td><td>pattern base</td><td>First pattern register</td></tr>
    <tr><td>0x020</td><td>pattern 0 type</td><td>Pattern 0 type identifier</td></tr>
    <tr><td>0x024</td><td>pattern 0 color</td><td>Pattern 0 <code>color_space</code>/<code>color_sampling</code> identifier</td></tr>
    <tr><td>0x028</td><td>pattern 1 type</td><td>Pattern 1 type identifier</td></tr>
    <tr><td>0x02C</td><td>pattern 1 color</td><td>Pattern 1 <code>color_space</code>/<code>color_sampling</code> identifier</td></tr>
    <tr><td>0x030</td><td>pattern 2 type</td><td>Pattern 2 type identifier</td></tr>
    <tr><td>0x034</td><td>pattern 2 color</td><td>Pattern 2 <code>color_space</code>/<code>color_sampling</code> identifier</td></tr>
    <tr><td>0x038</td><td>pattern 3 type</td><td>Pattern 3 type identifier</td></tr>
    <tr><td>0x03C</td><td>pattern 3 color</td><td>Pattern 3 <code>color_space</code>/<code>color_sampling</code> identifier</td></tr>
    <tr><td>0x040</td><td>pattern 4 type</td><td>Pattern 4 type identifier</td></tr>
    <tr><td>0x044</td><td>pattern 4 color</td><td>Pattern 4 <code>color_space</code>/<code>color_sampling</code> identifier</td></tr>
    <tr><td>0x048</td><td>pattern 5 type</td><td>Pattern 5 type identifier</td></tr>
    <tr><td>0x04C</td><td>pattern 5 color</td><td>Pattern 5 <code>color_space</code>/<code>color_sampling</code> identifier</td></tr>
    <tr><td>0x050</td><td>pattern 6 type</td><td>Pattern 6 type identifier</td></tr>
    <tr><td>0x054</td><td>pattern 6 color</td><td>Pattern 6 <code>color_space</code>/<code>color_sampling</code> identifier</td></tr>
    <tr><td>0x058</td><td>pattern 7 type</td><td>Pattern 7 type identifier</td></tr>
    <tr><td>0x05C</td><td>pattern 7 color</td><td>Pattern 7 <code>color_space</code>/<code>color_sampling</code> identifier</td></tr>
    <tr><td>0x140</td><td>status</td><td>Run-time status register</td></tr>
    <tr><td>0x144</td><td>field count</td><td>Frame/field counter since last change (read-only)</td></tr>
    <tr><td>0x148</td><td>control</td><td>Control register (start/stop)</td></tr>
    <tr><td>0x14C</td><td>commit</td><td>Commit Settings register (commit pending changes in full mode)</td></tr>
    <tr><td>0x150</td><td>pattern select</td><td>Pattern selection register</td></tr>
    <tr><td>0x154</td><td>pre field ctrl pkts</td><td>Number of aux/user packets from the control input to be inserted before an output field image info packet (full mode)</td></tr>
    <tr><td>0x158</td><td>post field ctrl pkts</td><td>Number of aux/user packets from the control input to be inserted before an output field end-of-field packet (full mode)</td></tr>
    <tr><td>0x15C</td><td>c0</td><td>C0 color sample (Cb or B) for the patterns that require it</td></tr>
    <tr><td>0x160</td><td>c1</td><td>C1 color sample (Y or G) for the patterns that require it</td></tr>
    <tr><td>0x164</td><td>c2</td><td>C2 color sample (Cr or R) for the patterns that require it</td></tr>
    <tr><td>0x168</td><td>bars select</td><td>Bars-style selection when the color bars pattern is enabled</td></tr>
    <tr><td>0x16C</td><td>zone x origin</td><td>X part of zone plate origin</td></tr>
    <tr><td>0x170</td><td>zone y origin</td><td>Y part of zone plate origin</td></tr>
    <tr><td>0x174</td><td>zone scaling factor</td><td>Coarse scaling factor of zone plate size</td></tr>
    <tr><td>0x178</td><td>zone fine tune factor</td><td>Fine tune scaling factor of zone plate size</td></tr>
  </tbody>
</table>

<br>

Source switch at 0x0022E800.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x008</td><td>lite mode</td><td><code>Lite_mode</code> register</td></tr>
    <tr><td>0x00C</td><td>debug enabled</td><td><code>Debug_enabled</code> register</td></tr>
    <tr><td>0x010</td><td>uninterrupted inputs</td><td>Uninterrupted inputs register</td></tr>
    <tr><td>0x014</td><td>auto consume</td><td><code>Auto_consume</code> register</td></tr>
    <tr><td>0x018</td><td>num inputs</td><td><code>Num_inputs</code> register</td></tr>
    <tr><td>0x01C</td><td>num outputs</td><td><code>Num_outputs</code> register</td></tr>
    <tr><td>0x020</td><td>use treadies</td><td>Use treadies register</td></tr>
    <tr><td>0x024</td><td>crash switch</td><td>Crash switching register</td></tr>
    <tr><td>0x140</td><td>status</td><td>Status register</td></tr>
    <tr><td>0x144</td><td>commit</td><td>Commit settings register</td></tr>
  </tbody>
</table>

<br>

The remosaic core at 0x0022E600 uses the VVP common register map. The core-specific registers of the remosaic core are not listed here.

## ISP Lite Pipe 0

Every core below also contains the VVP common registers.

<table align="center">
  <thead>
    <tr><th>IP</th><th>Base address</th><th>Span</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Color-matrix converter</td><td>0x0022D000</td><td>512 bytes</td><td>Color-matrix correction</td></tr>
    <tr><td>Demosaic block</td><td>0x0022D200</td><td>512 bytes</td><td>Demosaic</td></tr>
    <tr><td>White-balance block</td><td>0x0022D400</td><td>512 bytes</td><td>White-balance correction</td></tr>
    <tr><td>Black-level block</td><td>0x0022D600</td><td>512 bytes</td><td>Black-level correction</td></tr>
    <tr><td>Downscale scaler</td><td>0x0022D800</td><td>512 bytes</td><td>Scale from 4K to 1920 x 1080</td></tr>
    <tr><td>Chroma resampler</td><td>0x0022DA00</td><td>512 bytes</td><td>Chroma resampling to YUV 4:2:0</td></tr>
  </tbody>
</table>

<br>

Black-level correction at 0x0022D600.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x008</td><td>lite mode</td><td><code>Lite_mode</code> register</td></tr>
    <tr><td>0x00C</td><td>debug enabled</td><td><code>Debug_enabled</code> register</td></tr>
    <tr><td>0x010</td><td>bps in</td><td><code>Bps_in</code> register</td></tr>
    <tr><td>0x014</td><td>bps out</td><td><code>Bps_out</code> register</td></tr>
    <tr><td>0x018</td><td>num color in</td><td><code>Num_color_in</code> register</td></tr>
    <tr><td>0x01C</td><td>num color out</td><td><code>Num_color_out</code> register</td></tr>
    <tr><td>0x020</td><td>pip</td><td>Pip register</td></tr>
    <tr><td>0x024</td><td>max width</td><td>Max width register</td></tr>
    <tr><td>0x028</td><td>max height</td><td>Max height register</td></tr>
    <tr><td>0x02C</td><td>reflect around zero</td><td>Reflect Around Zero register</td></tr>
    <tr><td>0x140</td><td>status</td><td>Run-time read-only status register</td></tr>
    <tr><td>0x144</td><td>frame stats</td><td>Run-time read-only frame statistics register</td></tr>
    <tr><td>0x148</td><td>commit</td><td>Commit register (including lite mode)</td></tr>
    <tr><td>0x14C</td><td>settings</td><td>Settings register</td></tr>
    <tr><td>0x150</td><td>cfa 00 black</td><td>Cfa 00 black register</td></tr>
    <tr><td>0x154</td><td>cfa 00 color</td><td>Cfa 00 color register</td></tr>
    <tr><td>0x158</td><td>cfa 01 black</td><td>Cfa 01 black register</td></tr>
    <tr><td>0x15C</td><td>cfa 01 color</td><td>Cfa 01 color register</td></tr>
    <tr><td>0x160</td><td>cfa 10 black</td><td>Cfa 10 black register</td></tr>
    <tr><td>0x164</td><td>cfa 10 color</td><td>Cfa 10 color register</td></tr>
    <tr><td>0x168</td><td>cfa 11 black</td><td>Cfa 11 black register</td></tr>
    <tr><td>0x16C</td><td>cfa 11 color</td><td>Cfa 11 color register</td></tr>
  </tbody>
</table>

<br>

White-balance correction at 0x0022D400.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x008</td><td>lite mode</td><td><code>Lite_mode</code> register</td></tr>
    <tr><td>0x00C</td><td>debug enabled</td><td><code>Debug_enabled</code> register</td></tr>
    <tr><td>0x010</td><td>bps in</td><td><code>Bps_in</code> register</td></tr>
    <tr><td>0x014</td><td>bps out</td><td><code>Bps_out</code> register</td></tr>
    <tr><td>0x018</td><td>num color in</td><td><code>Num_color_in</code> register</td></tr>
    <tr><td>0x01C</td><td>num color out</td><td><code>Num_color_out</code> register</td></tr>
    <tr><td>0x020</td><td>pip</td><td>Pip register</td></tr>
    <tr><td>0x024</td><td>max width</td><td>Max width register</td></tr>
    <tr><td>0x028</td><td>max height</td><td>Max height register</td></tr>
    <tr><td>0x140</td><td>status</td><td>Run-time read-only status register</td></tr>
    <tr><td>0x144</td><td>frame stats</td><td>Run-time read-only frame statistics register</td></tr>
    <tr><td>0x148</td><td>commit</td><td>Commit register (including lite mode)</td></tr>
    <tr><td>0x14C</td><td>settings</td><td>Settings register</td></tr>
    <tr><td>0x150</td><td>cfa 00 color</td><td>Cfa 00 color register</td></tr>
    <tr><td>0x154</td><td>cfa 01 color</td><td>Cfa 01 color register</td></tr>
    <tr><td>0x158</td><td>cfa 10 color</td><td>Cfa 10 color register</td></tr>
    <tr><td>0x15C</td><td>cfa 11 color</td><td>Cfa 11 color register</td></tr>
  </tbody>
</table>

<br>

Demosaic at 0x0022D200.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x008</td><td>lite mode</td><td><code>Lite_mode</code> register</td></tr>
    <tr><td>0x00C</td><td>debug enabled</td><td><code>Debug_enabled</code> register</td></tr>
    <tr><td>0x010</td><td>bps in</td><td><code>Bps_in</code> register</td></tr>
    <tr><td>0x014</td><td>bps out</td><td><code>Bps_out</code> register</td></tr>
    <tr><td>0x018</td><td>num color in</td><td><code>Num_color_in</code> register</td></tr>
    <tr><td>0x01C</td><td>num color out</td><td><code>Num_color_out</code> register</td></tr>
    <tr><td>0x020</td><td>pip</td><td>Pip register</td></tr>
    <tr><td>0x024</td><td>max width</td><td>Max width register</td></tr>
    <tr><td>0x028</td><td>max height</td><td>Max height register</td></tr>
    <tr><td>0x140</td><td>status</td><td>Run-time read-only status register</td></tr>
    <tr><td>0x144</td><td>frame stats</td><td>Run-time read-only frame statistics register</td></tr>
    <tr><td>0x148</td><td>commit</td><td>Commit register (including lite mode)</td></tr>
    <tr><td>0x14C</td><td>settings</td><td>Settings register</td></tr>
  </tbody>
</table>

<br>

Color matrix at 0x0022D000.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x008</td><td>lite mode</td><td><code>Lite_mode</code> register</td></tr>
    <tr><td>0x00C</td><td>debug enabled</td><td><code>Debug_enabled</code> register</td></tr>
    <tr><td>0x010</td><td>bps in</td><td><code>Bps_in</code> register</td></tr>
    <tr><td>0x014</td><td>bps out</td><td><code>Bps_out</code> register</td></tr>
    <tr><td>0x018</td><td>frac bits</td><td><code>Frac_bits</code> register</td></tr>
    <tr><td>0x01C</td><td>coeffs signed</td><td><code>Coef_signed</code> register</td></tr>
    <tr><td>0x020</td><td>coeffs int bits</td><td><code>Coeff_int_bits</code> register</td></tr>
    <tr><td>0x024</td><td>summands signed</td><td><code>Summand_signed</code> register</td></tr>
    <tr><td>0x028</td><td>summands int bits</td><td><code>Summand_int_bits</code> register</td></tr>
    <tr><td>0x02C</td><td>binary point right move</td><td><code>Binary_point_right_move</code> register</td></tr>
    <tr><td>0x030</td><td>round method</td><td><code>Round_method</code> register</td></tr>
    <tr><td>0x144</td><td>commit</td><td>Commit settings register (full mode)</td></tr>
    <tr><td>0x178</td><td>output cs</td><td>Output color space register (full mode)</td></tr>
  </tbody>
</table>

<br>

Scaler at 0x0022D800.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x008</td><td>lite mode</td><td><code>Lite_mode</code> register</td></tr>
    <tr><td>0x00C</td><td>debug enabled</td><td><code>Debug_enabled</code> register</td></tr>
    <tr><td>0x010</td><td>pixels in parallel</td><td><code>Pixels_in_parallel</code> register</td></tr>
    <tr><td>0x014</td><td>max input width</td><td><code>Max_input_width</code> register</td></tr>
    <tr><td>0x018</td><td>max output width</td><td><code>Max_output_width</code> register</td></tr>
    <tr><td>0x01C</td><td>algorithm</td><td>Scaling algorithm register</td></tr>
    <tr><td>0x020</td><td>coeffs runtime load</td><td>Run-time coefficient loading register</td></tr>
    <tr><td>0x024</td><td>coeffs mem init</td><td>Coefficient memory initialization register</td></tr>
    <tr><td>0x028</td><td>v scaling enabled</td><td>Vertical scaling enabled register</td></tr>
    <tr><td>0x02C</td><td>v num taps</td><td>Vertical number of taps register</td></tr>
    <tr><td>0x030</td><td>v num phases</td><td>Vertical number of phases register</td></tr>
    <tr><td>0x034</td><td>v num banks</td><td>Vertical number of banks register</td></tr>
    <tr><td>0x038</td><td>v coeffs signed</td><td>Vertical <code>coef_signed</code> register</td></tr>
    <tr><td>0x03C</td><td>v coeffs int bits</td><td>Vertical coeff integer bits register</td></tr>
    <tr><td>0x040</td><td>v coeffs frac bits</td><td>Vertical coefficient fractional bits register</td></tr>
    <tr><td>0x044</td><td>h scaling enabled</td><td>Horizontal scaling enabled register</td></tr>
    <tr><td>0x048</td><td>h num taps</td><td>Horizontal number of taps register</td></tr>
    <tr><td>0x04C</td><td>h num phases</td><td>Horizontal number of phases register</td></tr>
    <tr><td>0x050</td><td>h num banks</td><td>Horizontal number of banks register</td></tr>
    <tr><td>0x054</td><td>h coeffs signed</td><td>Horizontal <code>coef_signed</code> register</td></tr>
    <tr><td>0x058</td><td>h coeffs int bits</td><td>Horizontal coeff integer bits register</td></tr>
    <tr><td>0x05C</td><td>h coeffs frac bits</td><td>Horizontal coefficient fractional bits register</td></tr>
    <tr><td>0x068</td><td>v partial scaling enabled</td><td>Vertical partial scaling enabled register</td></tr>
    <tr><td>0x06C</td><td>h partial scaling enabled</td><td>Horizontal partial scaling enabled register</td></tr>
    <tr><td>0x140</td><td>status</td><td>Run-time status register</td></tr>
    <tr><td>0x144</td><td>commit</td><td>Commit settings register (full mode)</td></tr>
    <tr><td>0x148</td><td>output width</td><td>Output width register</td></tr>
    <tr><td>0x14C</td><td>output height</td><td>Output height register</td></tr>
    <tr><td>0x150</td><td>h bank</td><td>Horizontal coefficient bank selection register</td></tr>
    <tr><td>0x154</td><td>v bank</td><td>Vertical coefficient bank selection register</td></tr>
    <tr><td>0x158</td><td>total in width</td><td>Total input width register when split horizontal scaling is enabled</td></tr>
    <tr><td>0x15C</td><td>total out width</td><td>Total output width register when split horizontal scaling is enabled</td></tr>
    <tr><td>0x160</td><td>h pos error</td><td>First pixel position error register when split horizontal scaling is enabled</td></tr>
    <tr><td>0x164</td><td>h phase offset</td><td>Phase offset register when split horizontal scaling is enabled</td></tr>
    <tr><td>0x168</td><td>h phase error</td><td>Phase offset error register when split horizontal scaling is enabled</td></tr>
    <tr><td>0x16C</td><td>h extended fill</td><td>Extended fill register. Indicates whether the partial input lines contain extended fill</td></tr>
    <tr><td>0x170</td><td>total in height</td><td>Total input height register when split vertical scaling is enabled</td></tr>
    <tr><td>0x174</td><td>total out height</td><td>Total output height register when split vertical scaling is enabled</td></tr>
    <tr><td>0x178</td><td>v pos error</td><td>First pixel position error register when split vertical scaling is enabled</td></tr>
    <tr><td>0x17C</td><td>v phase offset</td><td>Phase offset register when split vertical scaling is enabled</td></tr>
    <tr><td>0x180</td><td>v phase error</td><td>Phase offset error register when split vertical scaling is enabled</td></tr>
    <tr><td>0x184</td><td>v extended fill</td><td>Extended fill register. Indicates whether the partial input image contains extended fill</td></tr>
    <tr><td>0x200</td><td>rt coeff load bank select</td><td>Coefficient bank selection register (this is the bank that will be targeted by the next commit)</td></tr>
    <tr><td>0x204</td><td>rt coeff load phase select</td><td>Coefficient phase selection register (this is the phase that will be targeted by the next commit)</td></tr>
    <tr><td>0x208</td><td>rt coeff load commit</td><td>Coefficient commit register</td></tr>
    <tr><td>0x20C</td><td>rt coeff load tap base</td><td>Base register for the first tap coefficient, write <code>V_NUM_TAPS</code>/<code>H_NUM_TAPS</code> coefficients starting at this register</td></tr>
  </tbody>
</table>

<br>

Chroma resampler at 0x0022DA00.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x008</td><td>lite mode</td><td><code>Lite_mode</code> register</td></tr>
    <tr><td>0x00C</td><td>debug enabled</td><td><code>Debug_enabled</code> register</td></tr>
    <tr><td>0x010</td><td>enable 444 444</td><td>444 passthrough enabled</td></tr>
    <tr><td>0x014</td><td>enable 444 422</td><td>444-&gt;422 conversion enabled</td></tr>
    <tr><td>0x018</td><td>enable 444 420</td><td>444-&gt;420 conversion enabled</td></tr>
    <tr><td>0x01C</td><td>enable 422 444</td><td>422-&gt;444 conversion enabled</td></tr>
    <tr><td>0x020</td><td>enable 422 422</td><td>422 passthrough enabled</td></tr>
    <tr><td>0x024</td><td>enable 422 420</td><td>422-&gt;420 conversion enabled</td></tr>
    <tr><td>0x028</td><td>enable 420 444</td><td>420-&gt;444 conversion enabled</td></tr>
    <tr><td>0x02C</td><td>enable 420 422</td><td>420-&gt;422 conversion enabled</td></tr>
    <tr><td>0x030</td><td>enable 420 420</td><td>420 passthrough enabled</td></tr>
    <tr><td>0x140</td><td>status</td><td>Run-time status register</td></tr>
    <tr><td>0x144</td><td>commit</td><td>Commit settings register (full mode)</td></tr>
    <tr><td>0x148</td><td>output subsampling</td><td>Output subsampling selection register</td></tr>
  </tbody>
</table>

<br>

## ISP Lite Pipe 1

The register offsets are the same as ISP Lite Pipe 0. Add the offset to the base in this table.

<table align="center">
  <thead>
    <tr><th>IP</th><th>Base address</th><th>Span</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Color-matrix converter</td><td>0x0022C000</td><td>512 bytes</td><td>Color-matrix correction</td></tr>
    <tr><td>Demosaic block</td><td>0x0022C200</td><td>512 bytes</td><td>Demosaic</td></tr>
    <tr><td>White-balance block</td><td>0x0022C400</td><td>512 bytes</td><td>White-balance correction</td></tr>
    <tr><td>Black-level block</td><td>0x0022C600</td><td>512 bytes</td><td>Black-level correction</td></tr>
    <tr><td>Downscale scaler</td><td>0x0022C800</td><td>512 bytes</td><td>Scale from 4K to 1920 x 1080</td></tr>
    <tr><td>Chroma resampler</td><td>0x0022CA00</td><td>512 bytes</td><td>Chroma resampling to YUV 4:2:0</td></tr>
  </tbody>
</table>

<br>

## Video TPG 0

The test-pattern generator in this subsystem has no Avalon-MM agent. Only the throttle is programmed. Video TPG 1 uses the same offsets at its own base address.

<table align="center">
  <thead>
    <tr><th>IP</th><th>Base address</th><th>Span</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Rate-control throttle for test-pattern stream 0</td><td>0x0022F400</td><td>512 bytes</td><td>Output rate control for test-pattern stream 0</td></tr>
  </tbody>
</table>

<br>

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x140</td><td>Status</td><td>Throttle status</td></tr>
    <tr><td>0x144</td><td>Control</td><td>Throttle run control</td></tr>
    <tr><td>0x148</td><td>Clear error</td><td>Clears a latched throttle error</td></tr>
    <tr><td>0x14C</td><td>Update settings</td><td>Commits cycles-per-line and blanking writes</td></tr>
    <tr><td>0x150</td><td>Cycles per line</td><td>Output pacing, in clock cycles per line</td></tr>
    <tr><td>0x154</td><td>Blank lines</td><td>Vertical blanking inserted by the throttle</td></tr>
    <tr><td>0x120</td><td>Image width</td><td>Active width reported on the video info bus</td></tr>
    <tr><td>0x124</td><td>Image height</td><td>Active height reported on the video info bus</td></tr>
    <tr><td>0x134</td><td>Subsampling</td><td>Chroma subsampling reported on the video info bus</td></tr>
  </tbody>
</table>

<br>

## Video TPG 1

<table align="center">
  <thead>
    <tr><th>IP</th><th>Base address</th><th>Span</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Rate-control throttle for test-pattern stream 1</td><td>0x0022F200</td><td>512 bytes</td><td>Output rate control for test-pattern stream 1. Same registers as Video TPG 0</td></tr>
  </tbody>
</table>

<br>

## Stream Switch Subsystem

Same register offsets as the Bayer source switch. Add them to 0x0022F000.

<table align="center">
  <thead>
    <tr><th>IP</th><th>Base address</th><th>Span</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Stream switch</td><td>0x0022F000</td><td>512 bytes</td><td>Selects ISP Lite or TPG for each of the two outputs</td></tr>
  </tbody>
</table>

<br>

## Timestamp Subsystem

Each core is a 64-byte Altera® Ethernet IEEE 1588 time-of-day slave. The individual register offsets of this IP are not listed here.

<table align="center">
  <thead>
    <tr><th>IP</th><th>Base address</th><th>Span</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>Time-of-day slave</td><td>0x0022F600</td><td>64 bytes</td><td>Timestamp presented to the RoCEv2 Subsystem</td></tr>
    <tr><td>Time-of-day master counter</td><td>0x0022F640</td><td>64 bytes</td><td>Free-running 96-bit time-of-day counter</td></tr>
  </tbody>
</table>

<br>

## 10G Ethernet Subsystem

The IEEE 1588 PTP interface is on the transmit and receive streams.

<table align="center">
  <thead>
    <tr><th>IP</th><th>Base address</th><th>Span</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>IEEE 1588 PTP interface</td><td>0x0022F680</td><td>128 bytes</td><td>PTP interface on the 10G MAC streams. The window is 0x0022F680 to 0x0022F700.</td></tr>
  </tbody>
</table>

<br>

## RoCEv2 Subsystem

<table align="center">
  <thead>
    <tr><th>IP</th><th>Base address</th><th>Span</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>OmniRoCEv2 IP</td><td>0x00220000</td><td>16 KB</td><td>OmniRoCEv2 protocol engine</td></tr>
    <tr><td>Altera® Sensor Link Stream Multiplexer (<code>asl_stream_multiplexer</code>)</td><td>0x00224000</td><td>4 KB</td><td>Multiplexes the video and raw streams into the RoCEv2 work queues</td></tr>
    <tr><td>Chroma repacker for stream 1</td><td>0x00225000</td><td>512 bytes</td><td>YUV 4:2:0 to NV12-like packing, stream 1</td></tr>
    <tr><td>Chroma repacker for stream 0</td><td>0x00225200</td><td>512 bytes</td><td>YUV 4:2:0 to NV12-like packing, stream 0</td></tr>
  </tbody>
</table>

<br>

These are the direct OmniRoCEv2 registers at base 0x00220000. The queue-pair, source-header and destination-header tables are not separate address windows. A write loads the entry into the table-data registers and then commits it with the table-control register.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x00C</td><td>Status</td><td>Bit 0 is <code>init_done</code>. Poll it after reset before any other access. Bit 31 is the alarm flag.</td></tr>
    <tr><td>0x400</td><td>Configuration 0</td><td>Bits 3:0 are log2 of the payload MTU. The host writes 0xA, a 1024-byte payload. The reset value is 0xC, a 4096-byte payload.</td></tr>
    <tr><td>0x470</td><td>Table control</td><td>Entry number in bits 23:0. Table type in bits 25:24: 0 is queue-pair configuration, 1 is the source header, 2 is the destination header. Bit 28 is 0 for a write and 1 for a read. Bit 31 is busy.</td></tr>
    <tr><td>0x480</td><td>Table data</td><td>Word 0 of the entry. Word N is at 0x480 + 4N. A queue-pair entry and a source-header entry are 6 words. A destination-header entry is 9 words.</td></tr>
  </tbody>
</table>

<br>

A table write has three steps. Write the entry words at 0x480, 0x484 and so on. Write 0x470 with the entry number in bits 23:0 and the table type in bits 25:24, and leave bit 28 clear so the access is a write. Poll bit 31 of 0x470 until it reads 0.

The host programs local queue pairs 0x2 to 0x6. Queue pairs 0x2 and 0x3 carry video, queue pair 0x4 carries the 2D lidar stream, and queue pairs 0x5 and 0x6 carry the point cloud and the IMU. Each queue-pair entry points at source-header entry 18 and destination-header entry 10, and all five queue pairs share those two header entries. The mode is 0x4, unreliable connected. The source MAC address is 87:78:67:62:23:12 and the source IPv4 address is 192.168.1.101. The destination MAC address is 87:78:67:62:23:13 and the destination IPv4 address is 192.168.1.10. IPv6 and VLAN tags are not used.

Queue-pair configuration, table type 0. The entry number is the local queue pair. The same header-table indexes are written for 0x2 to 0x6. The remote queue pair equals the local queue pair.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Field</th><th>Value written by the host</th></tr>
  </thead>
  <tbody>
    <tr><td>0x480</td><td>Remote queue pair in bits 23:0. Mode in bits 27:24.</td><td>Remote queue pair 0x2 to 0x6. Mode 0x4.</td></tr>
    <tr><td>0x484</td><td>Source-header table entry in bits 23:0.</td><td>18</td></tr>
    <tr><td>0x488</td><td>Destination-header table entry in bits 23:0. VLAN valid in bit 24. IPv6 valid in bit 25.</td><td>Entry 10. VLAN and IPv6 clear.</td></tr>
    <tr><td>0x48C</td><td>Initiator PSN in bits 23:0. Read operations allowed in bits 31:24.</td><td>0</td></tr>
    <tr><td>0x490</td><td>Target PSN in bits 23:0.</td><td>0</td></tr>
    <tr><td>0x494</td><td>RQE queue in bits 23:0. RNR enable in bit 24.</td><td>0</td></tr>
  </tbody>
</table>

<br>

Source header, table type 1, entry 18. For IPv4 only bits 31:0 of the 128-bit source IP address are used.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Field</th><th>Value written by the host</th></tr>
  </thead>
  <tbody>
    <tr><td>0x480</td><td>Source MAC address, bits 31:0.</td><td>0x67622312</td></tr>
    <tr><td>0x484</td><td>Source MAC address, bits 47:32. Bits 31:16 are reserved.</td><td>0x00008778</td></tr>
    <tr><td>0x488</td><td>Source IP address, bits 31:0. This is the IPv4 address.</td><td>0xC0A80165 (192.168.1.101)</td></tr>
    <tr><td>0x48C</td><td>Source IP address, bits 63:32.</td><td>0x00000000</td></tr>
    <tr><td>0x490</td><td>Source IP address, bits 95:64.</td><td>0x00000000</td></tr>
    <tr><td>0x494</td><td>Source IP address, bits 127:96.</td><td>0x00000000</td></tr>
  </tbody>
</table>

<br>

Destination header, table type 2, entry 10. For IPv4 only bits 31:0 of the 128-bit destination IP address are used. The host writes `P_Key` 0xD05E and `Q_Key` 0xDECEA5ED, and leaves DSCP, the UDP source port and the VLAN tag at 0.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Field</th><th>Value written by the host</th></tr>
  </thead>
  <tbody>
    <tr><td>0x480</td><td>Destination MAC address, bits 31:0.</td><td>0x67622313</td></tr>
    <tr><td>0x484</td><td>Destination MAC address bits 47:32 in bits 15:0. VLAN VID in bits 27:16, DEI in bit 28 and PCP in bits 31:29.</td><td>0x00008778</td></tr>
    <tr><td>0x488</td><td>Destination IP address, bits 31:0. This is the IPv4 address.</td><td>0xC0A8010A (192.168.1.10)</td></tr>
    <tr><td>0x48C</td><td>Destination IP address, bits 63:32.</td><td>0x00000000</td></tr>
    <tr><td>0x490</td><td>Destination IP address, bits 95:64.</td><td>0x00000000</td></tr>
    <tr><td>0x494</td><td>Destination IP address, bits 127:96.</td><td>0x00000000</td></tr>
    <tr><td>0x498</td><td>DSCP in bits 5:0. UDP source port in bits 31:16.</td><td>0x00000000</td></tr>
    <tr><td>0x49C</td><td><code>P_Key</code> in bits 15:0.</td><td>0x0000D05E</td></tr>
    <tr><td>0x4A0</td><td><code>Q_Key</code>.</td><td>0xDECEA5ED</td></tr>
  </tbody>
</table>

<br>

The Altera® Sensor Link Stream Multiplexer (`asl_stream_multiplexer`) is at 0x00224000. Channel registers repeat every 0x40 bytes from 0x400. The offsets below are channel 0. Channel 1 is at 0x440, channel 2 at 0x480, channel 3 at 0x4C0, and channel 4 at 0x500.

<table align="center">
  <thead>
    <tr><th>Offset</th><th>Register</th><th>Function</th></tr>
  </thead>
  <tbody>
    <tr><td>0x000</td><td>Configuration</td><td>Read-only minor version, major version and number of channels</td></tr>
    <tr><td>0x004</td><td>Control</td><td>Per-channel enable bits and the debug trigger</td></tr>
    <tr><td>0x008</td><td>Status</td><td>Bridge status</td></tr>
    <tr><td>0x400</td><td>Channel config</td><td>Sample width and channel type. Repeats every 0x40 bytes for each channel</td></tr>
    <tr><td>0x404</td><td>Channel control</td><td>Num Lines in bits [15:0], Num Buffers minus one in bits [26:24], and Load Buffer in bits [30:28]. By default, one buffer is set on every channel and Load Buffer is 0.</td></tr>
    <tr><td>0x408</td><td>Channel status</td><td>Per-channel status</td></tr>
    <tr><td>0x40C</td><td>Channel RoCE control</td><td>Queue-pair number used for this channel</td></tr>
    <tr><td>0x410</td><td>Channel RoCE key</td><td><code>R_Key</code> or local key for this channel</td></tr>
    <tr><td>0x414</td><td>Channel offset</td><td>Byte offset applied to the buffer</td></tr>
    <tr><td>0x418</td><td>Remote address low</td><td>Lower 32 bits of the remote buffer address</td></tr>
    <tr><td>0x41C</td><td>Remote address high</td><td>Upper 32 bits of the remote buffer address</td></tr>
    <tr><td>0x420</td><td>Memory address low</td><td>Lower 32 bits of the local payload address</td></tr>
    <tr><td>0x424</td><td>Memory address high</td><td>Upper 32 bits of the local payload address</td></tr>
    <tr><td>0x428</td><td>Memory key</td><td>Local memory key for the payload buffer</td></tr>
    <tr><td>0x700</td><td>Measure video</td><td>Debug measurement in the video clock domain</td></tr>
    <tr><td>0x704</td><td>Measure Ethernet</td><td>Debug measurement in the Ethernet clock domain</td></tr>
  </tbody>
</table>

<br>

Both chroma-repack cores use the VVP image-info registers. Width is at offset 0x120 and height is at offset 0x124. Chroma repacker for stream 0 is at 0x00225200 and chroma repacker for stream 1 is at 0x00225000.

---

[← Terminology and References](10-terminology-and-references.md) | [Documentation index](README.md)
