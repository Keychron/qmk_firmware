#pragma once

/* One tap was sometimes coming out as two or three of the same character.
 * Contact bounce is irregular, so the count changed from tap to tap.
 *
 * Asymmetric eager-defer helped the release edge, but once a release was
 * accepted there was no lockout. A late bounce closed again and became a
 * second press. Symmetric eager per-key reports the press and the release
 * immediately, then ignores every edge on that key for this whole window,
 * so that late bounce cannot type again. 50ms matches what settles this
 * board without blocking normal typing speed.
 *
 * Holding a key still works: the press is already accepted, and OS key
 * repeat does not need the bounce window. */
#define DEFAULT_DEBOUNCE_TYPE DEBOUNCE_SYM_EAGER_PER_KEY
#define DEBOUNCE 50

/* After this long with no host colour frames, leave host mode and restore
 * the onboard effect from EEPROM. Long enough that a slow render tick does
 * not bounce back to hardware mid-effect. */
#define HOST_LIGHTING_TIMEOUT_MS 3000
