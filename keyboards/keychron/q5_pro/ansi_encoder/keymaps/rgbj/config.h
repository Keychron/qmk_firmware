#pragma once

/* One tap was sometimes coming out as two or three of the same character.
 * The switch contact bounces, and the old eager per-key debounce only
 * ignored those extra edges for 5ms. Bounce that lasted longer became
 * another press. The count changed from tap to tap because the bounce
 * is irregular.
 *
 * Asymmetric eager-defer still reports the press immediately, so a key
 * you keep holding stays held and key repeat still works. The release is
 * reported only after the contact has stayed open for this whole window,
 * so a bounce cannot become a second character. 10ms covers the chatter
 * we were seeing without making a normal tap feel held.
 *
 * The algorithm stores the time in 7 bits, so the window caps at 127ms. */
#define DEFAULT_DEBOUNCE_TYPE DEBOUNCE_ASYM_EAGER_DEFER_PER_KEY
#define DEBOUNCE 10
