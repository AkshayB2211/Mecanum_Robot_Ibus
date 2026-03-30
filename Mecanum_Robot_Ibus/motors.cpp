// ═══════════════════════════════════════════════════════════════
//  motors.cpp — Wheel driver implementation
// ═══════════════════════════════════════════════════════════════
#include <Arduino.h>
#include "config.h"
#include "motors.h"

// ── Internal helper ─────────────────────────────────────────────
static void _setMotor(uint8_t pinA, uint8_t pinPWM, uint8_t pinB, int spd) {
  spd = constrain(spd, -255, 255);
  if (spd > 0) {
    digitalWrite(pinA, LOW);
    analogWrite(pinPWM, spd);
    digitalWrite(pinB, HIGH);
  } else if (spd < 0) {
    digitalWrite(pinA, HIGH);
    analogWrite(pinPWM, -spd);
    digitalWrite(pinB, LOW);
  } else {
    analogWrite(pinPWM, 0);      // coast (both LOW keeps brake off)
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  }
}

// ── Init ────────────────────────────────────────────────────────
void motors_init() {
  uint8_t pins[] = {
    FRONT_LEFT_A,  FRONT_LEFT_PWM,  FRONT_LEFT_B,
    FRONT_RIGHT_A, FRONT_RIGHT_PWM, FRONT_RIGHT_B,
    REAR_LEFT_A,   REAR_LEFT_PWM,   REAR_LEFT_B,
    REAR_RIGHT_A,  REAR_RIGHT_PWM,  REAR_RIGHT_B
  };
  for (uint8_t p : pins) pinMode(p, OUTPUT);
  stopAll();
}

// ── Per-wheel setters ────────────────────────────────────────────
void setFrontLeft (int spd) { _setMotor(FRONT_LEFT_A,  FRONT_LEFT_PWM,  FRONT_LEFT_B,  spd); }
void setFrontRight(int spd) { _setMotor(FRONT_RIGHT_A, FRONT_RIGHT_PWM, FRONT_RIGHT_B, spd); }
void setRearLeft  (int spd) { _setMotor(REAR_LEFT_A,   REAR_LEFT_PWM,   REAR_LEFT_B,   spd); }
void setRearRight (int spd) { _setMotor(REAR_RIGHT_A,  REAR_RIGHT_PWM,  REAR_RIGHT_B,  spd); }

// ── Composite ────────────────────────────────────────────────────
void drive(int fl, int fr, int rl, int rr) {
  setFrontLeft (fl);
  setFrontRight(fr);
  setRearLeft  (rl);
  setRearRight (rr);
}

// ── Named motions ─────────────────────────────────────────────────

void moveForward (int spd) { drive( spd,  spd,  spd,  spd); }
void moveBackward(int spd) { drive(-spd, -spd, -spd, -spd); }

// Lateral strafe: diagonal roller vectors cancel longitudinal force
void strafeLeft (int spd) { drive(-spd,  spd,  spd, -spd); }
void strafeRight(int spd) { drive( spd, -spd, -spd,  spd); }

// Diagonal: only one pair of diagonal wheels active
void diagForwardRight (int spd) { drive( spd,    0,    0,  spd); }
void diagForwardLeft  (int spd) { drive(   0,  spd,  spd,    0); }
void diagBackwardRight(int spd) { drive(   0, -spd, -spd,    0); }
void diagBackwardLeft (int spd) { drive(-spd,    0,    0, -spd); }

// Pivot in place: left vs right oppose
void pivotClockwise       (int spd) { drive( spd, -spd,  spd, -spd); }
void pivotCounterClockwise(int spd) { drive(-spd,  spd, -spd,  spd); }

// Arc turns: one side faster
void turnRight(int fastSpd, int slowSpd) { drive(fastSpd, slowSpd, fastSpd, slowSpd); }
void turnLeft (int fastSpd, int slowSpd) { drive(slowSpd, fastSpd, slowSpd, fastSpd); }

void stopAll() { drive(0, 0, 0, 0); }
