#pragma once

// ═══════════════════════════════════════════════════════════════
//  ibus.h — FlySky iBUS receiver interface
//
//  iBUS frame format (32 bytes, ~7 ms cadence @ 115200 baud):
//    [0x20][0x40][CH1_L][CH1_H]...[CH14_L][CH14_H][CHK_L][CHK_H]
//
//  Channel values: 1000–2000 µs (1500 = centre)
// ═══════════════════════════════════════════════════════════════

#include <stdint.h>

#define IBUS_NUM_CHANNELS  14
#define IBUS_FRAME_LEN     32
#define IBUS_HEADER1       0x20
#define IBUS_HEADER2       0x40

// Call once in setup()
void ibus_init();

// Call every loop() — reads bytes from serial, parses complete frames.
// Returns true when a fresh frame has been decoded.
bool ibus_update();

// Return raw µs value for channel (1-indexed, 1–14).
// Returns 0 if channel is out of range or no frame received yet.
uint16_t ibus_get(uint8_t channel);

// Returns true if no valid frame has been received within timeout_ms.
bool ibus_lost(uint32_t timeout_ms = 500);

// Returns true once at least one valid frame has been received.
// Use this to hard-gate any output until the receiver is confirmed live.
bool ibus_ready();
