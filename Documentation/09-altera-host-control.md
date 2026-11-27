# Altera® Host Control Library

Altera® Host Control Library is a C library for reading and writing memory-mapped registers over pluggable transports including UDP. C++ and Python bindings are also available.

The Control Subsystem does not configure the cameras, the video pipeline, the OmniRoCEv2 IP, or the RoCEv2 streams at boot. An x86 host does that work. The host sends UDP messages across the 10G link on the non-RoCE path. The Control Subsystem receives them and reads or writes any Avalon-MM slave attached to it. That is how this system is configured.

The Control Subsystem firmware runs FreeRTOS. The network stack is FreeRTOS+UDP, using the Altera® TSE network interface and the modular scatter-gather DMA (mSGDMA) engines on the non-RoCE path. Those are the RX and TX DMA blocks in the Control Subsystem. In this build the node address is 192.168.1.101, the mask is 255.255.255.0, and the gateway is 192.168.1.1. The MAC address used by this stack is generated at boot with the locally administered bit set. It is not the MAC programmed into the RoCEv2 header tables.

After the network is up, the firmware starts the register server . PTP initialization is enabled and programs the IEEE 1588 interface in the 10G Ethernet Subsystem. The setup of the sensors, the VVP ISP, the OmniRoCEv2 IP, and the Altera® Sensor Link Stream Multiplexer is done over UDP instead.

The control server listens on UDP port 5005. Each request names a base address and a byte offset, using the Control addresses in [Appendix A](appendix-a-control-memory-map.md). A read returns the 32-bit value at that address. A write stores the value in the request. The offset must be a multiple of 4. One datagram may carry several requests, up to 32, and the reply contains one response for each of them.

For more details, see the README in the [host-control-library](https://github.com/altera-fpga/host-control-library) repository.

## UDP Transport

The UDP v1 packet structure is defined as follows:

**Request packet (24 bytes)**

- `magic` (u32): 0x41434C4B ("ACLK")
- `version` (u8): 1
- `op` (u8): 1=read, 2=write
- `reserved` (u16): 0
- `sequence_id` (u32): request ID echoed by the response
- `base_address` (u32): register block base address (network byte order)
- `offset` (u32): register offset from base address (network byte order)
- `value` (u32): write value for op=2, ignored for reads

**Response packet (24 bytes)**

- `magic` (u32): 0x41434C4B
- `version` (u8): 1
- `op` (u8): mirrors request op
- `status` (u16): 0=ok, non-zero indicates error
- `sequence_id` (u32): same as in the request
- `base_address` (u32): same as register block base address (network byte order)
- `offset` (u32): same as register offset (network byte order)
- `value` (u32): read value for op=1, ignored for writes

## Host configuration

An example application can be found in [host-examples](host-examples/mdk_dual_cam_lidar_example.cpp). The application opens a UDP register transport to 192.168.1.101 port 5005 and uses the library drivers for I2C, the Video and Vision Processing IP, the OmniRoCEv2 IP, and the Altera® Sensor Link Stream Multiplexer. Every access below is a UDP read or write of the kind defined in the packet format.

The application configures the board in this order.

<table align="center">
  <thead>
    <tr><th>What</th><th>Base address</th><th>What the host sets</th></tr>
  </thead>
  <tbody>
    <tr><td>Cameras</td><td>Camera I2C controller 0x00229200</td><td>Both cameras are programmed through the camera I2C controller. MIPI1 is a direct IMX678 at I2C address 0x1A whether or not a GMSL module is fitted. MIPI0 depends on the module. With the Framos GMSL module, the host first initializes the serializer at 0x42, the deserializer at 0x6A and the GPIO expander at 0x21, then programs the IMX678 at 0x36. Without the GMSL module, that link setup is skipped and the direct IMX678 on MIPI0 is at 0x37. GMSL is supported only on MIPI0. <code>main.cpp</code> is built for the GMSL address 0x36; 0x37 is the commented alternative for the direct camera.</td></tr>
    <tr><td>OmniRoCEv2 IP</td><td>0x00220000</td><td>Runs the global initialization, then brings up five unreliable-connected queue pairs. Local queue pairs 0x2 and 0x3 carry the two video streams. Queue pair 0x4 carries the 2D lidar stream, and queue pairs 0x5 and 0x6 carry the point cloud and the IMU. All five queue pairs use source-header entry 18 and destination-header entry 10. The source address in the header tables is 192.168.1.101 and the destination is 192.168.1.10. The table words are listed with the OmniRoCEv2 registers in <a href="appendix-a-control-memory-map.md">Appendix A</a>.</td></tr>
    <tr><td>Altera® Sensor Link Stream Multiplexer</td><td><code>asl_stream_multiplexer</code> 0x00224000<br>chroma repacker for stream 0 0x00225200<br>chroma repacker for stream 1 0x00225000</td><td>Channel 0 and channel 1 are 1080-line VVP 4:2:0 video, bound to queue pairs 0x2 and 0x3. Channel 2 is lidar, bound to queue pair 0x4. Channel 3 is the point cloud, bound to queue pair 0x5, and channel 4 is the IMU, bound to queue pair 0x6. All five channels are then enabled. Both chroma repackers are set to 1920 x 1080.</td></tr>
    <tr><td>Stream switch and rate control</td><td>Stream switch 0x0022F000<br>TPG 0 throttle 0x0022F400<br>TPG 1 throttle 0x0022F200</td><td>Consumes the two test-pattern inputs and enables the two ISP Lite inputs. Output 0 selects ISP Lite pipe 0. Output 1 selects ISP Lite pipe 1. Both video rate controllers are set to 1920 x 1080.</td></tr>
    <tr><td>ISP Lite pipes</td><td>Pipe 0 blc 0x0022D600<br>Pipe 0 wbc 0x0022D400<br>Pipe 0 dms 0x0022D200<br>Pipe 0 ccm 0x0022D000<br>Pipe 0 scaler 0x0022D800<br>Pipe 0 crs 0x0022DA00<br>Pipe 1 blc 0x0022C600<br>Pipe 1 wbc 0x0022C400<br>Pipe 1 dms 0x0022C200<br>Pipe 1 ccm 0x0022C000<br>Pipe 1 scaler 0x0022C800<br>Pipe 1 crs 0x0022CA00</td><td>Programs both pipes for a 3840 x 2160 Bayer input and a 1920 x 1080 output. That covers black level, white balance, demosaic, the color-matrix coefficients, the scaler, and the chroma resampler. The chroma resampler output is 4:2:0.</td></tr>
    <tr><td>ISP In</td><td>Bayer source switch 0x0022E800<br>internal Bayer test-pattern generator 0x0022E400<br>internal test-pattern remosaic 0x0022E600<br>pixel-packing converter for camera link 0 0x00229600</td><td>Selects the two MIPI Bayer inputs and consumes the internal test-pattern input. The internal test-pattern is still started, and the MIPI pixels-in-parallel converter is given a 3840 x 2160 image size.</td></tr>
  </tbody>
</table>

<br>

The host configures the five channels and queue pairs 0x2 through 0x6, including the Num Buffers and Load Buffer fields.

---

[← Control Subsystem](subsystems/14-control-subsystem.md) | [Documentation index](README.md) | [Terminology and References →](10-terminology-and-references.md)
