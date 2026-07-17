#include "Config.h"         // Contains configuration, see "Config.h"
#include "ACAN_ESP32Can.h"
#include "HouseKeeping.h"
#include "mdebugging.h"
#include "processor.h"
#include "OpenLcbCore.h"
#include "OpenLCBHeader.h"

#define OLCB_NO_BLUE_GOLD

extern "C" {        
#define N(x) xN(x)
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
            <name>Parameter 1 On-delay 0=steady-state, 1-255 = from 100ms to 25.5 seconds</name>
            <hints><slider tickSpacing='85' immediate='yes' showValue='yes'> </slider></hints>
          </int>

          <int size="1">
            <name>Parameter 2 Off-delay 0=No repeat, 1-255 = from 100ms to 25.5 seconds</name>
            <hints><slider tickSpacing='85' immediate='yes' showValue='yes'> </slider></hints>
          </int>
        </group>
      </group>

      <group replication=")" N(NUM_IO) R"(">
        <name>Inputs - choose the pin that you wish to use. Note these are on the lefthand side of the sensor shield</name>
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
          <name>On-delay 0=steady-state, 1-255 = from 100ms to 25.5 seconds</name>
          <hints><slider tickSpacing='85' immediate='yes' showValue='yes'> </slider></hints>
        </int>
        <int size='1'>
          <name>Off-delay 0=No repeat, 1-255 = from 100ms to 25.5 seconds</name>
          <hints><slider tickSpacing='85' immediate='yes' showValue='yes'> </slider></hints>
        </int>
        <eventid><name>On-Event</name></eventid>
        <eventid><name>Off-Event</name></eventid>
      </group>
    )" CDIfooter;
}

// Global state enums
enum evStates { VALID=4, INVALID=5, UNKNOWN=7 };

// ===== MERGED MemStruct =====
typedef struct {
  EVENT_SPACE_HEADER eventSpaceHeader;
  char nodeName[20];                    
  char nodeDesc[24];                    

  struct {
    char desc[24];
    EventID eid;
    uint8_t pini;
    char action;
    uint8_t durn;
    uint8_t rate;
  } action[NUM_ACTION];

  struct {
    char desc[24];
    uint8_t type;
    uint8_t duration;
    uint8_t period;
    EventID onEid;
    EventID offEid;
  } io[NUM_IO];

} MemStruct;  

extern "C" {
  #define aEID(i) CEID(action[i].eid), CEID(action[i + 1].eid), CEID(action[i + 2].eid), CEID(action[i + 3].eid), CEID(action[i + 4].eid)

  const EIDTab eidtab[NUM_EVENT] PROGMEM = {
    aEID(0), aEID(5), aEID(10), aEID(15),
    aEID(20), aEID(25), aEID(30), aEID(35),
    
    PEID(io[0].onEid), PEID(io[0].offEid),
    PEID(io[1].onEid), PEID(io[1].offEid),
    PEID(io[2].onEid), PEID(io[2].offEid),
    PEID(io[3].onEid), PEID(io[3].offEid),
    PEID(io[4].onEid), PEID(io[4].offEid),
    PEID(io[5].onEid), PEID(io[5].offEid),
    PEID(io[6].onEid), PEID(io[6].offEid),
    PEID(io[7].onEid), PEID(io[7].offEid)
  };

  const char SNII_const_data[] PROGMEM = "\001" MANU "\000" MODEL "\000" HWVERSION "\000" SWVERSION " " OlcbCommonVersion;
}

uint8_t protocolIdentValue[6] = {
  pSimple | pDatagram | pMemConfig | pPCEvents | !pIdent | pTeach | !pStream | !pReservation,
  pACDI | pSNIP | pCDI | !pRemote | !pDisplay | !pTraction | !pFunction | !pDCC,
  0, 0, 0, 0
};

// Variables
uint8_t iopin[] = { IOPINS };
bool iostate[NUM_IO] = {0};  
bool logstate[NUM_IO] = {0}; 
unsigned long next[NUM_IO] = {0};

bool initialized = false;

void setupIOPins();
void doActions();
void produceFromInputs();
void processProducer();

// ===== Process Consumer-eventIDs (Outputs) =====
void pceCallback(uint16_t index) {
  if(index < NUM_ACTION) {
    dP("\npceCallback-Output("); dP(index);
    uint8_t i = NODECONFIG.read(EEADDR(action[index].pini)) - 1;
    if(i < NUM_CHANNEL) {
      currentEvent[i] = index;
      timer[i] = millis() + NODECONFIG.read(EEADDR(action[index].durn));
      state[i] = 0;
    }
  }
}

uint8_t userState(uint16_t index) {
  int ch = index / 2;
  if (NODECONFIG.read(EEADDR(io[ch].type)) == 0) {
    return UNKNOWN;
  }
  uint8_t eidstate = (index % 2) ? 1 : 0;
  if (eidstate == iostate[ch]) return VALID;
  return INVALID;
}

