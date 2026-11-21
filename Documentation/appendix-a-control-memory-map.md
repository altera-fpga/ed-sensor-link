# Appendix A: Nios V/g Control Memory Map

Addresses in this section are byte addresses in the Control Subsystem Nios V data space. The processor reaches the other subsystems through `export_mm_bridge`, so these bases are already the addresses software uses. The register address is the IP base plus the offset in the register table.

Altera VVP video IP shares the register block below. Core-specific registers are listed with the first subsystem that uses that core. A later instance of the same core uses the same offsets at its own base address.

| Offset | Register | Function |
| --- | --- | --- |
| 0x000 | Vendor and product ID | Read-only vendor ID in [31:16] and product ID in [15:0] |
| 0x004 | Version | IP version and register-map version |
| 0x008 | Compile-time parameters | Read-only core parameters. Core-specific parameters follow, through 0x0FC |
| 0x100 | Interrupt control | Enables for the core interrupt sources |
| 0x104 | Interrupt status | Pending core interrupts |
| 0x120 | Image width | Active width, in pixels, of the current field |
| 0x124 | Image height | Active height, in lines, of the current field |
| 0x128 | Interlace | Interlace mode of the current field |
| 0x12C | Bits per sample | Sample precision of the current field |
| 0x130 | Colour space | Colour space of the current field |
| 0x134 | Subsampling | Chroma subsampling of the current field |
| 0x138 | Cositing | Chroma sample position of the current field |
| 0x13C | Field count | Read-only field counter, full mode only |
| 0x140 | Run-time base | First core-specific run-time register. Later offsets are listed with each core |

## Control Subsystem

These slaves are inside `control_subsystem`. `cpu_ram`, the debug module and the descriptor memory are memory windows, not control registers.

| IP | Base address | Span | Function |
| --- | --- | --- | --- |
| `cpu_ram` | 0x00000000 | 1 MB | Nios V instruction and data memory |
| cpu debug module | 0x00200000 | 64 KB | Debug-module access for the JTAG debugger |
| `sys_desc_mem` | 0x00210000 | 8 KB | On-chip memory shared with the message DMA |
| cpu timer | 0x00212400 | 64 bytes | Nios V machine timer |
| MSGDMA TX prefetcher | 0x00212440 | 32 bytes | Fetches transmit descriptors from memory |
| MSGDMA TX | 0x00212460 | 32 bytes | Reads non-RoCE packets from memory |
| MSGDMA RX prefetcher | 0x00212480 | 32 bytes | Fetches receive descriptors from memory |
| MSGDMA RX | 0x002124A0 | 32 bytes | Writes received non-RoCE packets into memory |
| `sysid_0` | 0x002124D0 | 8 bytes | Hardware identification |
| `cpu_jtag_uart` | 0x002124D8 | 8 bytes | Processor console |
| `control_pio_in` | 0x00212500 | 16 bytes | 4-bit input with rising-edge capture |
| `control_pio_out` | 0x00212520 | 16 bytes | 4-bit output |

PIO registers. `control_pio_in` uses data, interrupt mask and edge capture. `control_pio_out` uses data.

| Offset | Register | Function |
| --- | --- | --- |
| 0x00 | Data | Input pins, output pins, or both, depending on how the PIO was generated |
| 0x04 | Direction | Bit value 1 drives the pin as an output. Present on bidirectional PIOs |
| 0x08 | Interrupt mask | Per-bit interrupt enable. Present when the PIO has an interrupt |
| 0x0C | Edge capture | Latched edge event. Write 1 to a bit to clear it. Present when edge capture is enabled |
| 0x10 | Set bits | Write 1 to set the corresponding output bit |
| 0x14 | Clear bits | Write 1 to clear the corresponding output bit |

## MIPI In Subsystem

| IP | Base address | Span | Function |
| --- | --- | --- | --- |
| `mipi_dphy` | 0x00228000 | 4 KB | Dual-link MIPI D-PHY, both camera links |
| `mipi_in_pio_0` | 0x00229000 | 16 bytes | 32-bit bidirectional PIO |
| `mipi_in_pio_1` | 0x00229010 | 16 bytes | 32-bit input PIO |
| `cam_i2c` | 0x00229200 | 64 bytes | Camera I2C controller |
| `board_i2c` | 0x00229300 | 64 bytes | Board I2C controller |
| `pip_conv_0` | 0x00229600 | 512 bytes | Pixel packing for camera link 0 |
| `pip_conv_1` | 0x00229800 | 512 bytes | Pixel packing for camera link 1 |
| `mipi_csi2_0` | 0x0022A000 | 4 KB | CSI-2 receiver for camera link 0 |
| `mipi_csi2_1` | 0x0022B000 | 4 KB | CSI-2 receiver for camera link 1 |

