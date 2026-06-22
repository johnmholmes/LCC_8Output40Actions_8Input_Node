/*
**Disclaimer and Limitation of Liability**

This sketch (software) has been developed specifically for the **M5Stack Atom Lite** and the **M5Stack MiniCan** CAN transceiver module. It has only been tested on the author’s personal model railway layout.

**The sketch is provided “AS IS” and “AS AVAILABLE”**, without any warranties or guarantees of any kind. The author explicitly disclaims all warranties, whether express, implied, or statutory, including but not limited to any warranties of merchantability, fitness for a particular purpose, accuracy, reliability, or non-infringement.

The author accepts **no responsibility or liability** for:
- Any malfunction, failure, or unexpected behaviour of the sketch
- Damage to hardware, loss of data, or disruption to your layout
- Incompatibility caused by updates to third-party libraries, Arduino core, JMRI, or other software
- Any direct, indirect, incidental, consequential, or punitive damages arising from the use or inability to use this sketch

This code is offered strictly for **educational and hobbyist purposes** to help railway modellers learn how to use the OpenLCB Single Thread Library. It is not intended for commercial use, safety-critical applications, or any situation where failure could cause damage or injury.

By downloading, using, or modifying this sketch, you acknowledge that you assume **all risk** and full responsibility for any outcomes resulting from its use.

The author reserves the right to modify or remove this sketch at any time without notice.
*/


/* ESP32 8-Input & 8-Output Combined Node
   ==============================================================
   - 8 Output channels with multi-effects (Flash, Strobe, Random, Fire)
   - 8 Input channels with varying types (Inverted, Pull-up, Toggle) and Delays
   - CAN Transceiver Pins: RX pin 15, TX pin 2
   ==============================================================
*/

#include "Config.h"         // Contains configuration, see "Config.h"
#include "ACAN_ESP32Can.h"  // Uses ACan class
#include "HouseKeeping.h"   // Development system tools
#include "mdebugging.h"     // Debugging macros
#include "processor.h"      // Auto-selects processor and environment configuration
#include "OpenLcbCore.h"
#include "OpenLCBHeader.h"  // System house-keeping.

#define OLCB_NO_BLUE_GOLD

