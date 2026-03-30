// ═══════════════════════════════════════════════════════════════
//  mecanum_robot.ino — Main entry point
//
//  Mode selection via CH_MODE (channel 5 by default):
//    ~1000 µs  →  RC mode   : iBUS sticks control the robot
//    ~2000 µs  →  Test mode : automated motion sequence runs
//
//  Wiring (Arduino Mega):
//    iBUS receiver signal wire → TX1 pin 18 (Serial1 RX)
//    No extra library needed — ibus.cpp parses the protocol.
//
//  If using Arduino Uno:
//    Replace IBUS_SERIAL in config.h with a SoftwareSerial instance
//    and update ibus_init() accordingly.
// ═══════════════════════════════════════════════════════════════
#include <Arduino.h>
#include "config.h"
#include "motors.h"
#include "ibus.h"
#include "rc_control.h"
#include "test_motors.h"

// ── Mode detection ───────────────────────────────────────────────
static bool isTestMode() {
  return ibus_get(CH_MODE) > 1700;   // switch HIGH position
}

// ── Setup ────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println(F("Mecanum Robot — booting"));

  motors_init();
  ibus_init();

  Serial.println(F("Ready. Waiting for iBUS signal..."));
}

// ── Main loop ────────────────────────────────────────────────────
void loop() {
  ibus_update();   // parse any new bytes from the receiver

  // Block all motion until at least one valid iBUS frame is received.
  // This eliminates the brief motor spin on power-up with no receiver.
  if (!ibus_ready()) {
    stopAll();
    return;
  }

  // if (isTestMode()) {
    // test_all_motions();   // runs one full sequence then pauses
  // } else {
    rc_update();          // translate sticks → motor outputs
  // }
}
