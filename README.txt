OpenSprinkler ESP8266 Firmware 

Cloned from: https://github.com/OpenSprinkler/OpenSprinkler-Firmware

Changes:
- uses platformIO
- RFswitch function removed
- "server.c", "server.h" changed to OSserver.c/h because of some file conflicts
- uses MCP23S17 port expander
- latch valves removed