extern "C" {        
#define N(x) xN(x)  // Allows stringification of definitions inside XML
#define xN(x) #x    

  // ===== MERGED CDI ===== 
  const char configDefInfo[] PROGMEM =
    CDIheader R"(
      <name>Application Configuration</name>
      
      <group replication="4">
        <name>Action Groups</name>
        <hints><visibility hideable='yes' hidden='yes' ></visibility></hints>
        <description>Choose an Action group depending on your requirements. You have 40 Actions that can be spread across the 8 pins.</description>
        <repname>Actions 1-10</repname>
        <repname>Actions 11-20</repname>
        <repname>Actions 21-30</repname>
        <repname>Actions 31-40</repname>

        <group replication="10">
          <name>Actions</name>
          <string size='24'><name>Description</name></string>
          <repname>Action </repname>
          <description>Define what a pin can do when a consumed event is received.</description>
          <eventid><name>Event for action</name></eventid>

          <int size="1">
            <name>Choose the pin number you want to use the action on. Note these pins are on the righthand side of the sensor shield.</name>
            <map>
              <relation><property>0</property><value>None</value></relation>
              <relation><property>1</property><value>4</value></relation>
              <relation><property>2</property><value>16</value></relation>
              <relation><property>3</property><value>17</value></relation>
              <relation><property>4</property><value>18</value></relation>
              <relation><property>5</property><value>19</value></relation>
              <relation><property>6</property><value>21</value></relation>
              <relation><property>7</property><value>22</value></relation>
              <relation><property>8</property><value>23</value></relation>
            </map>
          </int>

          <int size="1">
            <name>Choose the action type you want to use for this pin. Fire uses the next pin for extra effects</name>
            <map>
              <relation><property>0</property><value>None</value></relation>
              <relation><property>1</property><value>Low</value></relation>
              <relation><property>2</property><value>High</value></relation>
              <relation><property>3</property><value>Flash</value></relation>
              <relation><property>4</property><value>Double Strobe</value></relation>
              <relation><property>5</property><value>Random</value></relation>
              <relation><property>6</property><value>Fire (uses two pins)</value></relation>
            </map>
          </int>

          <int size="1">
<<<<<<< HEAD
            <name>Parameter 1 On-delay 0=steady-state, 1-255 = from 100ms to 25.5 seconds</name>
=======
            <name>O = stead state On, 1 gives you 1*100 = 100ms 255 gives you 255*100 = 25.5 second on duration</name>
>>>>>>> 101e11cf0f733f584f6be34488b585f349e585cb
            <hints><slider tickSpacing='85' immediate='yes' showValue='yes'> </slider></hints>
          </int>

          <int size="1">
<<<<<<< HEAD
            <name>Parameter 2 Off-delay 0=No repeat, 1-255 = from 100msto 25.5 seconds</name>
=======
            <name>Off-delay 0 = No repeat, 1 gives you 1*100 = 100ms 255 gives you 255*100 = 25.5 second off duration </name>
>>>>>>> 101e11cf0f733f584f6be34488b585f349e585cb
            <hints><slider tickSpacing='85' immediate='yes' showValue='yes'> </slider></hints>
          </int>
        </group>
      </group>

      <group replication=')" N(NUM_IO) R"('>
        <name>Inputs choose the pin that you wish to use. Note these are on the lefthand side of the sensor shield</name>
        <repname>Input 13</repname>
        <repname>Input 12</repname>
        <repname>Input 14</repname>
        <repname>Input 27</repname>
        <repname>Input 26</repname>
        <repname>Input 25</repname>
        <repname>Input 33</repname>
        <repname>Input 32</repname>
        <string size='24'><name>Description</name></string>
        <int size='1'>
          <name>Choose from the 7 Channel Types in the dropdown for how the input works.</name>
          <map>
            <relation><property>0</property><value>None</value></relation> 
            <relation><property>1</property><value>Input</value></relation> 
            <relation><property>2</property><value>Input Inverted</value></relation> 
            <relation><property>3</property><value>Input with pull-up</value></relation>
            <relation><property>4</property><value>Input with pull-up, Inverted</value></relation>
            <relation><property>5</property><value>Toggle</value></relation>
            <relation><property>6</property><value>Toggle with pull-up</value></relation>
          </map>
        </int>
        <int size='1'>
<<<<<<< HEAD
          <name>On-delay 0=steady-state, 1-255 = from 100ms to 25.5 seconds</name>
          <hints><slider tickSpacing='85' immediate='yes' showValue='yes'> </slider></hints>
        </int>
        <int size='1'>
          <name>Off-delay 0=No repeat, 1-255 = from 100msto 25.5 seconds, </name>
=======
          <name>On-delay 0=steady-state, 1-255 = from 100ms to 25.5seconds,</name>
          <hints><slider tickSpacing='85' immediate='yes' showValue='yes'> </slider></hints>
        </int>
        <int size='1'>
          <name>Off-delay 0=No repeat, 1-255 = From 100ms to 25.5 seconds, </name>
>>>>>>> 101e11cf0f733f584f6be34488b585f349e585cb
          <hints><slider tickSpacing='85' immediate='yes' showValue='yes'> </slider></hints>
        </int>
        <eventid><name>On-Event</name></eventid>
        <eventid><name>Off-Event</name></eventid>
      </group>
    )" CDIfooter;
}

// ===== MERGED MemStruct =====
typedef struct {
  EVENT_SPACE_HEADER eventSpaceHeader; // MUST BE AT THE TOP OF STRUCT
  char nodeName[20];                    
  char nodeDesc[24];                    

  // Outputs (40 actions total allocated across pins)
  struct {
    char desc[24];
    EventID eid;
    uint8_t pini;
    char action;
    uint8_t durn;
    uint8_t rate;
  } action[NUM_ACTION];

  // Inputs
  struct {
    char desc[24];
    uint8_t type;
    uint8_t duration;    // On-delay 
    uint8_t period;      // Off-delay
    EventID onEid;
    EventID offEid;
  } io[NUM_IO];

} MemStruct;  

extern "C" {
  // Global Event Macro mapping Helpers for Outputs (Consumers)
  #define aEID(i) CEID(action[i].eid), \
                  CEID(action[i + 1].eid), \
                  CEID(action[i + 2].eid), \
                  CEID(action[i + 3].eid), \
                  CEID(action[i + 4].eid)

  // Merged Tracking Configuration Table
  const EIDTab eidtab[NUM_EVENT] PROGMEM = {
    // 40 Consumer Actions for the Outputs
    aEID(0),  aEID(5),  aEID(10), aEID(15),
    aEID(20), aEID(25), aEID(30), aEID(35),
    
    // 16 Producer Events for the 8 Inputs (onEid and offEid pairs)
    PEID(io[0].onEid), PEID(io[0].offEid),
    PEID(io[1].onEid), PEID(io[1].offEid),
    PEID(io[2].onEid), PEID(io[2].offEid),
    PEID(io[3].onEid), PEID(io[3].offEid),
    PEID(io[4].onEid), PEID(io[4].offEid),
    PEID(io[5].onEid), PEID(io[5].offEid),
    PEID(io[6].onEid), PEID(io[6].offEid),
    PEID(io[7].onEid), PEID(io[7].offEid)
  };

  // Keep ONLY this single copy of the SNIP hardware identity data!
  const char SNII_const_data[] PROGMEM = "\001" MANU "\000" MODEL "\000" HWVERSION "\000" OlcbCommonVersion;
}



