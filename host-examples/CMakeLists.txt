cmake_minimum_required(VERSION 3.16)
project(sensor_link_examples LANGUAGES CXX)

set(HCL_BUILD_CPP      ON CACHE BOOL "" FORCE)
set(HCL_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(HCL_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(HCL_BUILD_DRIVERS  ON CACHE BOOL "" FORCE)
set(HCL_BUILD_SENSORS  ON CACHE BOOL "" FORCE)
set(HCL_BUILD_PYTHON   OFF CACHE BOOL "" FORCE)

add_subdirectory(host-control-library)

add_executable(mdk_dual_cam_lidar_example
    mdk_dual_cam_lidar_example.cpp
)

target_compile_features(mdk_dual_cam_lidar_example PRIVATE cxx_std_20)

target_link_libraries(mdk_dual_cam_lidar_example
    PRIVATE
        hcl::core
        hcl::drivers::altera_i2c
        hcl::drivers::altera_roce
        hcl::drivers::abr_bridge
        hcl::drivers::altera_vvp_switch
        hcl::drivers::altera_vvp_crs
        hcl::drivers::altera_vvp_scaler
        hcl::drivers::altera_vvp_csc
        hcl::drivers::altera_vvp_demosaic
        hcl::drivers::altera_vvp_wbc
        hcl::drivers::altera_vvp_blc
        hcl::drivers::altera_vvp_tpg
        hcl::drivers::altera_vvp_pip_conv
        hcl::sensors::framos_gmsl
        hcl::sensors::imx678
)
