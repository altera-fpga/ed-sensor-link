# External GTS Reset

The External GTS Reset subsystem is the reset sequencer shared by the two Ethernet PHYs. The GTS Reset Sequencer produces the PMA clock and the reset grant. The reset split sends that grant to the 10G Ethernet MAC and to the Ethernet Ingest PHY. Each PHY returns a reset request. This subsystem has no processor and is not on the Control memory map.

**IP in this subsystem**

| IP | Function |
| --- | --- |
| GTS Reset Sequencer | `eth_srcss_gts`. Produces the PMA clock and the reset grant. |
| GTS Reset Sequencer Split | `gts_rst_split`. Sends the PMA clock and reset grant to the 10G MAC and the Ethernet Ingest PHY, and returns each PHY's reset request. |

![External GTS Reset block diagram](../images/figure-28-external-gts-reset-block-diagram.png)

*Figure 28. External GTS Reset block diagram.*

---

[← Ethernet Ingest Subsystem](09-ethernet-ingest-subsystem.md) | [Documentation index](../README.md) | [Timestamp Subsystem →](11-timestamp-subsystem.md)