uint8_t protocolIdentValue[6] = {
  pSimple | pDatagram | pMemConfig | pPCEvents | !pIdent | pTeach | !pStream | !pReservation,
  pACDI | pSNIP | pCDI | !pRemote | !pDisplay | !pTraction | !pFunction | !pDCC,
  0, 0, 0, 0
};

// Variable states allocations
uint8_t iopin[] = { IOPINS };
bool iostate[NUM_IO] = {0};  
bool logstate[NUM_IO] = {0}; 
unsigned long next[NUM_IO] = {0};

bool initialized = false;

// Macro prints definitions for debugging
#define PV(x) { dP(" " #x "="); dP(x); }
#define PVL(x) { dP("\n" #x "="); dP(x); }

// Forward declarations
void setupIOPins();

// ===== Process Consumer-eventIDs =====
void pceCallback(uint16_t index) {
  // If the event index matches an Output configuration action space
  if(index < NUM_ACTION) {
    dP("\npceCallback-Output(");
    dP(index);
    uint8_t i = NODECONFIG.read(EEADDR(action[index].pini)) - 1;  // correct to 0-index
    if(i < NUM_CHANNEL) {
      currentEvent[i] = index;
      timer[i] = millis() + NODECONFIG.read(EEADDR(action[index].durn));
      state[i] = 0;
      PV(i); PV(currentEvent[i]);
    }
  }
}

// Global protocol verification reporting
enum evStates { VALID=4, INVALID=5, UNKNOWN=7 };
uint8_t userState(uint16_t index) {
    int ch = index / 2;
    if (NODECONFIG.read(EEADDR(io[ch].type)) == 0) {
        return UNKNOWN;
    }
    uint8_t eidstate = (index % 2) ? 1 : 0;
    if (eidstate == iostate[ch]) {
        return VALID;
    }
    return INVALID;
}

// Factory Clear / Init Configuration Reset Space Setup
void userInitAll() {
  NODECONFIG.put(EEADDR(nodeName), ESTRING("ESP32"));
  NODECONFIG.put(EEADDR(nodeDesc), ESTRING("8 In / 8 Out"));

  // Reset outputs
  for (int i = 0; i < NUM_ACTION; i++) NODECONFIG.write(EEADDR(action[i].pini), 0);
  for (int i = 0; i < NUM_CHANNEL; i++) {
    pinMode(pin[i], OUTPUT);
    state[i] = 0;
    timer[i] = 0;
    currentEvent[i] = 255;
  }

  // Reset inputs
  for(uint8_t i = 0; i < NUM_IO; i++) {
    NODECONFIG.put(EEADDR(io[i].desc), ESTRING(""));
    NODECONFIG.update(EEADDR(io[i].type), 0);
    NODECONFIG.update(EEADDR(io[i].duration), 0);
    NODECONFIG.update(EEADDR(io[i].period), 0);
  }  
  EEPROMcommit;
  initialized = true;
}

void userSoftReset() { REBOOT; }
void userHardReset() { REBOOT; }

void userConfigWritten(uint32_t address, uint16_t length, uint16_t func) {
  EEPROMcommit;
  setupIOPins();
}

NodeID nodeid(NODE_ADDRESS);  
#include "OpenLCBMid.h"       // System backend bindings injection

enum Action { aLOW = 1, aHIGH, aFLASH, aDSTROBE, aRANDOM, aFIRE };

