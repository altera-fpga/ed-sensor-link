# Altera Control Link

Altera Control Link is a C library for reading and writing memory-mapped registers over pluggable transports including UDP. C++ and Python bindings are also available.

The Control Subsystem Nios does not configure the cameras, the video pipeline, or RoCEv2 at boot. An x86 host does that work. The host sends UDP messages across the 10G link on the non-RoCE path. The Nios receives them and reads or writes any Avalon-MM slave attached to it. That is how this system is configured.

The Nios V/g firmware runs FreeRTOS. The network stack is FreeRTOS+UDP, using the Altera TSE network interface and the modular scatter-gather DMA (msgdma) engines on the non-RoCE path. Those are the RX and TX DMA blocks in the Control Subsystem. In this build the processor address is 192.168.1.101, the mask is 255.255.255.0, and the gateway is 192.168.1.1. The MAC address used by this stack is generated at boot with the locally administered bit set. It is not the MAC programmed into the RoCEv2 header tables.

After the network is up, the firmware starts the register server and an interrupt-notify task. PTP initialisation is enabled and programs the IEEE 1588 interface in the 10G Ethernet Subsystem. The sensor, VVP, RoCE and bridge setup is done over UDP instead.

The control server listens on UDP port 5005. Each request names a base address and a byte offset, using the Control Nios addresses in [Appendix A](appendix-a-control-memory-map.md). A read returns the 32-bit value at that address. A write stores the value in the request. The offset must be a multiple of 4. One datagram may carry several requests, up to 32, and the reply contains one response for each of them. A third operation registers an interrupt: the Nios then sends a notification to UDP port 5006 on the host that issued the request.

For more details, see the README in the [host-control-library](https://github.com/altera-fpga/host-control-library) repository.

## UDP Transport

The UDP v1 packet structure is defined as follows:

**Request packet (24 bytes)**

- magic (u32): 0x41434C4B ("ACLK")
- version (u8): 1
- op (u8): 1=read, 2=write
- reserved (u16): 0
- `sequence_id` (u32): request ID echoed by the response
- `base_address` (u32): register block base address (network byte order)
- offset (u32): register offset from base address (network byte order)
- value (u32): write value for op=2, ignored for reads

**Response packet (24 bytes)**

- magic (u32): 0x41434C4B
- version (u8): 1
- op (u8): mirrors request op
- status (u16): 0=ok, non-zero indicates error
- `sequence_id` (u32): same as request correlation ID
- `base_address` (u32): same as register block base address (network byte order)
- offset (u32): same as register offset (network byte order)
- value (u32): read value for op=1, ignored for writes

## Host configuration

The host program is altera-control-link. The application in `main.cpp` is `sensor_bridge_app`. It opens a UDP register transport to 192.168.1.101 port 5005 and uses the library drivers for I2C, the Video and Vision Processing IP, the Base RoCEv2 IP, and the VVP to ABR Bridge. Every access below is a UDP read or write of the kind defined in the packet format.

The application configures the board in this order.

| What | Base address | What the host sets |
| --- | --- | --- |
| Cameras | `cam_i2c` 0x00229200 | Both cameras are programmed through `cam_i2c`. MIPI1 is a direct IMX678 at I2C address 0x1A whether or not a GMSL module is fitted. MIPI0 depends on the module. With the Framos GMSL module, the host first initializes the serializer at 0x42, the deserializer at 0x6A and the GPIO expander at 0x21, then programs the IMX678 at 0x36. Without the GMSL module, that link setup is skipped and the direct IMX678 on MIPI0 is at 0x37. GMSL is supported only on MIPI0. `main.cpp` is built for the GMSL address 0x36; 0x37 is the commented alternative for the direct camera. |
| Base RoCEv2 IP | `rocev2_ip` 0x00220000 | Runs the global initialization, then brings up three unreliable-connected queue pairs. Local queue pairs 0x2 and 0x3 carry the two video streams. Queue pair 0x4 carries lidar. All three queue pairs use source-header entry 18 and destination-header entry 10. The source address in the header tables is 192.168.1.101 and the destination is 192.168.1.10. The table words are listed with the OmniRoCE registers in [Appendix A](appendix-a-control-memory-map.md). |
| VVP to ABR Bridge | `vvp_to_abr` 0x00224000<br>`chroma_repack_0` 0x00225200<br>`chroma_repack_1` 0x00225000 | Channel 0 and channel 1 are 1080-line VVP 4:2:0 video, bound to queue pairs 0x2 and 0x3. Channel 2 is lidar, bound to queue pair 0x4. The three channels are then enabled. Both chroma repackers are set to 1920 x 1080. |
| Stream switch and rate control | `vvp_switch` 0x0022F000<br>TPG throttle 0x0022F400<br>TPG 1 throttle 0x0022F200 | Consumes the two test-pattern inputs and enables the two ISP Lite inputs. Output 0 selects ISP Lite pipe 0. Output 1 selects ISP Lite pipe 1. Both video rate controllers are set to 1920 x 1080. |
| ISP Lite pipes | Pipe 0 blc 0x0022D600<br>Pipe 0 wbc 0x0022D400<br>Pipe 0 dms 0x0022D200<br>Pipe 0 ccm 0x0022D000<br>Pipe 0 scaler 0x0022D800<br>Pipe 0 crs 0x0022DA00<br>Pipe 1 blc 0x0022C600<br>Pipe 1 wbc 0x0022C400<br>Pipe 1 dms 0x0022C200<br>Pipe 1 ccm 0x0022C000<br>Pipe 1 scaler 0x0022C800<br>Pipe 1 crs 0x0022CA00 | Programs both pipes for a 3840 x 2160 Bayer input and a 1920 x 1080 output. That covers black level, white balance, demosaic, the colour-matrix coefficients, the scaler, and the chroma resampler. The chroma resampler output is 4:2:0. |
| ISP In | `isp_in_switch` 0x0022E800<br>`isp_in_tpg` 0x0022E400<br>`isp_in_rms` 0x0022E600<br>`pip_conv_0` 0x00229600 | Selects the two MIPI Bayer inputs and consumes the internal test-pattern input. The internal test-pattern is still started, and the MIPI pixels-in-parallel converter is given a 3840 x 2160 image size. |

The control firmware lists five channels, queue pairs 0x2 through 0x6, and the Num Buffers and Load Buffer fields.

---

[← Control Subsystem](subsystems/14-control-subsystem.md) | [Documentation index](README.md) | [Terminology and References →](09-terminology-and-references.md)