// Factory Reset
void userInitAll() {
  NODECONFIG.put(EEADDR(nodeName), ESTRING("ESP32"));
  NODECONFIG.put(EEADDR(nodeDesc), ESTRING("8 In / 8 Out"));

  for (int i = 0; i < NUM_ACTION; i++) NODECONFIG.write(EEADDR(action[i].pini), 0);
  for (int i = 0; i < NUM_CHANNEL; i++) {
    pinMode(pin[i], OUTPUT);
    state[i] = 0;
    timer[i] = 0;
    currentEvent[i] = 255;
  }

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
#include "OpenLCBMid.h"

// Action enum
enum Action { aLOW = 1, aHIGH, aFLASH, aDSTROBE, aRANDOM, aFIRE };

// Outputs
void doActions() {
  static uint8_t i = 255;
  if (!initialized) return;
  if (++i >= NUM_CHANNEL) i = 0;
  uint8_t ei = currentEvent[i];
  if (ei == 255) return;

  long now = millis();
  uint8_t pi = NODECONFIG.read(EEADDR(action[ei].pini)) - 1;
  uint8_t actn = NODECONFIG.read(EEADDR(action[ei].action));
  uint8_t durn = NODECONFIG.read(EEADDR(action[ei].durn));
  uint8_t rate = NODECONFIG.read(EEADDR(action[ei].rate));

  switch (actn) {
    case aLOW:  if (state[i]) return; digitalWrite(pin[pi], LOW);  state[i] = 1; break;
    case aHIGH: if (state[i]) return; digitalWrite(pin[pi], HIGH); state[i] = 1; break;
    case aFLASH: /* your original FLASH code */ break;
    case aDSTROBE: /* your original DSTROBE code */ break;
    case aRANDOM: /* your original RANDOM code */ break;
    case aFIRE: /* your original FIRE code */ break;
  }
}

// Fixed Input Handling
void produceFromInputs() {
  static uint8_t c = 0;
  static unsigned long last = 0;
  
  if ((millis() - last) < 20) return;
  last = millis();

  uint8_t type = NODECONFIG.read(EEADDR(io[c].type));
  if (type == 0) { if(++c >= NUM_IO) c=0; return; }

  bool s = digitalRead(iopin[c]);
  uint8_t d;

  if (type == 5 || type == 6) {
    if (s != iostate[c]) {
      iostate[c] = s;
      if (!s) {
        logstate[c] ^= 1;
        d = logstate[c] ? NODECONFIG.read(EEADDR(io[c].duration)) : NODECONFIG.read(EEADDR(io[c].period));
        uint16_t idx = NUM_ACTION + c*2 + logstate[c];
        if (d == 0) OpenLcb.produce(idx);
        else next[c] = millis() + (uint16_t)d * 100;
      }
    }
  } 
  else if (type > 0 && type < 5) {
    if (s != iostate[c]) {
      iostate[c] = s;
      d = (!iostate[c]) ? NODECONFIG.read(EEADDR(io[c].duration)) : NODECONFIG.read(EEADDR(io[c].period));
      uint16_t idx = NUM_ACTION + c*2 + (!s ^ (type & 1));
      if (d == 0) OpenLcb.produce(idx);
      else next[c] = millis() + (uint16_t)d * 100;
    }
  }

  if (++c >= NUM_IO) c = 0;
}

void processProducer() {
  static unsigned long last = 0;
  unsigned long now = millis();
  if ((now - last) < 50) return;
  last = now;

  for (uint8_t c = 0; c < NUM_IO; c++) {
    if (next[c] == 0 || now < next[c]) continue;

    uint8_t type = NODECONFIG.read(EEADDR(io[c].type));
    uint16_t idx = NUM_ACTION + c*2;
    if (type == 5 || type == 6) idx += logstate[c];
    else idx += (!iostate[c] ^ (type & 1));

    OpenLcb.produce(idx);
    next[c] = 0;
  }
}

void setupIOPins() {
  dP("\nPins Setup: ");
  for(uint8_t i = 0; i < NUM_IO; i++) {
    uint8_t type = NODECONFIG.read(EEADDR(io[i].type));
    if (type >= 1 && type <= 2 || type == 5) pinMode(iopin[i], INPUT);
    else if (type >= 3 && type <= 4 || type == 6) pinMode(iopin[i], INPUT_PULLUP);

    iostate[i] = digitalRead(iopin[i]);
    logstate[i] = iostate[i];
    next[i] = 0;
  }
}

// ==== SETUP ====
void setup() {
  Serial.begin(115200);
  delay(1000);
  dP("\n=== Combined IO Node Starting ===");

  EEPROMbegin;
  Olcb_init(nodeid, RESET_TO_FACTORY_DEFAULTS);
 
  for (int c = 0; c < NUM_CHANNEL; c++) pinMode(pin[c], OUTPUT);

  setupIOPins();
  initialized = true;
}

// ==== LOOP ====
void loop() {
  Olcb_process();
  doActions();
  produceFromInputs();
  processProducer();
}