Both I2C controllers use this map. `cam_i2c` is at 0x00229200 and `board_i2c` is at 0x00229300.

| Offset | Register | Function |
| --- | --- | --- |
| 0x00 | Transfer command | Start, stop, 7-bit address and the read/write data bit |
| 0x04 | Receive data | Oldest byte in the receive FIFO |
| 0x08 | Control | Controller enable, bus speed and FIFO thresholds |
| 0x0C | Interrupt enable | Enables for receive overflow, arbitration loss and transfer events |
| 0x10 | Interrupt status | Pending I2C interrupts. Write 1 to clear |
| 0x14 | Status | Bus busy and FIFO state |
| 0x18 | Command FIFO level | Number of entries in the transfer-command FIFO |
| 0x1C | Receive FIFO level | Number of bytes in the receive FIFO |
| 0x20 | SCL low count | SCL low period, in clock cycles |
| 0x24 | SCL high count | SCL high period, in clock cycles |
| 0x28 | SDA hold | SDA hold time, in clock cycles |

Both pixel-packing converters use the VVP common map plus these registers. `pip_conv_0` is at 0x00229600 and `pip_conv_1` is at 0x00229800.

| Offset | Register | Function |
| --- | --- | --- |
| 0x008 | lite mode | `Lite_mode` register |
| 0x00C | debug enabled | `Debug_enabled` register |
| 0x140 | status | Run-time status register |

The two PIO instances use the PIO map above. `pio_0` is bidirectional, so it also has the direction register. `pio_1` is input only. The D-PHY and the two CSI-2 receivers occupy the 4 KB windows in the table. Their bit fields are the Intel MIPI D-PHY and MIPI CSI-2 register files and are not reproduced here.

## ISP In Subsystem

| IP | Base address | Span | Function |
| --- | --- | --- | --- |
| `isp_in_tpg` | 0x0022E400 | 512 bytes | Internal Bayer test-pattern generator |
| `isp_in_rms` | 0x0022E600 | 256 bytes | Remosaic of the internal test pattern |
| `isp_in_switch` | 0x0022E800 | 512 bytes | Selects the Bayer source for each ISP Lite pipe |

Test-pattern generator at 0x0022E400. Offsets are added to that base, and the VVP common registers are also present.

| Offset | Register | Function |
| --- | --- | --- |
| 0x008 | lite mode | `Lite_mode` register |
| 0x00C | debug enabled | `Debug_enabled` register |
| 0x010 | num patterns | Number of patterns register |
| 0x018 | bps | Bits per color sample register |
| 0x01C | pip | Pixels per beat register |
| 0x014 | ctrl in enabled | `Ctrl_in_enabled` register |
| 0x020 | pattern base | First pattern register |
| 0x020 | pattern 0 type | Pattern 0 type identifier |
| 0x024 | pattern 0 color | Pattern 0 `color_space`/`color_sampling` identifier |
| 0x028 | pattern 1 type | Pattern 1 type identifier |
| 0x02C | pattern 1 color | Pattern 1 `color_space`/`color_sampling` identifier |
| 0x030 | pattern 2 type | Pattern 2 type identifier |
| 0x034 | pattern 2 color | Pattern 2 `color_space`/`color_sampling` identifier |
| 0x038 | pattern 3 type | Pattern 3 type identifier |
| 0x03C | pattern 3 color | Pattern 3 `color_space`/`color_sampling` identifier |
| 0x040 | pattern 4 type | Pattern 4 type identifier |
| 0x044 | pattern 4 color | Pattern 4 `color_space`/`color_sampling` identifier |
| 0x048 | pattern 5 type | Pattern 5 type identifier |
| 0x04C | pattern 5 color | Pattern 5 `color_space`/`color_sampling` identifier |
| 0x050 | pattern 6 type | Pattern 6 type identifier |
| 0x054 | pattern 6 color | Pattern 6 `color_space`/`color_sampling` identifier |
| 0x058 | pattern 7 type | Pattern 7 type identifier |
| 0x05C | pattern 7 color | Pattern 7 `color_space`/`color_sampling` identifier |
| 0x140 | status | Run-time status register |
| 0x144 | field count | Frame/field counter since last change (read-only) |
| 0x148 | control | Control register (start/stop) |
| 0x14C | commit | Commit Settings register (commit pending changes in full mode) |
| 0x150 | pattern select | Pattern selection register |
| 0x154 | pre field ctrl pkts | Number of aux/user packets from the control input to be inserted before an output field image info packet (full mode) |
| 0x158 | post field ctrl pkts | Number of aux/user packets from the control input to be inserted before an output field end-of-field packet (full mode) |
| 0x15C | c0 | C0 color sample (Cb or B) for the patterns that require it |
| 0x160 | c1 | C1 color sample (Y or G) for the patterns that require it |
| 0x164 | c2 | C2 color sample (Cr or R) for the patterns that require it |
| 0x168 | bars select | Bars-style selection when the color bars pattern is enabled |
| 0x16C | zone x origin | X part of zone plate origin |
| 0x170 | zone y origin | Y part of zone plate origin |
| 0x174 | zone scaling factor | Coarse scaling factor of zone plate size |
| 0x178 | zone fine tune factor | Fine tune scaling factor of zone plate size |

