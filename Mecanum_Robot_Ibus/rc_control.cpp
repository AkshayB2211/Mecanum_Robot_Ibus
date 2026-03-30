// ═══════════════════════════════════════════════════════════════
//  rc_control.cpp — Maps iBUS stick values to mecanum drive
//
//  Trigonometric polar mixing:
//    The right stick (throttle + strafe) is treated as a 2-D
//    vector.  Its magnitude and angle are computed, then each
//    wheel's contribution is derived by projecting that vector
//    onto the wheel's effective drive axis (±45°).
//
//    This means pushing forward + right at full deflection gives
//    a true diagonal at full speed, not a clipped / uneven output.
//
//  Rotation (left stick H) is added linearly on top of the
//  translation result, then the whole output is normalised so
//  no wheel exceeds MAX_SPEED while preserving direction ratios.
//
//  Wheel layout (viewed from above, front at top):
//
//          FL ╲       ╱ FR
//              ╲     ╱
//              ╱     ╲
//          RL ╱       ╲ RR
//
//  Mecanum roller axes are at 45°, so the effective drive
//  projections for a movement vector (Vx=strafe, Vy=fwd) are:
//    FL =  Vy - Vx   (or: magnitude × sin(θ + 45°) × √2)
//    FR =  Vy + Vx
//    RL =  Vy + Vx
//    RR =  Vy - Vx
//  ...which is mathematically equivalent to the polar form below.
// ═══════════════════════════════════════════════════════════════
#include <Arduino.h>
#include <math.h>
#include "config.h"
#include "ibus.h"
#include "motors.h"
#include "rc_control.h"

// ── Stick → normalised float [-1.0, +1.0] ────────────────────────
// Returns 0.0 inside the dead-band, scales linearly to ±1.0 at
// full deflection.  Using floats here keeps the trig clean;
// conversion back to integer PWM happens at the very end.
static float stickToFloat(uint16_t raw) {
  int val = (int)raw - STICK_CENTER;
  if (abs(val) < DEAD_BAND) return 0.0f;

  // Shift dead-band edge to zero for a continuous output
  val = (val > 0) ? val - DEAD_BAND : val + DEAD_BAND;

  float range = (float)((STICK_MAX - STICK_CENTER) - DEAD_BAND);
  return constrain((float)val / range, -1.0f, 1.0f);
}

// ── RC update ────────────────────────────────────────────────────
void rc_update() {
  // Hard gate: no output until at least one valid frame has arrived.
  if (!ibus_ready() || ibus_lost() || ibus_get(CH_MODE) > 1900) {
    stopAll();
    return;
  }

  // CH potentiometer on transmitter is faulty, so take only extremes
  long ibus_rotate_map = ibus_get(CH_ROTATE);
  // long ibus_rotate_map = map(ibus_get(CH_ROTATE), 1300, 1700, 1000, 2000);
  // constrain(ibus_rotate_map, 1000, 2000);
  if (ibus_rotate_map > 1650) {
    ibus_rotate_map = 2000;
  } else if (ibus_rotate_map < 1350) {
    ibus_rotate_map = 1000;
  } else {
    ibus_rotate_map = 1500;
  }
  
  // DEBUG IBUS VALUES
  Serial.print(ibus_get(CH_THROTTLE));
  Serial.print(",");
  Serial.print(ibus_get(CH_STRAFE));
  Serial.print(",");
  Serial.print(ibus_rotate_map);
  Serial.println();

  // ── 1. Read axes as normalised floats ───────────────────────────
  float Vy =  stickToFloat(ibus_get(CH_THROTTLE));  // +1 = full fwd
  float Vx =  stickToFloat(ibus_get(CH_STRAFE));    // +1 = full right
  float rot =  stickToFloat(ibus_rotate_map);    // +1 = full CW

  // ── 2. Polar decomposition of the translation vector ────────────
  //  θ   = direction the robot should move (radians, 0 = forward)
  //  mag = how fast (0.0 – 1.0)
  float theta = atan2f(Vx, Vy);   // atan2(strafe, throttle)
  float mag   = sqrtf(Vx * Vx + Vy * Vy);
  mag = constrain(mag, 0.0f, 1.0f);  // clamp to unit circle

  // ── 3. Project onto each wheel's 45° roller axis ────────────────
  //  For a mecanum robot the optimal per-wheel speed for a pure
  //  translation at angle θ with magnitude mag is:
  //    FL / RR  =  mag × sin(θ + π/4)
  //    FR / RL  =  mag × cos(θ + π/4)   (= sin(θ + π/4 + π/2))
  //
  //  This is equivalent to the algebraic form (Vy±Vx) but correctly
  //  limits the combined vector to the unit circle so diagonal speed
  //  equals straight-line speed at the same stick deflection.
  float sinVal = sinf(theta + M_PI / 4.0f);
  float cosVal = cosf(theta + M_PI / 4.0f);

  // Scale by magnitude so partial deflection gives partial speed
  float fl_f =  mag * sinVal;
  float fr_f =  mag * cosVal;
  float rl_f =  mag * cosVal;
  float rr_f =  mag * sinVal;

  // ── 4. Add rotation component ────────────────────────────────────
  //  Rotation is independent of translation; add it directly.
  //  Sign: +rot spins CW → left wheels fwd, right wheels back.
  fl_f += rot;
  fr_f -= rot;
  rl_f += rot;
  rr_f -= rot;

  // ── 5. Normalise so no wheel exceeds ±1.0 ───────────────────────
  //  Find the largest absolute value across all wheels.
  //  If it exceeds 1.0, scale everything down by that factor so the
  //  direction ratios are preserved at full stick + full rotation.
  float maxVal = max(max(fabsf(fl_f), fabsf(fr_f)),
                     max(fabsf(rl_f), fabsf(rr_f)));
  if (maxVal > 1.0f) {
    fl_f /= maxVal;
    fr_f /= maxVal;
    rl_f /= maxVal;
    rr_f /= maxVal;
  }

  // ── 6. Convert to integer PWM and drive ─────────────────────────
  drive((int)(fl_f * MAX_SPEED),
        (int)(fr_f * MAX_SPEED),
        (int)(rl_f * MAX_SPEED),
        (int)(rr_f * MAX_SPEED));
}