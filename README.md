# Mecanum_Robot_Ibus
Code for 4 wheel mecanum robot using arduino with ibus communication with FS i6 transmitter receiver

# Trigonometric polar mixing:
The right stick (throttle + strafe) is treated as a 2-D vector. Its magnitude and angle are computed, then each wheel's contribution is derived by projecting that vector onto the wheel's effective drive axis (±45°).

This means pushing forward + right at full deflection gives a true diagonal at full speed, not a clipped / uneven output.

Rotation (left stick H) is added linearly on top of the translation result, then the whole output is normalised so no wheel exceeds MAX_SPEED while preserving direction ratios.

# Wheel layout (viewed from above, front at top):

          FL ╲       ╱ FR
              ╲     ╱
              ╱     ╲
          RL ╱       ╲ RR
          