Source switch at 0x0022E800.

| Offset | Register | Function |
| --- | --- | --- |
| 0x008 | intf type | `Lite_mode` register |
| 0x00C | debug enabled | `Debug_enabled` register |
| 0x010 | uninterrupted inputs | Uninterrupted inputs register |
| 0x014 | auto consume | `Auto_consumme` register |
| 0x018 | num inputs | `Num_inputs` register |
| 0x01C | num outputs | `Num_outputs` register |
| 0x020 | use treadies | Use treadies register |
| 0x024 | crash switch | Crash switching register |
| 0x140 | status | Status register |
| 0x144 | commit | Commit settings register |

The remosaic core at 0x0022E600 uses the VVP common register map. The BSP does not ship a core-specific register header for `intel_vvp_remosaic`.

## ISP Lite Pipe 0

Instance `isp_lite_subsystem`. Every core below also contains the VVP common registers.

| IP | Base address | Span | Function |
| --- | --- | --- | --- |
| `isp_ccm` | 0x0022D000 | 512 bytes | Colour-matrix correction |
| `isp_dms` | 0x0022D200 | 512 bytes | Demosaic |
| `isp_wbc` | 0x0022D400 | 512 bytes | White-balance correction |
| `isp_blc` | 0x0022D600 | 512 bytes | Black-level correction |
| `isp_scaler_down` | 0x0022D800 | 512 bytes | Scale from 4K to 1920 x 1080 |
| `isp_chroma_resampler` | 0x0022DA00 | 512 bytes | Chroma resampling to YUV 4:2:0 |

Black-level correction at 0x0022D600.

| Offset | Register | Function |
| --- | --- | --- |
| 0x008 | lite mode | `Lite_mode` register |
| 0x00C | debug enabled | `Debug_enabled` register |
| 0x010 | bps in | `Bps_in` register |
| 0x014 | bps out | `Bps_out` register |
| 0x018 | num color in | `Num_color_in` register |
| 0x01C | num color out | `Num_color_out` register |
| 0x020 | pip | Pip register |
| 0x024 | max width | Max width register |
| 0x028 | max height | Max height register |
| 0x02C | reflect around zero | Reflect Around Zero register |
| 0x140 | status | Run-time read-only status register |
| 0x144 | frame stats | Run-time read-only frame statistics register |
| 0x148 | commit | Commit register (including lite mode) |
| 0x14C | settings | Settings register |
| 0x150 | cfa 00 black | Cfa 00 black register |
| 0x154 | cfa 00 color | Cfa 00 color register |
| 0x158 | cfa 01 black | Cfa 01 black register |
| 0x15C | cfa 01 color | Cfa 00 color register |
| 0x160 | cfa 10 black | Cfa 10 black register |
| 0x164 | cfa 10 color | Cfa 00 color register |
| 0x168 | cfa 11 black | Cfa 11 black register |
| 0x16C | cfa 11 color | Cfa 00 color register |

White-balance correction at 0x0022D400.

