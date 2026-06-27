#ifndef CONFIG_H
#define CONFIG_H

// To set a new nodeid edit the next line
#define NODE_ADDRESS  0x05,0x01,0x01,0x01,0x8E,0x03 // must be unique from an address space owned by you or DIY

// To Force Reset EEPROM to Factory Defaults set this value to 1, else 0.
// Need to do this at least once.
#define RESET_TO_FACTORY_DEFAULTS 1

#define ESP32_BOARD // Needed for sketch to work. We only offer this for the ESP32 Devkit 1

/* Debugging -- uncomment to activate debugging statements: */
//#define DEBUG Serial

// CAN Interface Transceiver Pin Bindings
#define CAN_RX_PIN (gpio_num_t) 15
#define CAN_TX_PIN (gpio_num_t) 2

// ====================== OUTPUTS / ACTIONS CONFIGURATION ======================
#define NUM_CHANNEL       8     // 8 physical output lines
#define NUM_ACTION        40    // Total configuration actions allocated to outputs
#define NUM_ACTION_SETS   4     // Number of groups in CDI
#define NUM_APS           10    // Actions Per Set (group)

// Global state trackers for output actions
uint8_t currentEvent[NUM_CHANNEL]; 
uint8_t state[NUM_CHANNEL];        
long    timer[NUM_CHANNEL];        

inline int getActionIndex(int group, int action) {
    return group * NUM_APS + action;
}

// Physical Output Pin Array (First 8 pins)
uint8_t pin[NUM_CHANNEL] = {4, 16, 17, 18, 19, 21, 22, 23};


// ====================== INPUTS CONFIGURATION =================================
#define NUM_IO            8     // 8 physical input lines

// Physical Input Pins (Last 8 pins)
#define IOPINS            13, 12, 14, 27, 26, 25, 33, 32


// ====================== OPENLCB EVENT CONFIGURATION ==========================
// NUM_EVENT = 40 Consumer Actions + (2 Events per Input * 8 Inputs = 16) = 56 Total
#define NUM_EVENT         (NUM_ACTION + (2 * NUM_IO)) 


// ====================== EEPROM storage allocation ============================
#define EEPROMSIZE 4096
#define EEPROMbegin { EEPROM.begin(EEPROMSIZE); dP("\nEEPROM begin "); dP(EEPROMSIZE); }
#define EEPROMcommit { EEPROM.commit(); dP("EEPROM COMMIT"); }

// Board definitions metadata
#define MANU "OpenLCB"
#define MODEL "8 In / 8 Out Node"
#define HWVERSION "1.1"
#define SWVERSION "1.1"
