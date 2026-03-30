// ═══════════════════════════════════════════════════════════════
//  test_motors.cpp — Automated motion test sequences
// ═══════════════════════════════════════════════════════════════
#include <Arduino.h>
#include "config.h"
#include "motors.h"
#include "test_motors.h"

#define MOVE_MS  1000   // ms per motion step
#define PAUSE_MS  400   // ms pause between steps

static void _step(const char* label, void (*fn)(int), int spd = SPEED_FAST) {
  Serial.print(F("[TEST] ")); Serial.println(label);
  fn(spd);
  delay(MOVE_MS);
  stopAll();
  delay(PAUSE_MS);
}

// Wrappers to match void(*)(int) signature for pivot / turn variants
static void _pivotCW (int s) { pivotClockwise(s); }
static void _pivotCCW(int s) { pivotCounterClockwise(s); }
static void _strafeL (int s) { strafeLeft(s); }
static void _strafeR (int s) { strafeRight(s); }
static void _diagFR  (int s) { diagForwardRight(s); }
static void _diagFL  (int s) { diagForwardLeft(s); }
static void _diagBR  (int s) { diagBackwardRight(s); }
static void _diagBL  (int s) { diagBackwardLeft(s); }

void test_cardinal() {
  _step("Forward",   moveForward);
  _step("Backward",  moveBackward);
  _step("StrafeL",   _strafeL);
  _step("StrafeR",   _strafeR);
}

void test_diagonals() {
  _step("Diag Fwd-Right",  _diagFR);
  _step("Diag Fwd-Left",   _diagFL);
  _step("Diag Bwd-Right",  _diagBR);
  _step("Diag Bwd-Left",   _diagBL);
}

void test_pivots() {
  _step("Pivot CW",  _pivotCW);
  _step("Pivot CCW", _pivotCCW);
}

void test_arc_turns() {
  Serial.println(F("[TEST] Arc turn right"));
  turnRight(SPEED_FAST, SPEED_SLOW);
  delay(MOVE_MS);
  stopAll(); delay(PAUSE_MS);

  Serial.println(F("[TEST] Arc turn left"));
  turnLeft(SPEED_FAST, SPEED_SLOW);
  delay(MOVE_MS);
  stopAll(); delay(PAUSE_MS);
}

void test_all_motions() {
  Serial.println(F("\n=== Motor test START ==="));
  test_cardinal();
  test_diagonals();
  test_pivots();
  test_arc_turns();
  Serial.println(F("=== Motor test DONE  ===\n"));
  delay(3000);   // long pause before repeating
}