| Offset | Register | Function |
| --- | --- | --- |
| 0x008 | lite mode | `Lite_mode` register |
| 0x00C | debug enabled | `Debug_enabled` register |
| 0x010 | bps in | `Bps_in` register |
| 0x014 | bps out | `Bps_out` register |
| 0x018 | num color in | `Num_color_in` register |
| 0x01C | num color out | `Num_color_out` register |
| 0x020 | pip | Pip register |
| 0x024 | max width | Max width register |
| 0x028 | max height | Max height register |
| 0x140 | status | Run-time read-only status register |
| 0x144 | frame stats | Run-time read-only frame statistics register |
| 0x148 | commit | Commit register (including lite mode) |
| 0x14C | settings | Settings register |
| 0x150 | cfa 00 color | Cfa 00 color register |
| 0x154 | cfa 01 color | Cfa 00 color register |
| 0x158 | cfa 10 color | Cfa 00 color register |
| 0x15C | cfa 11 color | Cfa 00 color register |

Demosaic at 0x0022D200.

| Offset | Register | Function |
| --- | --- | --- |
| 0x008 | lite mode | `Lite_mode` register |
| 0x00C | debug enabled | `Debug_enabled` register |
| 0x010 | bps in | `Bps_in` register |
| 0x014 | bps out | `Bps_out` register |
| 0x018 | num color in | `Num_color_in` register |
| 0x01C | num color out | `Num_color_out` register |
| 0x020 | pip | Pip register |
| 0x024 | max width | Max width register |
| 0x028 | max height | Max height register |
| 0x140 | status | Run-time read-only status register |
| 0x144 | frame stats | Run-time read-only frame statistics register |
| 0x148 | commit | Commit register (including lite mode) |
| 0x14C | settings | Settings register |

Colour matrix at 0x0022D000.

| Offset | Register | Function |
| --- | --- | --- |
| 0x008 | lite mode | `Lite_mode` register |
| 0x00C | debug enabled | `Debug_enabled` register |
| 0x010 | bps in | `Bps_in` register |
| 0x014 | bps out | `Bps_out` register |
| 0x018 | frac bits | `Frac_bits` register |
| 0x01C | coeffs signed | `Coef_signed` register |
| 0x020 | coeffs int bits | `Coeff_int_bits` register |
| 0x024 | summands signed | `Summand_signed` register |
| 0x028 | summands int bits | `Summand_int_bits` register |
| 0x02C | binary point right move | `Binary_point_right_move` register |
| 0x030 | round method | `Round_method` register |
| 0x144 | commit | Commit settings register (full mode) |
| 0x178 | output cs | Output color space register (full mode) |

Scaler at 0x0022D800.

| Offset | Register | Function |
| --- | --- | --- |
| 0x008 | lite mode | `Lite_mode` register |
| 0x00C | debug enabled | `Debug_enabled` register |
| 0x010 | pixels in parallel | `Pixels_in_parallel` register |
| 0x014 | max input width | `Max_input_width` register |
| 0x018 | max output width | `Max_output_width` register |
| 0x01C | algorithm | Scaling algorithm register |
| 0x020 | coeffs runtime load | Run-time coefficient loading register |
| 0x024 | coeffs mem init | Coefficient memory initialization register |
| 0x028 | v scaling enabled | Vertical scaling enabled register |
| 0x02C | v num taps | Vertical number of taps register |
| 0x030 | v num phases | Vertical number of phases register |
| 0x034 | v num banks | Vertical number of banks register |
| 0x038 | v coeffs signed | Vertical `coef_signed` register |
| 0x03C | v coeffs int bits | Vertical coeff integer bits register |
| 0x040 | v coeffs frac bits | Vertical coefficient fractional bits register |
| 0x044 | h scaling enabled | Horizontal scaling enabled register |
| 0x048 | h num taps | Horizontal number of taps register |
| 0x04C | h num phases | Horizontal number of phases register |
| 0x050 | h num banks | Horizontal number of banks register |
| 0x054 | h coeffs signed | Horizontal `coef_signed` register |
| 0x058 | h coeffs int bits | Horizontal coeff integer bits register |
| 0x05C | h coeffs frac bits | Horizontal coefficient fractional bits register |
| 0x068 | v partial scaling enabled | Vertical partial scaling enabled register |
| 0x06C | h partial scaling enabled | Horizontal partial scaling enabled register |
| 0x140 | status | Run-time status register |
| 0x144 | commit | Commit settings register (full mode) |
| 0x148 | output width | Output width register |
| 0x14C | output height | Output height register |
| 0x150 | h bank | Horizontal coefficient bank selection register |
| 0x154 | v bank | Vertical coefficient bank selection register |
| 0x158 | total in width | Total input width register when split horizontal scaling is enabled |
| 0x15C | total out width | Total output width register when split horizontal scaling is enabled |
| 0x160 | h pos error | First pixel position error register when split horizontal scaling is enabled |
| 0x164 | h phase offset | Phase offset register when split horizontal scaling is enabled |
| 0x168 | h phase error | Phase offset error register when split horizontal scaling is enabled |
| 0x16C | h extended fill | "extended fill" register that indicates whether the partial input lines contains |
| 0x170 | total in height | Total input height register when split vertical scaling is enabled |
| 0x174 | total out height | Total output height register when split vertical scaling is enabled |
| 0x178 | v pos error | First pixel position error register when split horizontal scaling is enabled |
| 0x17C | v phase offset | Phase offset register when split vertical scaling is enabled |
| 0x180 | v phase error | Phase offset error register when split vertical scaling is enabled |
| 0x184 | v extended fill | "extended fill" register that indicates whether the partial input image contains |
| 0x200 | rt coeff load bank select | Coefficient bank selection register (this is the bank that will be targeted by the next commit) |
| 0x204 | rt coeff load phase select | Coefficient phase selection register (this is the phase that will be targeted by the next commit) |
| 0x208 | rt coeff load commit | Coefficient commit register |
| 0x20C | rt coeff load tap base | Base register for the first tap coefficient, write `V_NUM_TAPS`/`H_NUM_TAPS` coefficients from this register to |

