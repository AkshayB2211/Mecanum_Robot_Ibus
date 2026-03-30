#pragma once

// ═══════════════════════════════════════════════════════════════
//  motors.h — Low-level wheel drivers and mecanum motion API
// ═══════════════════════════════════════════════════════════════

void motors_init();

// ── Per-wheel primitives (spd: +255 fwd … 0 coast … -255 rev) ──
void setFrontLeft (int spd);
void setFrontRight(int spd);
void setRearLeft  (int spd);
void setRearRight (int spd);

// ── Composite drive (fl, fr, rl, rr) ───────────────────────────
void drive(int fl, int fr, int rl, int rr);

// ── Named motion functions ──────────────────────────────────────
void moveForward (int spd = SPEED_FAST);
void moveBackward(int spd = SPEED_FAST);

void strafeLeft (int spd = SPEED_FAST);
void strafeRight(int spd = SPEED_FAST);

void diagForwardRight (int spd = SPEED_FAST);
void diagForwardLeft  (int spd = SPEED_FAST);
void diagBackwardRight(int spd = SPEED_FAST);
void diagBackwardLeft (int spd = SPEED_FAST);

void pivotClockwise       (int spd = SPEED_FAST);
void pivotCounterClockwise(int spd = SPEED_FAST);

void turnRight(int fastSpd = SPEED_FAST, int slowSpd = SPEED_SLOW);
void turnLeft (int fastSpd = SPEED_FAST, int slowSpd = SPEED_SLOW);

void stopAll();
