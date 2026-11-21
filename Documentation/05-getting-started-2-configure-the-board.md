# Getting Started: Configure the Board

This is part 2 of 3 of Getting Started: [Hardware Connection](04-getting-started-1-hardware-connection.md), [Configure the Board](05-getting-started-2-configure-the-board.md), [Execute the Control Plane](06-getting-started-3-execute-the-control-plane.md).

## Configure the board

Once all connections are ready according to the selected hardware configuration and SOF, follow these steps:

1. Get [Quartus Programmer 26.1](https://www.altera.com/downloads/fpga-development-tools/quartus-prime-pro-edition-design-software-version-26-1-windows) from the Standalone Software section of the Altera® download page. Direct download: [QuartusProProgrammerSetup-26.1.0.110-windows.exe](https://www.altera.com/download-center/license-agreement/127231/4e7f616c20e1954783e8d9971c0503cab69483c6?filename=QuartusProProgrammerSetup-26.1.0.110-windows.exe).
2. Power on the lidars and the Framos GMSL module first. **This step is important!**
3. Turn on the development kit.
4. Open the Programmer, click Hardware Setup, select `Agilex_5E` MDK Carrier [USB-1], and close the dialog.

    <p align="center">
      <img src="images/figure-13-programmer-hardware-setup.png" alt="Programmer hardware setup">
    </p>

    <p align="center"><strong>Figure 13. Programmer hardware setup.</strong></p>

    <br>

5. In the main window, click Auto Detect. The device should appear in the JTAG chain as A5ED065BB32AR0 / A5ED065BB32AE4S. Select it, choose Change File, select the SOF, and make sure Program/Configure is enabled.

    <p align="center">
      <img src="images/figure-14-programming-the-board.png" alt="Programming the board">
    </p>

    <p align="center"><strong>Figure 14. Programming the board.</strong></p>

    <br>

6. Click Program. No RoCEv2 packets are sent at this point. You must use the control plane to start the streams.

---

[← Getting Started: Hardware Connection](04-getting-started-1-hardware-connection.md) | [Documentation index](README.md) | [Getting Started: Execute the Control Plane →](06-getting-started-3-execute-the-control-plane.md)