Chroma resampler at 0x0022DA00.

| Offset | Register | Function |
| --- | --- | --- |
| 0x008 | lite mode | `Lite_mode` register |
| 0x00C | debug enabled | `Debug_enabled` register |
| 0x010 | enable 444 444 | 444 passthrough enabled |
| 0x014 | enable 444 422 | 444-&gt;422 conversion enabled |
| 0x018 | enable 444 420 | 444-&gt;420 conversion enabled |
| 0x01C | enable 422 444 | 422-&gt;444 conversion enabled |
| 0x020 | enable 422 422 | 422 passthrough enabled |
| 0x024 | enable 422 420 | 422-&gt;420 conversion enabled |
| 0x028 | enable 420 444 | 420-&gt;444 conversion enabled |
| 0x02C | enable 420 422 | 420-&gt;422 conversion enabled |
| 0x030 | enable 420 420 | 420 passthrough enabled |
| 0x140 | status | Run-time status register |
| 0x144 | commit | Commit settings register (full mode) |
| 0x148 | output subsampling | Output subsampling selection register |

## ISP Lite Pipe 1

Instance `isp_lite_subsystem_1`. The register offsets are the same as ISP Lite Pipe 0. Add the offset to the base in this table.

| IP | Base address | Span | Function |
| --- | --- | --- | --- |
| `isp_ccm` | 0x0022C000 | 512 bytes | Colour-matrix correction |
| `isp_dms` | 0x0022C200 | 512 bytes | Demosaic |
| `isp_wbc` | 0x0022C400 | 512 bytes | White-balance correction |
| `isp_blc` | 0x0022C600 | 512 bytes | Black-level correction |
| `isp_scaler_down` | 0x0022C800 | 512 bytes | Scale from 4K to 1920 x 1080 |
| `isp_chroma_resampler` | 0x0022CA00 | 512 bytes | Chroma resampling to YUV 4:2:0 |

## TPG Subsystem

The test-pattern generator in this subsystem has no Avalon-MM agent. Only the throttle is programmed. Offsets match the table under TPG 1 as well.

| IP | Base address | Span | Function |
| --- | --- | --- | --- |
| `vvp_throttle_0` | 0x0022F400 | 512 bytes | Output rate control for test-pattern stream 0 |

| Offset | Register | Function |
| --- | --- | --- |
| 0x140 | Status | Throttle status |
| 0x144 | Control | Throttle run control |
| 0x148 | Clear error | Clears a latched throttle error |
| 0x14C | Update settings | Commits cycles-per-line and blanking writes |
| 0x150 | Cycles per line | Output pacing, in clock cycles per line |
| 0x154 | Blank lines | Vertical blanking inserted by the throttle |
| 0x120 | Image width | Active width reported on the video info bus |
| 0x124 | Image height | Active height reported on the video info bus |
| 0x134 | Subsampling | Chroma subsampling reported on the video info bus |

