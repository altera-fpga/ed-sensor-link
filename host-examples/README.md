# Altera® Sensor Link Host Examples

This directory contains C++ applications that configure supported Sensor Link FPGA designs through the Host Control Library.

## Prerequisites

- Docker
- A supported Sensor Link FPGA binary already programmed to the board

## Build the Docker image

From this directory, run:

```bash
docker build -t sensor-link-examples .
```

The Docker build creates a runtime image containing all example application executables.

## Run an example

Start the Docker container using host networking so the container can reach the FPGA UDP endpoint:

```bash
docker run --rm --network host sensor-link-examples
```


### `mdk_dual_cam_lidar_example`
Configures the dual-camera, 2D lidar, and 3D lidar reference design for the Modular Development Kit. It initializes the video pipeline and RoCEv2 queue pairs, and optionally initializes a Framos GMSL camera module.

```bash
./mdk_dual_cam_lidar_example
```

To initialize the optional Framos GMSL camera chain, pass `--gmsl`:

```bash
./mdk_dual_cam_lidar_example --gmsl
```
