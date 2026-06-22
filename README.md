# LCC_8Output40Actions_8Input_Node

Here is the complete, raw Markdown code for your `README.md` file contained inside a single, easy-to-copy block.

You can copy everything inside this block and paste it directly into a file named `README.md` in the root of your GitHub repository:


# OpenLCB / LCC 8-Input & 8-Output ESP32 Node

An OpenLCB (LCC) compliant layout automation node built for the **ESP32 DevKit v1**. This project combines high-flexibility lighting effects across 8 output channels with precise input sensor/button debounce and delay tracking across 8 input channels.

It utilizes CAN bus architecture for robust model railroad layout control or general automation setups via standard OpenLCB configuration software (such as JMRI).

---

##  Features

### 8 Output Channels (Pins 4 to 23)
Supports up to **40 programmable Action Slots** assigned dynamically to physical output pins.
* **Low / High:** Steady-state on/off control.
* **Flash:** Alternates high/low with customizable high and low durations (100ms to 25.5s).
* **Double Strobe:** Double pulses outputs (great for warning beacons or emergency vehicles).
* **Random:** High/low organic variance. Perfect for simulating welding arcs or flickering house lighting.
* **Fire:** Couples consecutive pins using PWM (`analogWrite`) to create realistic embers or flickering building fire effects.

### 8 Input Channels (Pins 13 to 32)
* **Multiple Channel Types:** Standard Input, Inverted Input, Internal Pull-Up tracking, and Input Toggle state flipping.
* **On/Off Delay Controls:** Configurable state delays via software sliders (100ms to 25.5s) to filter out track bouncing, sensor noise, or dirty wheels.

---

## 🛠 Hardware Configuration

The pin mapping is optimized for a standard **ESP32 DevKit v1** paired with an external CAN transceiver (e.g., SN65HVD230).

| Function | ESP32 GPIO Pin | Description |
| :--- | :--- | :--- |
| **CAN RX** | `GPIO 15` | Connect to Transceiver RXD |
| **CAN TX** | `GPIO 2` | Connect to Transceiver TXD |
| **Outputs (1 - 8)** | `4, 16, 17, 18, 19, 21, 22, 23` | High/Low and PWM lighting effect pins |
| **Inputs (1 - 8)** | `13, 12, 14, 27, 26, 25, 33, 32` | Pushbuttons, toggle switches, or block detectors |

---

##  Software Dependencies

To compile this project, ensure you have the following libraries installed in your Arduino IDE environment:

1. **ACAN_ESP32Can** – Active CAN controller driver library optimized for ESP32 hardware.
2. **OpenLcbCore / OpenLCB** – Core LCC/OpenLCB layout control automation stack library.

---

##  Installation & First-Time Setup

1. Clone or download this repository into your Arduino sketchbook directory.
2. Open `Config.h` and verify or update your unique `NODE_ADDRESS`:
   ```cpp
   #define NODE_ADDRESS  5,1,1,1,0x8E,0x01


3. Set `RESET_TO_FACTORY_DEFAULTS` to `1` for your initial upload:

#define RESET_TO_FACTORY_DEFAULTS 1

4. Choose **ESP32 Dev Module** in your Arduino IDE boards manager, select your COM port, and click **Upload**.
5. *Optional:* Once the node boots and executes its first wipe cycle successfully, change `RESET_TO_FACTORY_DEFAULTS` back to `0` and upload again to ensure custom settings survive future reboots.

##  Configuration via JMRI

Because this sketch incorporates a full **CDI (Configuration Description Information)** XML block, it is completely plug-and-play.

1. Connect your node to an active LCC/OpenLCB CAN Bus network linked to your computer.
2. Open **JMRI** and navigate to **Tools > Layout Command Control > Configure Nodes**.
3. Locate your node (`8 In / 8 Out Node`) in the list.
4. Open the configuration window to intuitively label channels, pick input logic behaviors, adjust delay timing sliders, and assign unique OpenLCB Event IDs directly into the node's onboard EEPROM.

---

##  Authors & Acknowledgments

* Based on foundational OpenLCB examples by **Bob Jacobsen** and **Alex Shepherd**.
* Original hardware control schemes adapted by **David Harris**.
* Merged, refactored, and optimized for dual-mode I/O deployment by **John Holmes**.

```