## TPG 1 Subsystem

| IP | Base address | Span | Function |
| --- | --- | --- | --- |
| `vvp_throttle_0` | 0x0022F200 | 512 bytes | Output rate control for test-pattern stream 1. Same registers as TPG Subsystem |

## Stream Switch Subsystem

Same register offsets as `isp_in_switch`. Add them to 0x0022F000.

| IP | Base address | Span | Function |
| --- | --- | --- | --- |
| `vvp_switch` | 0x0022F000 | 512 bytes | Selects ISP Lite or TPG for each of the two outputs |

## Timestamp Subsystem

Each core is a 64-byte Intel Ethernet 1588 time-of-day slave. The BSP does not include a register header for `altera_eth_1588_tod`, so the individual offsets are not listed.

| IP | Base address | Span | Function |
| --- | --- | --- | --- |
| `eth_1588_tod_slave` | 0x0022F600 | 64 bytes | Timestamp presented to the RoCEv2 Subsystem |
| `eth_1588_tod_master` | 0x0022F640 | 64 bytes | Free-running 96-bit time-of-day counter |

## 10G Ethernet Subsystem

`eth_1588` is the IEEE 1588 PTP interface on the transmit and receive streams.

| IP | Base address | Span | Function |
| --- | --- | --- | --- |
| `eth_1588` | 0x0022F680 | 128 bytes | IEEE 1588 PTP interface. The window is 0xF680 to 0xF700. |

## RoCEv2 Subsystem

| IP | Base address | Span | Function |
| --- | --- | --- | --- |
| `rocev2_ip` | 0x00220000 | 16 KB | OmniRoCE protocol engine |
| `vvp_to_abr` | 0x00224000 | 4 KB | Video and lidar bridge into the RoCE work queues |
| `chroma_repack_1` | 0x00225000 | 512 bytes | YUV 4:2:0 to NV12-like packing, stream 1 |
| `chroma_repack_0` | 0x00225200 | 512 bytes | YUV 4:2:0 to NV12-like packing, stream 0 |

These are the direct OmniRoCE registers at base 0x00220000. The queue-pair, source-header and destination-header tables are not separate address windows. A write loads the entry into the table-data registers and then commits it with the table-control register.

| Offset | Register | Function |
| --- | --- | --- |
| 0x00C | Status | Bit 0 is `init_done`. Poll it after reset before any other access. Bit 31 is the alarm flag. |
| 0x400 | Configuration 0 | Bits 3:0 are log2 of the payload MTU. The host writes 0xA, a 1024-byte payload. The reset value is 0xC, a 4096-byte payload. |
| 0x470 | Table control | Entry number in bits 23:0. Table type in bits 25:24: 0 is queue-pair configuration, 1 is the source header, 2 is the destination header. Bit 28 is 0 for a write and 1 for a read. Bit 31 is busy. |
| 0x480 | Table data | Word 0 of the entry. Word N is at 0x480 + 4N. A queue-pair entry and a source-header entry are 6 words. A destination-header entry is 9 words. |

A table write has three steps. Write the entry words at 0x480, 0x484 and so on. Write 0x470 with the entry number in bits 23:0 and the table type in bits 25:24, and leave bit 28 clear so the access is a write. Poll bit 31 of 0x470 until it reads 0.

The host programs local queue pairs 0x2, 0x3 and 0x4. Queue pairs 0x2 and 0x3 carry video and queue pair 0x4 carries lidar. Each queue-pair entry points at source-header entry 18 and destination-header entry 10, and all three queue pairs share those two header entries. The mode is 0x4, unreliable connected. The source MAC address is 87:78:67:62:23:12 and the source IPv4 address is 192.168.1.101. The destination MAC address is 87:78:67:62:23:13 and the destination IPv4 address is 192.168.1.10. IPv6 and VLAN tags are not used.

Queue-pair configuration, table type 0. The entry number is the local queue pair. The same header-table indexes are written for 0x2, 0x3 and 0x4. The remote queue pair equals the local queue pair.