// Outputs Processing Engine
void doActions() {
  static uint8_t i = 255;
  uint8_t pi, actn, rate, durn;
  if (!initialized) return;
  if (++i >= NUM_CHANNEL) i = 0;
  uint8_t ei = currentEvent[i];
  if (ei == 255) return;
  long now = millis();

  pi = NODECONFIG.read(EEADDR(action[ei].pini)) - 1;  // pin index
  actn = NODECONFIG.read(EEADDR(action[ei].action));
  durn = NODECONFIG.read(EEADDR(action[ei].durn));
  rate = NODECONFIG.read(EEADDR(action[ei].rate));

  switch (actn) {
    case aLOW:
      if (state[i]) return;
      digitalWrite(pin[pi], LOW);
      state[i] = 1;
      break;
    case aHIGH:
      if (state[i]) return;
      digitalWrite(pin[pi], HIGH);
      state[i] = 1;
      break;
    case aFLASH:
      if (timer[i] && (now - timer[i]) > 0) {
        switch (state[i]) {
          case 0:
            digitalWrite(pin[pi], HIGH);
            timer[i] = now + durn * 100;
            state[i] = 1;
            break;
          case 1:
            digitalWrite(pin[pi], LOW);
            timer[i] = now + rate * 100;
            state[i] = 0;
            break;
        }
      }
      break;
    case aDSTROBE:
      if (timer[i] && (now - timer[i]) > 0) {
        switch (state[i]) {
          case 0:
            digitalWrite(pin[pi], HIGH);
            timer[i] = now + 100;
            state[i] = 1;
            break;
          case 1:
            digitalWrite(pin[pi], LOW);
            timer[i] = now + durn * 100;
            state[i] = 2;
            break;
          case 2:
            digitalWrite(pin[pi], HIGH);
            timer[i] = now + 100;
            state[i] = 3;
            break;
          case 3:
            digitalWrite(pin[pi], LOW);
            timer[i] = now + rate * 100;
            state[i] = 0;
            break;
        }
      }
      break;
    case aRANDOM:
      if (timer[i] && (now - timer[i]) > 0) {
        switch (state[i]) {
          case 0:
            digitalWrite(pin[pi], HIGH);
            timer[i] = now + random(durn * durn * 5, durn * durn * 20);
            state[i] = 1;
            break;
          case 1:
            digitalWrite(pin[pi], LOW);
            timer[i] = now + random(rate * rate * 5, rate * rate * 20);
            state[i] = 0;
            break;
        }
      }
      break;
    case aFIRE:
      if (state[i] != 0) return;
      if (durn == 0) {
        analogWrite(pin[pi], 0);
        analogWrite(pin[pi + 1], 0);
        state[i] = 1;
      }
      if (timer[i] && (now - timer[i]) > 0) {
        if (pi + 1 > NUM_CHANNEL) break;
        analogWrite(pin[pi], random(durn));
        analogWrite(pin[pi + 1], random(rate));
        timer[i] = now + 15;
      }
      break;
  }
}

// Inputs Scans Engine
void produceFromInputs() {
  static uint8_t c = 0;
  static unsigned long last = 0;
  if((millis()-last)<(50/NUM_IO)) return;
  last = millis();
  uint8_t type = NODECONFIG.read(EEADDR(io[c].type));
  uint8_t d;
  
  if(type==5 || type==6) {
    bool s = digitalRead(iopin[c]);
    if(s != iostate[c]) {
      iostate[c] = s;
      if(!s) {
        logstate[c] ^= 1;
        if(logstate[c]) d = NODECONFIG.read(EEADDR(io[c].duration));
        else            d = NODECONFIG.read(EEADDR(io[c].period));
        if(d==0) OpenLcb.produce(logstate[c] ); 
        else next[c] = millis() + (uint16_t)d*100;          
      }
    }
  }
  if(type>0 && type<5) {
    bool s = digitalRead(iopin[c]);
    if(s != iostate[c]) {
      iostate[c] = s;
      if(!iostate[c]) d = NODECONFIG.read(EEADDR(io[c].duration)); 
      else d = NODECONFIG.read(EEADDR(io[c].period));
      if(d==0) OpenLcb.produce(!s^(type&1) ); 
      else {
        next[c] = millis() + (uint16_t)d*100;                  
      }
    }
  }
  if(++c>=NUM_IO) c = 0;
}

void processProducer() {
  // Handles delayed input state conversions if registered
}

// Configuration deployment of pins
void setupIOPins() {
  dP("\nPins Setup Configuration Check: ");
  for(uint8_t i=0; i<NUM_IO; i++) {
    uint8_t type = NODECONFIG.read( EEADDR(io[i].type));
    switch (type) {
      case 1: case 2: case 5:
        pinMode(iopin[i], INPUT); 
        iostate[i] = type&1;
        if(type==5) iostate[i] = 0;
        break;
      case 3: case 4: case 6:
        pinMode(iopin[i], INPUT_PULLUP); 
        iostate[i] = type&1;
        break;
    }
  }
}

// ==== SETUP ====
void setup() {
  Serial.begin(115200);
  delay(1000);
  dP("\nInitializing Combined IO Node Sketch...");

  EEPROMbegin;
  NodeID nodeid(NODE_ADDRESS);       
  Olcb_init(nodeid, RESET_TO_FACTORY_DEFAULTS);
 
  // Set output pins up
  for (int c = 0; c < NUM_CHANNEL; c++) {
    pinMode(pin[c], OUTPUT);
  }

  setupIOPins();
  initialized = true;
}

// ==== MAIN LOOP ====
void loop() {
  bool activity = Olcb_process(); // Main OpenLCB engine tasks
  
  doActions();          // Service outputs 
  produceFromInputs();  // Scan inputs
  processProducer();    // Dynamic delayed events handler
}
