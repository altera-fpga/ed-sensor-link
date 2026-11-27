FROM ubuntu:24.04 AS builder

ARG DEBIAN_FRONTEND=noninteractive
ARG BUILD_TYPE=Release

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        cmake \
        git \
        ninja-build \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
RUN git clone --depth 1 --branch release/0.3 https://github.com/altera-fpga/host-control-library.git host-control-library

COPY CMakeLists.txt \
     mdk_dual_cam_lidar_example.cpp \
     ./

RUN cmake -S . -B /build -G Ninja -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    && cmake --build /build -j"$(nproc)"

FROM ubuntu:24.04 AS runtime

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /build/mdk_dual_cam_lidar_example /app/mdk_dual_cam_lidar_example