| Offset | Field | Value written by the host |
| --- | --- | --- |
| 0x480 | Remote queue pair in bits 23:0. Mode in bits 27:24. | Remote queue pair 0x2, 0x3 or 0x4. Mode 0x4. |
| 0x484 | Source-header table entry in bits 23:0. | 18 |
| 0x488 | Destination-header table entry in bits 23:0. VLAN valid in bit 24. IPv6 valid in bit 25. | Entry 10. VLAN and IPv6 clear. |
| 0x48C | Initiator PSN in bits 23:0. Read operations allowed in bits 31:24. | 0 |
| 0x490 | Target PSN in bits 23:0. | 0 |
| 0x494 | RQE queue in bits 23:0. RNR enable in bit 24. | 0 |

Source header, table type 1, entry 18. For IPv4 only bits 31:0 of the 128-bit source IP address are used.

| Offset | Field | Value written by the host |
| --- | --- | --- |
| 0x480 | Source MAC address, bits 31:0. | 0x67622312 |
| 0x484 | Source MAC address, bits 47:32. Bits 31:16 are reserved. | 0x00008778 |
| 0x488 | Source IP address, bits 31:0. This is the IPv4 address. | 0xC0A80165 (192.168.1.101) |
| 0x48C | Source IP address, bits 63:32. | 0x00000000 |
| 0x490 | Source IP address, bits 95:64. | 0x00000000 |
| 0x494 | Source IP address, bits 127:96. | 0x00000000 |

Destination header, table type 2, entry 10. For IPv4 only bits 31:0 of the 128-bit destination IP address are used. The host writes `P_Key` 0xD05E and `Q_Key` 0xDECEA5ED, and leaves DSCP, the UDP source port and the VLAN tag at 0.

| Offset | Field | Value written by the host |
| --- | --- | --- |
| 0x480 | Destination MAC address, bits 31:0. | 0x67622313 |
| 0x484 | Destination MAC address bits 47:32 in bits 15:0. VLAN VID in bits 27:16, DEI in bit 28 and PCP in bits 31:29. | 0x00008778 |
| 0x488 | Destination IP address, bits 31:0. This is the IPv4 address. | 0xC0A8010A (192.168.1.10) |
| 0x48C | Destination IP address, bits 63:32. | 0x00000000 |
| 0x490 | Destination IP address, bits 95:64. | 0x00000000 |
| 0x494 | Destination IP address, bits 127:96. | 0x00000000 |
| 0x498 | DSCP in bits 5:0. UDP source port in bits 31:16. | 0x00000000 |
| 0x49C | `P_Key` in bits 15:0. | 0x0000D05E |
| 0x4A0 | `Q_Key`. | 0xDECEA5ED |

`vvp_to_abr` at 0x00224000. Channel registers repeat every 0x40 bytes from 0x400. The offsets below are channel 0. Channel 1 is at 0x440, channel 2 at 0x480, channel 3 at 0x4C0, and channel 4 at 0x500.

| Offset | Register | Function |
| --- | --- | --- |
| 0x000 | Configuration | Read-only minor version, major version and number of channels |
| 0x004 | Control | Per-channel enable bits and the debug trigger |
| 0x008 | Status | Bridge status |
| 0x400 | Channel config | Sample width and channel type. Repeats every 0x40 bytes for each channel |
| 0x404 | Channel control | Num Lines in bits [15:0], Num Buffers minus one in bits [26:24], and Load Buffer in bits [30:28]. The firmware table sets one buffer on every channel, and sets Load Buffer to 0. |
| 0x408 | Channel status | Per-channel status |
| 0x40C | Channel RoCE control | Queue-pair number used for this channel |
| 0x410 | Channel RoCE key | `R_Key` or local key for this channel |
| 0x414 | Channel offset | Byte offset applied to the buffer |
| 0x418 | Remote address low | Lower 32 bits of the remote buffer address |
| 0x41C | Remote address high | Upper 32 bits of the remote buffer address |
| 0x420 | Memory address low | Lower 32 bits of the local payload address |
| 0x424 | Memory address high | Upper 32 bits of the local payload address |
| 0x428 | Memory key | Local memory key for the payload buffer |
| 0x700 | Measure video | Debug measurement in the video clock domain |
| 0x704 | Measure Ethernet | Debug measurement in the Ethernet clock domain |

Both chroma-repack cores use the VVP image-info registers. Width is at offset 0x120 and height is at offset 0x124. `chroma_repack_0` is at 0x00225200 and `chroma_repack_1` is at 0x00225000.

---

[← Terminology and References](09-terminology-and-references.md) | [Documentation index](README.md)
