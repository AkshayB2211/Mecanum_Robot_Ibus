#pragma once

// ═══════════════════════════════════════════════════════════════
//  test_motors.h — Standalone motor test sequences
// ═══════════════════════════════════════════════════════════════

// Runs through every motion mode sequentially.
// Call from loop() when CH_MODE switch is in TEST position.
void test_all_motions();

// Individual test steps (can be called separately)
void test_cardinal();     // forward, backward, strafe L/R
void test_diagonals();    // all four diagonal directions
void test_pivots();       // CW and CCW pivot in place
void test_arc_turns();    // arc right and arc left
