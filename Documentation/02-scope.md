# Scope

The architecture is shown in Figure 1 in the [Architecture](03-architecture.md) section. It combines the sensor-ingestion path and the UDP-based Control Link with Ethernet ingest, PTP, a shared GTS reset, and five RoCE channels.

**In scope**

- Two MIPI camera inputs based on the IMX678C sensor and connected through Framos connectors.
- The MIPI0 input, corresponding to the first camera connection on the development kit, supports a Framos GMSL camera module. GMSL module support is limited to MIPI0 and is not supported on the MIPI1 input.
- One raw data stream from an RPLIDAR A1M8 two-dimensional lidar module. The lidar data is forwarded directly from the sensor without transformation; its format is defined in [Data Formats](06-data-formats.md).
- Lightweight image conditioning using ISP Lite, including conversion from the camera sensor's 4K, 12-bit Bayer output to 8-bit YUV420 at 1080p and 30 frames per second.
- Data-format standardization across the supported sensor streams, as defined by Altera®, including RoCEv2-side chroma repacking from YUV420 to an NV12-like format before Ethernet transmission.
- Unidirectional Unreliable Connection (UC) transmission over a 10 GbE link using Altera OmniRoCEv2 IP. The RoCEv2 path is transmit-only from the FPGA to the host.
- Remote control, configuration, and initialization of the implemented FPGA IP, enabling a UDP communication between the host and the FPGA. FPGA resource control and reconfiguration over the Ethernet link.
- A hardware timestamping strategy that provides a common timestamp reference for captured sensor data, synchronized to the host via PTP.
- Ethernet ingest for Livox MID-360S 3D Lidar + IMU. Each stream is 64 bits wide and is forwarded into the RoCEv2 subsystem.
- Five RoCEv2 channels. Channels 0 and 1 carry the camera streams, channel 2 carries lidar, and channels 3 and 4 carry the Ethernet ingest streams (Point Cloud and IMU).
- IEEE 1588 PTP on the 10G Ethernet link, with the time-of-day value broadcast into the Ethernet MAC.

The supported sensor streams are ingested through dedicated FPGA resources. The camera streams are conditioned and standardized where applicable. The lidar stream and the two Ethernet ingest streams are forwarded as raw data. The streams are timestamped and transmitted to the host for downstream processing.

**Out of scope**

The following capabilities are outside the scope of this release and may be considered for later development:

- Additional sensor types, including radar, external IMU, and odometry sources.
- Full image signal processing beyond the defined ISP Lite functions, as well as advanced analytics executed on the FPGA.
- Bidirectional or receive-side RoCEv2 data transport.
- Host-side sensor fusion, analytics, and application processing beyond receiving and interpreting the transmitted streams.

Future releases may extend sensor coverage, increase supported line rates, and introduce more advanced on-FPGA processing without changing the fundamental ingestion-and-transport model established in this release.

---

[← Introduction](01-introduction.md) | [Documentation index](README.md) | [Architecture →](03-architecture.md)
