// ═══════════════════════════════════════════════════════════════
//  ibus.cpp — FlySky iBUS parser (no external library needed)
// ═══════════════════════════════════════════════════════════════
#include <Arduino.h>
#include "config.h"
#include "ibus.h"

// ── Internal state ───────────────────────────────────────────────
static uint8_t  _buf[IBUS_FRAME_LEN];
static uint8_t  _idx       = 0;
static uint16_t _ch[IBUS_NUM_CHANNELS] = {0};
static uint32_t _lastFrame = 0;
static bool     _synced    = false;
static bool     _hasFrame  = false;   // true only after first valid frame

// ── Checksum verify ──────────────────────────────────────────────
// iBUS checksum: 0xFFFF minus sum of first 30 bytes
static bool _checksum() {
  uint16_t sum = 0;
  for (uint8_t i = 0; i < IBUS_FRAME_LEN - 2; i++) sum += _buf[i];
  uint16_t check = (uint16_t)_buf[IBUS_FRAME_LEN - 1] << 8 | _buf[IBUS_FRAME_LEN - 2];
  return ((0xFFFF - sum) == check);
}

// ── Parse channels from validated buffer ─────────────────────────
static void _parse() {
  for (uint8_t c = 0; c < IBUS_NUM_CHANNELS; c++) {
    uint8_t base = 2 + c * 2;
    _ch[c] = (uint16_t)_buf[base] | ((uint16_t)_buf[base + 1] << 8);
  }
  _lastFrame = millis();
  _hasFrame  = true;   // latch permanently once we have real data
}

// ── Public API ───────────────────────────────────────────────────
void ibus_init() {
  IBUS_SERIAL.begin(115200);
  _idx      = 0;
  _synced   = false;
  _hasFrame = false;   // motors stay off until first real frame arrives
  _lastFrame = 0;
  // Seed all channels to safe neutral — these values are NEVER sent to
  // motors until _hasFrame is true, so the exact seed doesn't matter,
  // but centre / min-throttle is still a sane fallback.
  for (uint8_t i = 0; i < IBUS_NUM_CHANNELS; i++) _ch[i] = STICK_CENTER;
  _ch[CH_THROTTLE - 1] = STICK_MIN;
}

bool ibus_update() {
  bool gotFrame = false;

  while (IBUS_SERIAL.available()) {
    uint8_t b = IBUS_SERIAL.read();

    // Re-sync on header pair
    if (!_synced) {
      if (_idx == 0 && b == IBUS_HEADER1) { _buf[_idx++] = b; }
      else if (_idx == 1 && b == IBUS_HEADER2) { _buf[_idx++] = b; _synced = true; }
      else { _idx = 0; }   // reset on unexpected byte
      continue;
    }

    _buf[_idx++] = b;

    if (_idx == IBUS_FRAME_LEN) {
      // Full frame received
      _idx    = 0;
      _synced = false;
      if (_checksum()) {
        _parse();
        gotFrame = true;
      }
    }
  }

  return gotFrame;
}

uint16_t ibus_get(uint8_t channel) {
  if (channel < 1 || channel > IBUS_NUM_CHANNELS) return 0;
  return _ch[channel - 1];
}

bool ibus_lost(uint32_t timeout_ms) {
  // No valid frame ever received → always report lost
  if (!_hasFrame) return true;
  return (millis() - _lastFrame) > timeout_ms;
}

bool ibus_ready() {
  return _hasFrame;
}
