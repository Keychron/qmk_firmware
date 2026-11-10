/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* config.h is force-included, so this is only undefined when the keyboard's
 * config.h forgot to include eeconfig_kb.h. In that case no datablock is
 * reserved and the offsets below land on VIA's EEPROM data. */
#ifndef EECONFIG_KB_DATA_SIZE
#    error "Keychron common code requires #include \"eeconfig_kb.h\" in the keyboard's config.h"
#endif

#include <string.h>
#include "eeconfig_kb.h"
#include "raw_hid.h"
#include "eeconfig.h"
#include "eeprom.h"
#include "matrix.h"
#include "quantum.h"
#include "keychron_raw_hid.h"
#include "nvm_eeprom_eeconfig_internal.h"

_Static_assert(EECONFIG_BASE_LANGUAGE == EECONFIG_BASE_SIZE, "EECONFIG_BASE_LANGUAGE must match the start of the keyboard datablock");

static uint8_t lang;

static bool language_get(uint8_t *data) {
    eeprom_read_block(&lang, (uint8_t *)(EECONFIG_BASE_LANGUAGE), sizeof(lang));
    data[1] = lang;

    return true;
}

static bool language_set(uint8_t *data) {
    lang = data[0];
    eeprom_update_block(&lang, (uint8_t *)(EECONFIG_BASE_LANGUAGE), sizeof(lang));

    return true;
}

void language_rx(uint8_t *data, uint8_t length) {
    uint8_t cmd     = data[1];
    bool    success = true;

    switch (cmd) {
        case LANGUAGE_GET:
            success = language_get(&data[2]);
            break;

        case LANGUAGE_SET:
            success = language_set(&data[2]);
            break;

        default:
            data[0] = 0xFF;
            break;
    }

    data[2] = success ? 0 : 1;
}
