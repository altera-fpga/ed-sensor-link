/*
 * Copyright (C) Altera Corporation
 *
 * This software and the related documents are Altera copyrighted materials, and
 * your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise, you may
 * not use, modify, copy, publish, distribute, disclose or transmit this software
 * or the related documents without Altera's prior written permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in the License.
 */

#include <array>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string_view>
#include <thread>
#include <vector>

#include "hcl/drivers/altera_roce.hpp"
#include "hcl/drivers/abr_bridge.hpp"
#include "hcl/drivers/altera_vvp_switch.hpp"
#include "hcl/drivers/altera_vvp_crs.hpp"
#include "hcl/drivers/altera_vvp_scaler.hpp"
#include "hcl/drivers/altera_vvp_csc.hpp"
#include "hcl/drivers/altera_vvp_demosaic.hpp"
#include "hcl/drivers/altera_vvp_wbc.hpp"
#include "hcl/drivers/altera_vvp_blc.hpp"
#include "hcl/drivers/altera_vvp_core.hpp"
#include "hcl/drivers/altera_vvp_tpg.hpp"
#include "hcl/drivers/altera_vvp_pip_conv.hpp"
#include "hcl/drivers/altera_i2c.hpp"
#include "hcl/register_interface.hpp"
#include "hcl/sensors/framos_gmsl.hpp"
#include "hcl/sensors/imx678.hpp"
#include "hcl/udp_register_transport.hpp"

#define INIT_HSIZE                  3840
#define INIT_VSIZE                  2160
#define ICON_HSIZE                  144
#define ICON_VSIZE                  144
#define IMG_INFO_COLORSPACE_RGB     0
#define IMG_INFO_COLORSPACE_YCbCr   1

#define IMG_INFO_SUBSAMPLING_420    0
#define IMG_INFO_SUBSAMPLING_422    2
#define IMG_INFO_SUBSAMPLING_444    3
#define BAYER_PHASE                 0
#define TPG_BAYER_SWITCH_IN         0
#define MIPI_BAYER_SWITCH_IN        1
#define MIPI2_BAYER_SWITCH_IN       2
#define BAYER_SWITCH_OUT            0
#define BAYER_SWITCH_OUT2           1

#define VVP_THROTTLE_IMG_INFO_WIDTH             0x120u
#define VVP_THROTTLE_IMG_INFO_HEIGHT            0x124u
#define VVP_THROTTLE_IMG_INFO_SUBSAMPLING       0x134u
#define VVP_THROTTLE_STATUS_REG_ADDR            0x140u
#define VVP_THROTTLE_CONTROL_REG_ADDR           0x144u
#define VVP_THROTTLE_CLEAR_ERROR_REG_ADDR       0x148u
#define VVP_THROTTLE_UPDATE_SETTINGS_REG_ADDR   0x14Cu
#define VVP_THROTTLE_CYCLES_PER_LINE_REG_ADDR   0x150u
#define VVP_THROTTLE_BLANK_LINES_REG_ADDR       0x154u

#define VVP_CHROMA_REPACK_IMG_INFO_WIDTH        0x120u
#define VVP_CHROMA_REPACK_IMG_INFO_HEIGHT       0x124u

namespace {

const char* status_code_to_string(hcl::StatusCode code) {
    switch (code) {
        case hcl::StatusCode::Ok:
            return "Ok";
        case hcl::StatusCode::Timeout:
            return "Timeout";
        case hcl::StatusCode::TransportError:
            return "TransportError";
        case hcl::StatusCode::ProtocolError:
            return "ProtocolError";
        case hcl::StatusCode::Unsupported:
            return "Unsupported";
        case hcl::StatusCode::InvalidArgument:
            return "InvalidArgument";
        case hcl::StatusCode::NotConnected:
            return "NotConnected";
        case hcl::StatusCode::OutOfRange:
            return "OutOfRange";
        case hcl::StatusCode::AllocationFailed:
            return "AllocationFailed";
        case hcl::StatusCode::InternalError:
            return "InternalError";
        default:
            return "Unknown";
    }
}

const hcl::drivers::AlteraVvpCsc::Coefficients ccm_4000k = {
    .coeffs = {
        2.8356850100f, -1.7611857700f, -0.0801705718f,
       -0.3981165590f,  2.3106331800f, -0.9117203350f,
       -0.5990669730f, -0.3523570900f,  1.9526220600f,
    },
    .summands = {0.0f, 0.0f, 0.0f},
};

}  // namespace

