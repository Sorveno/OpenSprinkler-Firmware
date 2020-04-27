
#include <Wire.h>
#include "../lib/MCP23017/MCP23017.h"

struct tcp_pcb;
extern struct tcp_pcb* tcp_tw_pcbs;
extern "C" void tcp_abort (struct tcp_pcb* pcb);
void tcpCleanup() { // losing bytes work around
	while(tcp_tw_pcbs) { tcp_abort(tcp_tw_pcbs); }
}

#include "OpenSprinkler.h"

extern OpenSprinkler os;

void do_setup();
void do_loop();

void setup() {
  do_setup();
}

void loop() {
  do_loop();
  tcpCleanup();
}
