#pragma once

// ═══════════════════════════════════════════════════════════════
//  config.h — Pin map, speed presets, and iBUS channel mapping
// ═══════════════════════════════════════════════════════════════

// ── Motor pin definitions ────────────────────────────────────────
// Rear Right Motor
#define REAR_RIGHT_A   2
#define REAR_RIGHT_PWM 3
#define REAR_RIGHT_B   4

// Rear Left Motor
#define REAR_LEFT_A   5
#define REAR_LEFT_PWM 6
#define REAR_LEFT_B   7

// Front Right Motor
#define FRONT_RIGHT_A   8
#define FRONT_RIGHT_PWM 9
#define FRONT_RIGHT_B   10

// Front Left Motor
#define FRONT_LEFT_A   12
#define FRONT_LEFT_PWM 11
#define FRONT_LEFT_B   13

// ── iBUS serial port ─────────────────────────────────────────────
// On Arduino Mega use Serial1/2/3 (pins 19/18, 17/16, 15/14)
// On Arduino Uno you must use SoftwareSerial on a free pin
#define IBUS_SERIAL     Serial1   // change to Serial2/3 if needed

// ── iBUS channel assignments (1-indexed as per FlySky convention) ─
#define CH_THROTTLE  3   // Right stick vertical   → forward / backward
#define CH_STRAFE    4   // Right stick horizontal  → strafe left / right
#define CH_ROTATE    1   // Left  stick horizontal  → pivot CW / CCW
#define CH_MODE      5   // SWA (2-pos switch)      → 1000=RC, 2000=TEST

// ── Stick dead-band & scaling ────────────────────────────────────
#define STICK_CENTER  1500   // µs midpoint
#define DEAD_BAND      100   // ±100 µs ignored around centre
#define STICK_MIN     1000   // µs full reverse / left
#define STICK_MAX     2000   // µs full forward / right
#define MAX_SPEED      240   // PWM ceiling (0–255)

// ── Speed presets (used by test routines) ────────────────────────
#define SPEED_SLOW  100
#define SPEED_FAST  200