int main(int argc, char** argv) {
    bool gmsl_enabled = false;

    for (int i = 1; i < argc; ++i) {
        const std::string_view arg(argv[i]);
        if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: mdk_dual_cam_lidar_example [--gmsl]\n";
            std::cout << "  --gmsl  Initialize the Framos GMSL driver\n";
            return 0;
        } else if (arg == "--gmsl") {
            gmsl_enabled = true;
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            std::cerr << "Usage: mdk_dual_cam_lidar_example [--gmsl]\n";
            return 1;
        }
    }

    constexpr const char* kHost = "192.168.1.101";
    constexpr std::uint16_t kPort = 5005u;
    constexpr std::uint32_t kTimeoutMs = 500u;
    constexpr std::uint32_t kRoceBaseAddress = 0x220000u;
    constexpr std::uint32_t kI2cBaseAddress = 0x229200u;
    constexpr std::uint32_t kI2cSysClkHz = 100000000u;
    constexpr std::uint8_t kImx678Address  = 0x37u;
    constexpr std::uint8_t kImx678Address2 = 0x1Au;
    constexpr std::uint8_t kGmslSerAddress  = 0x42u;
    constexpr std::uint8_t kGmslDesAddress  = 0x6Au;
    constexpr std::uint8_t kGmslGpioAddress = 0x21u;
    constexpr std::uint32_t kVvpThrottleCsrBase  = 0x22f400u;
    constexpr std::uint32_t kVvpThrottleCsrBase2 = 0x22f200u;
    constexpr std::uint32_t kAbrBridgeBaseAddress = 0x224000u;
    constexpr std::uint32_t kVvpSwitchBaseAddress = 0x22f000u;
    constexpr std::uint32_t kInputSwitchBaseAddress = 0x22e800u;
    constexpr std::uint32_t kCrsBaseAddress = 0x22da00u;
    constexpr std::uint32_t kCrs2BaseAddress = 0x22ca00u;
    constexpr std::uint32_t kScalerBaseAddress = 0x22d800u;
    constexpr std::uint32_t kScaler2BaseAddress = 0x22c800u;
    constexpr std::uint32_t kCscBaseAddress = 0x22d000u;
    constexpr std::uint32_t kCsc2BaseAddress = 0x22c000u;
    constexpr std::uint32_t kDemosaicBaseAddress = 0x22d200u;
    constexpr std::uint32_t kDemosaic2BaseAddress = 0x22c200u;
    constexpr std::uint32_t kWbcBaseAddress = 0x22d400u;
    constexpr std::uint32_t kWbc2BaseAddress = 0x22c400u;
    constexpr std::uint32_t kBlcBaseAddress = 0x22d600u;
    constexpr std::uint32_t kBlc2BaseAddress = 0x22c600u;
    constexpr std::uint32_t kPipConvMipiBaseAddress = 0x229600u;
    constexpr std::uint32_t kInputTpgBaseAddress = 0x22e400u;
    constexpr std::uint32_t kVvpChromaRepackCsrBase  = 0x225200u;
    constexpr std::uint32_t kVvpChromaRepackCsrBase2 = 0x225000u;

    // Create a UDP transport and wrap it in a RegisterInterface
    auto transport = std::make_shared<hcl::UdpRegisterTransport>(kHost, kPort, kTimeoutMs);
    auto reg = std::make_shared<hcl::RegisterInterface>(transport);
    auto status_check = [&](const hcl::Status& current_status, const char* error_message) {
        if (current_status.ok()) {
            return true;
        }

        std::cerr << error_message << ": " << status_code_to_string(current_status.code) << '\n';
        transport->disconnect();
        return false;
    };

    hcl::drivers::AlteraRoCE roce(reg, kRoceBaseAddress);
    hcl::drivers::AbrBridge bridge(reg, kAbrBridgeBaseAddress);
    hcl::drivers::AlteraVvpSwitch vvp_switch(reg, kVvpSwitchBaseAddress);
    hcl::drivers::AlteraVvpSwitch input_switch(reg, kInputSwitchBaseAddress);
    hcl::drivers::AlteraVvpCrs crs(reg, kCrsBaseAddress);
    hcl::drivers::AlteraVvpCrs crs2(reg, kCrs2BaseAddress);
    hcl::drivers::AlteraVvpScaler scaler(reg, kScalerBaseAddress);
    hcl::drivers::AlteraVvpScaler scaler2(reg, kScaler2BaseAddress);
    hcl::drivers::AlteraVvpCsc csc(reg, kCscBaseAddress);
    hcl::drivers::AlteraVvpCsc csc2(reg, kCsc2BaseAddress);
    hcl::drivers::AlteraVvpDemosaic demosaic(reg, kDemosaicBaseAddress);
    hcl::drivers::AlteraVvpDemosaic demosaic2(reg, kDemosaic2BaseAddress);
    hcl::drivers::AlteraVvpWbc wbc(reg, kWbcBaseAddress);
    hcl::drivers::AlteraVvpWbc wbc2(reg, kWbc2BaseAddress);
    hcl::drivers::AlteraVvpBlc blc(reg, kBlcBaseAddress);
    hcl::drivers::AlteraVvpBlc blc2(reg, kBlc2BaseAddress);
    hcl::drivers::AlteraVvpPipConv pip_conv_mipi(reg, kPipConvMipiBaseAddress);
    hcl::drivers::AlteraVvpTpg input_tpg(reg, kInputTpgBaseAddress);

    auto i2c = std::make_shared<hcl::drivers::AlteraI2C>(reg, kI2cBaseAddress, kI2cSysClkHz);
    hcl::sensors::FramosGmsl framos_gmsl(i2c, kGmslSerAddress, kGmslDesAddress, kGmslGpioAddress, kImx678Address);
    hcl::sensors::IMX678 imx678(i2c, kImx678Address);
    hcl::sensors::IMX678 imx678_2(i2c, kImx678Address2);

    const std::array<hcl::drivers::AlteraRoCE::UcQpParams, 5> kQpParams = {{
        {
            .local_qp         = 0x2,
            .remote_qp        = 0x2,
            .src_hdr_entry    = 18u,
            .dst_hdr_entry    = 10u,
            .rqe_queue        = 0u,
            .initiator_psn    = 0u,
            .target_psn       = 0u,
            .timeout          = 0u,
            .rnr_en           = 0u,
            .vlan_valid       = 0u,
            .ipv6_valid       = 0u,
            .read_ops_allowed = 0u,
            .src = {
                .src_mac   = 0x00000007ED622312ull,
                .src_ip_hi = 0x0000000000000000u,
                .src_ip_lo = 0x00000000C0A80165u,  /* 192.168.1.101 */
            },
            .dst = {
                .dst_mac      = 0x00000007ED622313ull,
                .vlan_vid     = 0u,
                .vlan_dei     = 0u,
                .vlan_pcp     = 0u,
                .dst_ip_hi    = 0x0000000000000000u,
                .dst_ip_lo    = 0x00000000C0A8010Au,  /* 192.168.1.10 */
                .dscp         = 0u,
                .udp_src_port = 0u,
                .pkey         = 0xD05Eu,
                .qkey         = 0xDECEA5EDu,
            },
        },
        {
            .local_qp         = 0x3,
            .remote_qp        = 0x3,
            .src_hdr_entry    = 18u,
            .dst_hdr_entry    = 10u,
            .rqe_queue        = 0u,
            .initiator_psn    = 0u,
            .target_psn       = 0u,
            .timeout          = 0u,
            .rnr_en           = 0u,
            .vlan_valid       = 0u,
            .ipv6_valid       = 0u,
            .read_ops_allowed = 0u,
            .src = {
                .src_mac   = 0x00000007ED622312ull,
                .src_ip_hi = 0x0000000000000000u,
                .src_ip_lo = 0x00000000C0A80165u,  /* 192.168.1.101 */
            },
            .dst = {
                .dst_mac      = 0x00000007ED622313ull,
                .vlan_vid     = 0u,
                .vlan_dei     = 0u,
                .vlan_pcp     = 0u,
                .dst_ip_hi    = 0x0000000000000000u,
                .dst_ip_lo    = 0x00000000C0A8010Au,  /* 192.168.1.10 */
                .dscp         = 0u,
                .udp_src_port = 0u,
                .pkey         = 0xD05Eu,
                .qkey         = 0xDECEA5EDu,
            },
        },
        {
            .local_qp         = 0x4,
            .remote_qp        = 0x4,
            .src_hdr_entry    = 18u,
            .dst_hdr_entry    = 10u,
            .rqe_queue        = 0u,
            .initiator_psn    = 0u,
            .target_psn       = 0u,
            .timeout          = 0u,
            .rnr_en           = 0u,
            .vlan_valid       = 0u,
            .ipv6_valid       = 0u,
            .read_ops_allowed = 0u,
            .src = {
                .src_mac   = 0x00000007ED622312ull,
                .src_ip_hi = 0x0000000000000000u,
                .src_ip_lo = 0x00000000C0A80165u,  /* 192.168.1.101 */
            },
            .dst = {
                .dst_mac      = 0x00000007ED622313ull,
                .vlan_vid     = 0u,
                .vlan_dei     = 0u,
                .vlan_pcp     = 0u,
                .dst_ip_hi    = 0x0000000000000000u,
                .dst_ip_lo    = 0x00000000C0A8010Au,  /* 192.168.1.10 */
                .dscp         = 0u,
                .udp_src_port = 0u,
                .pkey         = 0xD05Eu,
                .qkey         = 0xDECEA5EDu,
            },
        },
        {
            .local_qp         = 0x5,
            .remote_qp        = 0x5,
            .src_hdr_entry    = 18u,
            .dst_hdr_entry    = 10u,
            .rqe_queue        = 0u,
            .initiator_psn    = 0u,
            .target_psn       = 0u,
            .timeout          = 0u,
            .rnr_en           = 0u,
            .vlan_valid       = 0u,
            .ipv6_valid       = 0u,
            .read_ops_allowed = 0u,
            .src = {
                .src_mac   = 0x00000007ED622312ull,
                .src_ip_hi = 0x0000000000000000u,
                .src_ip_lo = 0x00000000C0A80165u,  /* 192.168.1.101 */
            },
            .dst = {
                .dst_mac      = 0x00000007ED622313ull,
                .vlan_vid     = 0u,
                .vlan_dei     = 0u,
                .vlan_pcp     = 0u,
                .dst_ip_hi    = 0x0000000000000000u,
                .dst_ip_lo    = 0x00000000C0A8010Au,  /* 192.168.1.10 */
                .dscp         = 0u,
                .udp_src_port = 0u,
                .pkey         = 0xD05Eu,
                .qkey         = 0xDECEA5EDu,
            },
        },
        {
            .local_qp         = 0x6,
            .remote_qp        = 0x6,
            .src_hdr_entry    = 18u,
            .dst_hdr_entry    = 10u,
            .rqe_queue        = 0u,
            .initiator_psn    = 0u,
            .target_psn       = 0u,
            .timeout          = 0u,
            .rnr_en           = 0u,
            .vlan_valid       = 0u,
            .ipv6_valid       = 0u,
            .read_ops_allowed = 0u,
            .src = {
                .src_mac   = 0x00000007ED622312ull,
                .src_ip_hi = 0x0000000000000000u,
                .src_ip_lo = 0x00000000C0A80165u,  /* 192.168.1.101 */
            },
            .dst = {
                .dst_mac      = 0x00000007ED622313ull,
                .vlan_vid     = 0u,
                .vlan_dei     = 0u,
                .vlan_pcp     = 0u,
                .dst_ip_hi    = 0x0000000000000000u,
                .dst_ip_lo    = 0x00000000C0A8010Au,  /* 192.168.1.10 */
                .dscp         = 0u,
                .udp_src_port = 0u,
                .pkey         = 0xD05Eu,
                .qkey         = 0xDECEA5EDu,
            },
        },
    }};

    // Connect
    hcl::Status status = transport->connect();
    if (!status.ok()) {
        std::cerr << "Connect failed: " << status_code_to_string(status.code) << '\n';
        return 1;
    }

    std::cout << "Connected to " << kHost << ':' << kPort << '\n';

    if (!status_check(i2c->init(), "I2C init failed")) {
        return 1;
    }

    // Framos GMSL
    if (gmsl_enabled) {
        std::cout << "Initialize Framos GMSL modules\n";
        if (!status_check(framos_gmsl.init(), "Framos GMSL init failed")) {
            return 1;
        }
    }

    // Check sensors
    auto sensor_check_result = i2c->read_extended(kImx678Address, 0x3000u);
    if (!status_check(sensor_check_result.status, "IMX678 sensor 1 check failed")) {
        return 1;
    } else {
        std::cout << "IMX678 sensor 1 check passed: " << static_cast<unsigned>(sensor_check_result.value) << '\n';
    }

    auto sensor_check_result_2 = i2c->read_extended(kImx678Address2, 0x3000u);
    if (!status_check(sensor_check_result_2.status, "IMX678 sensor 2 check failed")) {
        return 1;
    } else {
        std::cout << "IMX678 sensor 2 check passed: " << static_cast<unsigned>(sensor_check_result_2.value) << '\n';
    }

    // IMX678 sensor 1 setup
    if (!status_check(imx678.init(), "Initial sensor IMX678 setup has failed")) {
        return 1;
    }

    // IMX678 sensor 2 setup
    if (!status_check(imx678_2.init(), "Initial sensor IMX678 (Sensor 2) setup has failed")) {
        return 1;
    }

    if (!status_check(roce.init_global(), "RoCE init failed")) {
        return 1;
    }

    for (const auto& qp : kQpParams) {
        status = roce.init_qp_uc(qp);
        if (!status.ok()) {
            std::cerr << "QP init failed (local_qp=" << qp.local_qp << "): " << status_code_to_string(status.code) << '\n';
            transport->disconnect();
            return 1;
        }
    }

    // Configure VVP-to-ABR bridge channels
    const std::array<hcl::drivers::AbrBridge::ChannelParams, 5> kChanParams = {{
        {
            .channel_width    = HCL_ABR_BRIDGE_CHAN_WIDTH_VVP420,
            .channel_type     = HCL_ABR_BRIDGE_CHAN_TYPE_VVP420,
            .num_lines        = 1080u,
            .roce_qp          = static_cast<std::uint8_t>(kQpParams[0].local_qp),
            .num_buffers      = 0u,
            .offset           = 0x0,
            .roce_rkeys       = {0u},
            .remote_addresses = {0x0},
            .metadata_address = 0x0,
            .metadata_key     = 0x0,
        },
        {
            .channel_width    = HCL_ABR_BRIDGE_CHAN_WIDTH_VVP420,
            .channel_type     = HCL_ABR_BRIDGE_CHAN_TYPE_VVP420,
            .num_lines        = 1080u,
            .roce_qp          = static_cast<std::uint8_t>(kQpParams[1].local_qp),
            .num_buffers      = 0u,
            .offset           = 0x0,
            .roce_rkeys       = {0u},
            .remote_addresses = {0x0},
            .metadata_address = 0x0,
            .metadata_key     = 0x0,
        },
        {
            .channel_width    = HCL_ABR_BRIDGE_CHAN_WIDTH_LIDAR,
            .channel_type     = HCL_ABR_BRIDGE_CHAN_TYPE_LIDAR,
            .num_lines        = 0u,
            .roce_qp          = static_cast<std::uint8_t>(kQpParams[2].local_qp),
            .num_buffers      = 0u,
            .offset           = 0x0,
            .roce_rkeys       = {0u},
            .remote_addresses = {0x0},
            .metadata_address = 0x0,
            .metadata_key     = 0x0,
        },
        {
            .channel_width    = HCL_ABR_BRIDGE_CHAN_WIDTH_LIDAR,
            .channel_type     = HCL_ABR_BRIDGE_CHAN_TYPE_LIDAR,
            .num_lines        = 0u,
            .roce_qp          = static_cast<std::uint8_t>(kQpParams[3].local_qp),
            .num_buffers      = 0u,
            .offset           = 0x0,
            .roce_rkeys       = {0u},
            .remote_addresses = {0x0},
            .metadata_address = 0x0,
            .metadata_key     = 0x0,
        },
        {
            .channel_width    = HCL_ABR_BRIDGE_CHAN_WIDTH_LIDAR,
            .channel_type     = HCL_ABR_BRIDGE_CHAN_TYPE_LIDAR,
            .num_lines        = 0u,
            .roce_qp          = static_cast<std::uint8_t>(kQpParams[4].local_qp),
            .num_buffers      = 0u,
            .offset           = 0x0,
            .roce_rkeys       = {0u},
            .remote_addresses = {0x0},
            .metadata_address = 0x0,
            .metadata_key     = 0x0,
        },
    }};

    const auto cfg_result = bridge.read_configuration();
    if (!cfg_result.status.ok()) {
        std::cerr << "VVP ABR read_configuration failed: " << status_code_to_string(cfg_result.status.code) << '\n';
        transport->disconnect();
        return 1;
    }

    for (std::size_t channel = 0; channel < kChanParams.size(); ++channel) {
        status = bridge.configure_channel(static_cast<std::uint32_t>(channel), kChanParams[channel]);
        if (!status.ok()) {
            std::cerr << "configure_channel(" << channel << ") failed: "
                      << status_code_to_string(status.code) << '\n';
            transport->disconnect();
            return 1;
        }
    }

    // Configure the chroma repackers
    std::vector<hcl::Operation> chroma_repack_ops = {
        {
            .operation = hcl::OperationType::Write32,
            .base_address = kVvpChromaRepackCsrBase,
            .offset = VVP_CHROMA_REPACK_IMG_INFO_WIDTH,
            .write_value = 1920u,
        },
        {
            .operation = hcl::OperationType::Write32,
            .base_address = kVvpChromaRepackCsrBase,
            .offset = VVP_CHROMA_REPACK_IMG_INFO_HEIGHT,
            .write_value = 1080u,
        },
        {
            .operation = hcl::OperationType::Write32,
            .base_address = kVvpChromaRepackCsrBase2,
            .offset = VVP_CHROMA_REPACK_IMG_INFO_WIDTH,
            .write_value = 1920u,
        },
        {
            .operation = hcl::OperationType::Write32,
            .base_address = kVvpChromaRepackCsrBase2,
            .offset = VVP_CHROMA_REPACK_IMG_INFO_HEIGHT,
            .write_value = 1080u,
        },
    };

    if (!status_check(reg->execute(chroma_repack_ops), "Chroma repacker configuration failed")) {
        return 1;
    }

    // Enable ABR channels
    for (std::size_t channel = 0; channel < kChanParams.size(); ++channel) {
        status = bridge.enable_channel(static_cast<std::uint32_t>(channel));
        if (!status.ok()) {
            std::cerr << "enable_channel(" << channel << ") failed: "
                      << status_code_to_string(status.code) << '\n';
            transport->disconnect();
            return 1;
        }
    }

    // VVP switch setup
    if (!status_check(vvp_switch.init(), "VVP switch init failed")) {
        return 1;
    }
    if (!status_check(vvp_switch.set_input_config(0u, HCL_VVP_SWITCH_INPUT_CONSUMED), "VVP switch set_input_config(0) failed")) {
        return 1;
    }
    if (!status_check(vvp_switch.set_input_config(1u, HCL_VVP_SWITCH_INPUT_CONSUMED), "VVP switch set_input_config(1) failed")) {
        return 1;
    }
    if (!status_check(vvp_switch.set_input_config(2u, HCL_VVP_SWITCH_INPUT_ENABLED), "VVP switch set_input_config(2) failed")) {
        return 1;
    }
    if (!status_check(vvp_switch.set_input_config(3u, HCL_VVP_SWITCH_INPUT_ENABLED), "VVP switch set_input_config(3) failed")) {
        return 1;
    }
    if (!status_check(vvp_switch.set_output_config(0u, true, 2u), "VVP switch set_output_config(0) failed")) {
        return 1;
    }
    if (!status_check(vvp_switch.set_output_config(1u, true, 3u), "VVP switch set_output_config(1) failed")) {
        return 1;
    }
    if (!status_check(vvp_switch.commit(), "VVP switch commit failed")) {
        return 1;
    }

    // Configure VVP throttle for both sensor channels

    std::vector<hcl::Operation> throttle_ops = {
        { .operation = hcl::OperationType::Read32,  .base_address = kVvpThrottleCsrBase,  .offset = VVP_THROTTLE_STATUS_REG_ADDR },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase,  .offset = VVP_THROTTLE_IMG_INFO_WIDTH,            .write_value = 1920u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase,  .offset = VVP_THROTTLE_IMG_INFO_HEIGHT,           .write_value = 1080u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase,  .offset = VVP_THROTTLE_IMG_INFO_SUBSAMPLING,      .write_value = 3u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase,  .offset = VVP_THROTTLE_CYCLES_PER_LINE_REG_ADDR,  .write_value = 4400u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase,  .offset = VVP_THROTTLE_BLANK_LINES_REG_ADDR,      .write_value = 45u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase,  .offset = VVP_THROTTLE_UPDATE_SETTINGS_REG_ADDR,  .write_value = 1u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase,  .offset = VVP_THROTTLE_CONTROL_REG_ADDR,          .write_value = 1u },

        { .operation = hcl::OperationType::Read32,  .base_address = kVvpThrottleCsrBase2, .offset = VVP_THROTTLE_STATUS_REG_ADDR },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase2, .offset = VVP_THROTTLE_IMG_INFO_WIDTH,           .write_value = 1920u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase2, .offset = VVP_THROTTLE_IMG_INFO_HEIGHT,          .write_value = 1080u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase2, .offset = VVP_THROTTLE_IMG_INFO_SUBSAMPLING,     .write_value = 3u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase2, .offset = VVP_THROTTLE_CYCLES_PER_LINE_REG_ADDR, .write_value = 4400u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase2, .offset = VVP_THROTTLE_BLANK_LINES_REG_ADDR,     .write_value = 45u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase2, .offset = VVP_THROTTLE_UPDATE_SETTINGS_REG_ADDR, .write_value = 1u },
        { .operation = hcl::OperationType::Write32, .base_address = kVvpThrottleCsrBase2, .offset = VVP_THROTTLE_CONTROL_REG_ADDR,         .write_value = 1u },
    };

    if (!status_check(reg->execute(throttle_ops), "VVP throttle configuration failed")) {
        return 1;
    }

    // ISP pipeline 1 setup
    if (!status_check(crs.init(), "CRS init failed")) {
        return 1;
    }
    if (!status_check(crs.set_img_info_width(1920u), "CRS set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(crs.set_img_info_height(1080u), "CRS set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(crs.set_img_info_interlace(3u), "CRS set_img_info_interlace failed")) {
        return 1;
    }
    if (!status_check(crs.set_img_info_colorspace(IMG_INFO_COLORSPACE_YCbCr), "CRS set_img_info_colorspace failed")) {
        return 1;
    }
    if (!status_check(crs.set_img_info_subsampling(IMG_INFO_SUBSAMPLING_444), "CRS set_img_info_subsampling failed")) {
        return 1;
    }
    if (!status_check(crs.set_img_info_cositing(0u), "CRS set_img_info_cositing failed")) {
        return 1;
    }
    if (!status_check(crs.set_output_subsampling(HCL_VVP_CRS_SUBSAMPLING_MODE_420), "CRS set_output_subsampling failed")) {
        return 1;
    }
    if (!status_check(crs.commit(), "CRS commit failed")) {
        return 1;
    }

    if (!status_check(scaler.init(), "Scaler init failed")) {
        return 1;
    }
    if (!status_check(scaler.set_img_info_width(INIT_HSIZE), "Scaler set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(scaler.set_img_info_height(INIT_VSIZE), "Scaler set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(scaler.set_img_info_interlace(3u), "Scaler set_img_info_interlace failed")) {
        return 1;
    }
    if (!status_check(scaler.set_img_info_colorspace(IMG_INFO_COLORSPACE_RGB), "Scaler set_img_info_colorspace failed")) {
        return 1;
    }
    if (!status_check(scaler.set_img_info_subsampling(IMG_INFO_SUBSAMPLING_444), "Scaler set_img_info_subsampling failed")) {
        return 1;
    }
    if (!status_check(scaler.set_img_info_cositing(0u), "Scaler set_img_info_cositing failed")) {
        return 1;
    }
    if (!status_check(scaler.set_output_width(1920u), "Scaler set_output_width failed")) {
        return 1;
    }
    if (!status_check(scaler.set_output_height(1080u), "Scaler set_output_height failed")) {
        return 1;
    }
    if (!status_check(scaler.commit(), "Scaler commit failed")) {
        return 1;
    }

    if (!status_check(csc.init(), "CSC init failed")) {
        return 1;
    }
    if (!status_check(csc.set_img_info_width(INIT_HSIZE), "CSC set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(csc.set_img_info_height(INIT_VSIZE), "CSC set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(csc.set_img_info_interlace(3u), "CSC set_img_info_interlace failed")) {
        return 1;
    }
    if (!status_check(csc.set_img_info_colorspace(IMG_INFO_COLORSPACE_RGB), "CSC set_img_info_colorspace failed")) {
        return 1;
    }
    if (!status_check(csc.set_img_info_subsampling(IMG_INFO_SUBSAMPLING_444), "CSC set_img_info_subsampling failed")) {
        return 1;
    }
    if (!status_check(csc.set_img_info_cositing(0u), "CSC set_img_info_cositing failed")) {
        return 1;
    }
    if (!status_check(csc.set_coeff_data(ccm_4000k, 10), "CSC set_coeff_data failed")) {
        return 1;
    }
    if (!status_check(csc.commit(), "CSC commit failed")) {
        return 1;
    }

    if (!status_check(demosaic.init(), "Demosaic init failed")) {
        return 1;
    }
    if (!status_check(demosaic.set_img_info_width(INIT_HSIZE), "Demosaic set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(demosaic.set_img_info_height(INIT_VSIZE), "Demosaic set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(demosaic.set_cfa_phase(BAYER_PHASE), "Demosaic set_cfa_phase failed")) {
        return 1;
    }
    if (!status_check(demosaic.set_bypass(false), "Demosaic set_bypass failed")) {
        return 1;
    }

    if (!status_check(wbc.init(), "WBC init failed")) {
        return 1;
    }
    if (!status_check(wbc.set_img_info_width(INIT_HSIZE), "WBC set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(wbc.set_img_info_height(INIT_VSIZE), "WBC set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(wbc.set_cfa_00_color_scaler(3236u), "WBC set_cfa_00_color_scaler failed")) {
        return 1;
    }
    if (!status_check(wbc.set_cfa_01_color_scaler(2048u), "WBC set_cfa_01_color_scaler failed")) {
        return 1;
    }
    if (!status_check(wbc.set_cfa_10_color_scaler(2048u), "WBC set_cfa_10_color_scaler failed")) {
        return 1;
    }
    if (!status_check(wbc.set_cfa_11_color_scaler(4687u), "WBC set_cfa_11_color_scaler failed")) {
        return 1;
    }
    if (!status_check(wbc.set_cfa_phase(BAYER_PHASE), "WBC set_cfa_phase failed")) {
        return 1;
    }
    if (!status_check(wbc.set_bypass(false), "WBC set_bypass failed")) {
        return 1;
    }
    if (!status_check(wbc.commit(), "WBC commit failed")) {
        return 1;
    }

    if (!status_check(blc.init(), "BLC init failed")) {
        return 1;
    }
    if (!status_check(blc.set_img_info_width(INIT_HSIZE), "BLC set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(blc.set_img_info_height(INIT_VSIZE), "BLC set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(blc.set_cfa_00_black_pedestal(200u), "BLC set_cfa_00_black_pedestal failed")) {
        return 1;
    }
    if (!status_check(blc.set_cfa_00_color_scaler(13460u), "BLC set_cfa_00_color_scaler failed")) {
        return 1;
    }
    if (!status_check(blc.set_cfa_01_black_pedestal(200u), "BLC set_cfa_01_black_pedestal failed")) {
        return 1;
    }
    if (!status_check(blc.set_cfa_01_color_scaler(13460u), "BLC set_cfa_01_color_scaler failed")) {
        return 1;
    }
    if (!status_check(blc.set_cfa_10_black_pedestal(200u), "BLC set_cfa_10_black_pedestal failed")) {
        return 1;
    }
    if (!status_check(blc.set_cfa_10_color_scaler(13460u), "BLC set_cfa_10_color_scaler failed")) {
        return 1;
    }
    if (!status_check(blc.set_cfa_11_black_pedestal(200u), "BLC set_cfa_11_black_pedestal failed")) {
        return 1;
    }
    if (!status_check(blc.set_cfa_11_color_scaler(13460u), "BLC set_cfa_11_color_scaler failed")) {
        return 1;
    }
    if (!status_check(blc.set_cfa_phase(BAYER_PHASE), "BLC set_cfa_phase failed")) {
        return 1;
    }
    if (!status_check(blc.set_bypass(false), "BLC set_bypass failed")) {
        return 1;
    }
    if (!status_check(blc.commit(), "BLC commit failed")) {
        return 1;
    }

    // ISP pipeline 2 setup (identical configuration)
    if (!status_check(crs2.init(), "CRS2 init failed")) {
        return 1;
    }
    if (!status_check(crs2.set_img_info_width(1920u), "CRS2 set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(crs2.set_img_info_height(1080u), "CRS2 set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(crs2.set_img_info_interlace(3u), "CRS2 set_img_info_interlace failed")) {
        return 1;
    }
    if (!status_check(crs2.set_img_info_colorspace(IMG_INFO_COLORSPACE_YCbCr), "CRS2 set_img_info_colorspace failed")) {
        return 1;
    }
    if (!status_check(crs2.set_img_info_subsampling(IMG_INFO_SUBSAMPLING_444), "CRS2 set_img_info_subsampling failed")) {
        return 1;
    }
    if (!status_check(crs2.set_img_info_cositing(0u), "CRS2 set_img_info_cositing failed")) {
        return 1;
    }
    if (!status_check(crs2.set_output_subsampling(HCL_VVP_CRS_SUBSAMPLING_MODE_420), "CRS2 set_output_subsampling failed")) {
        return 1;
    }
    if (!status_check(crs2.commit(), "CRS2 commit failed")) {
        return 1;
    }

    if (!status_check(scaler2.init(), "Scaler2 init failed")) {
        return 1;
    }
    if (!status_check(scaler2.set_img_info_width(INIT_HSIZE), "Scaler2 set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(scaler2.set_img_info_height(INIT_VSIZE), "Scaler2 set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(scaler2.set_img_info_interlace(3u), "Scaler2 set_img_info_interlace failed")) {
        return 1;
    }
    if (!status_check(scaler2.set_img_info_colorspace(IMG_INFO_COLORSPACE_RGB), "Scaler2 set_img_info_colorspace failed")) {
        return 1;
    }
    if (!status_check(scaler2.set_img_info_subsampling(IMG_INFO_SUBSAMPLING_444), "Scaler2 set_img_info_subsampling failed")) {
        return 1;
    }
    if (!status_check(scaler2.set_img_info_cositing(0u), "Scaler2 set_img_info_cositing failed")) {
        return 1;
    }
    if (!status_check(scaler2.set_output_width(1920u), "Scaler2 set_output_width failed")) {
        return 1;
    }
    if (!status_check(scaler2.set_output_height(1080u), "Scaler2 set_output_height failed")) {
        return 1;
    }
    if (!status_check(scaler2.commit(), "Scaler2 commit failed")) {
        return 1;
    }

    if (!status_check(csc2.init(), "CSC2 init failed")) {
        return 1;
    }
    if (!status_check(csc2.set_img_info_width(INIT_HSIZE), "CSC2 set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(csc2.set_img_info_height(INIT_VSIZE), "CSC2 set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(csc2.set_img_info_interlace(3u), "CSC2 set_img_info_interlace failed")) {
        return 1;
    }
    if (!status_check(csc2.set_img_info_colorspace(IMG_INFO_COLORSPACE_RGB), "CSC2 set_img_info_colorspace failed")) {
        return 1;
    }
    if (!status_check(csc2.set_img_info_subsampling(IMG_INFO_SUBSAMPLING_444), "CSC2 set_img_info_subsampling failed")) {
        return 1;
    }
    if (!status_check(csc2.set_img_info_cositing(0u), "CSC2 set_img_info_cositing failed")) {
        return 1;
    }
    if (!status_check(csc2.set_coeff_data(ccm_4000k, 10), "CSC2 set_coeff_data failed")) {
        return 1;
    }
    if (!status_check(csc2.commit(), "CSC2 commit failed")) {
        return 1;
    }

    if (!status_check(demosaic2.init(), "Demosaic2 init failed")) {
        return 1;
    }
    if (!status_check(demosaic2.set_img_info_width(INIT_HSIZE), "Demosaic2 set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(demosaic2.set_img_info_height(INIT_VSIZE), "Demosaic2 set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(demosaic2.set_cfa_phase(BAYER_PHASE), "Demosaic2 set_cfa_phase failed")) {
        return 1;
    }
    if (!status_check(demosaic2.set_bypass(false), "Demosaic2 set_bypass failed")) {
        return 1;
    }

    if (!status_check(wbc2.init(), "WBC2 init failed")) {
        return 1;
    }
    if (!status_check(wbc2.set_img_info_width(INIT_HSIZE), "WBC2 set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(wbc2.set_img_info_height(INIT_VSIZE), "WBC2 set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(wbc2.set_cfa_00_color_scaler(3236u), "WBC2 set_cfa_00_color_scaler failed")) {
        return 1;
    }
    if (!status_check(wbc2.set_cfa_01_color_scaler(2048u), "WBC2 set_cfa_01_color_scaler failed")) {
        return 1;
    }
    if (!status_check(wbc2.set_cfa_10_color_scaler(2048u), "WBC2 set_cfa_10_color_scaler failed")) {
        return 1;
    }
    if (!status_check(wbc2.set_cfa_11_color_scaler(4687u), "WBC2 set_cfa_11_color_scaler failed")) {
        return 1;
    }
    if (!status_check(wbc2.set_cfa_phase(BAYER_PHASE), "WBC2 set_cfa_phase failed")) {
        return 1;
    }
    if (!status_check(wbc2.set_bypass(false), "WBC2 set_bypass failed")) {
        return 1;
    }
    if (!status_check(wbc2.commit(), "WBC2 commit failed")) {
        return 1;
    }

    if (!status_check(blc2.init(), "BLC2 init failed")) {
        return 1;
    }
    if (!status_check(blc2.set_img_info_width(INIT_HSIZE), "BLC2 set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(blc2.set_img_info_height(INIT_VSIZE), "BLC2 set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(blc2.set_cfa_00_black_pedestal(200u), "BLC2 set_cfa_00_black_pedestal failed")) {
        return 1;
    }
    if (!status_check(blc2.set_cfa_00_color_scaler(13460u), "BLC2 set_cfa_00_color_scaler failed")) {
        return 1;
    }
    if (!status_check(blc2.set_cfa_01_black_pedestal(200u), "BLC2 set_cfa_01_black_pedestal failed")) {
        return 1;
    }
    if (!status_check(blc2.set_cfa_01_color_scaler(13460u), "BLC2 set_cfa_01_color_scaler failed")) {
        return 1;
    }
    if (!status_check(blc2.set_cfa_10_black_pedestal(200u), "BLC2 set_cfa_10_black_pedestal failed")) {
        return 1;
    }
    if (!status_check(blc2.set_cfa_10_color_scaler(13460u), "BLC2 set_cfa_10_color_scaler failed")) {
        return 1;
    }
    if (!status_check(blc2.set_cfa_11_black_pedestal(200u), "BLC2 set_cfa_11_black_pedestal failed")) {
        return 1;
    }
    if (!status_check(blc2.set_cfa_11_color_scaler(13460u), "BLC2 set_cfa_11_color_scaler failed")) {
        return 1;
    }
    if (!status_check(blc2.set_cfa_phase(BAYER_PHASE), "BLC2 set_cfa_phase failed")) {
        return 1;
    }
    if (!status_check(blc2.set_bypass(false), "BLC2 set_bypass failed")) {
        return 1;
    }
    if (!status_check(blc2.commit(), "BLC2 commit failed")) {
        return 1;
    }

    // Input switch (VVP-Full) 2x1
    if (!status_check(input_switch.init(), "Input switch init failed")) {
        return 1;
    }
    if (!status_check(input_switch.set_input_config(TPG_BAYER_SWITCH_IN, HCL_VVP_SWITCH_INPUT_CONSUMED), "Input switch set_input_config(TPG) failed")) {
        return 1;
    }
    if (!status_check(input_switch.set_input_config(MIPI_BAYER_SWITCH_IN, HCL_VVP_SWITCH_INPUT_ENABLED), "Input switch set_input_config(MIPI1) failed")) {
        return 1;
    }
    if (!status_check(input_switch.set_input_config(MIPI2_BAYER_SWITCH_IN, HCL_VVP_SWITCH_INPUT_ENABLED), "Input switch set_input_config(MIPI2) failed")) {
        return 1;
    }
    if (!status_check(input_switch.set_output_config(BAYER_SWITCH_OUT, true, MIPI_BAYER_SWITCH_IN), "Input switch set_output_config(OUT2) failed")) {
        return 1;
    }
    if (!status_check(input_switch.set_output_config(BAYER_SWITCH_OUT2, true, MIPI2_BAYER_SWITCH_IN), "Input switch set_output_config(OUT) failed")) {
        return 1;
    }
    if (!status_check(input_switch.commit(), "Input switch commit failed")) {
        return 1;
    }

    if (!status_check(reg->write32(0x22e600u, 0x04u, 0x94u), "RMS write failed")) {
        return 1;
    }

    // 10-bit TPG for base layer with solid colour + colour bars
    if (!status_check(input_tpg.init(), "Input TPG init failed")) {
        return 1;
    }
    if (!status_check(input_tpg.stop(), "Input TPG stop failed")) {
        return 1;
    }
    if (!status_check(input_tpg.set_img_info_width(INIT_HSIZE), "Input TPG set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(input_tpg.set_img_info_height(INIT_VSIZE), "Input TPG set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(input_tpg.set_img_info_interlace(3u), "Input TPG set_img_info_interlace failed")) {
        return 1;
    }
    if (!status_check(input_tpg.set_pattern(0u), "Input TPG set_pattern failed")) {
        return 1;
    }
    if (!status_check(input_tpg.set_bars_type(HCL_VVP_TPG_BARS_TYPE_COLOR), "Input TPG set_bars_type failed")) {
        return 1;
    }
    if (!status_check(input_tpg.set_colors(0u, 4095u, 0u), "Input TPG set_colors failed")) {
        return 1;
    }
    if (!status_check(input_tpg.commit(), "Input TPG commit failed")) {
        return 1;
    }
    if (!status_check(input_tpg.start(), "Input TPG start failed")) {
        return 1;
    }

    // PiP converter MIPI Rx
    if (!status_check(pip_conv_mipi.init(), "PiP converter MIPI init failed")) {
        return 1;
    }
    if (!status_check(pip_conv_mipi.set_img_info_width(INIT_HSIZE), "PiP converter MIPI set_img_info_width failed")) {
        return 1;
    }
    if (!status_check(pip_conv_mipi.set_img_info_height(INIT_VSIZE), "PiP converter MIPI set_img_info_height failed")) {
        return 1;
    }
    if (!status_check(pip_conv_mipi.set_img_info_interlace(3u), "PiP converter MIPI set_img_info_interlace failed")) {
        return 1;
    }
    if (!status_check(pip_conv_mipi.set_img_info_colorspace(IMG_INFO_COLORSPACE_RGB), "PiP converter MIPI set_img_info_colorspace failed")) {
        return 1;
    }
    if (!status_check(pip_conv_mipi.set_img_info_subsampling(IMG_INFO_SUBSAMPLING_444), "PiP converter MIPI set_img_info_subsampling failed")) {
        return 1;
    }
    if (!status_check(pip_conv_mipi.set_img_info_cositing(0u), "PiP converter MIPI set_img_info_cositing failed")) {
        return 1;
    }

    transport->disconnect();

    return 0;
}
