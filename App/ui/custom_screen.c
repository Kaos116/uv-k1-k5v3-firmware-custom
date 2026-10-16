/* Copyright 2023 Dual Tachyon
 * https://github.com/DualTachyon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */

#include <stdio.h>
#include <string.h>

#include "ui/custom_screen.h"
#include "ui/helper.h"
#include "ui/inputbox.h"
#include "ui/ui.h"
#include "driver/st7565.h"
#include "settings.h"
#include "functions.h"
#include "misc.h"

void UI_DisplayCustomScreen(void)
{
    char string[16];

    // 1. Clear the screen buffer before drawing
    UI_DisplayClear();

    // ==========================================
    // LINE 1: Top Text Header (14-16 chars)
    // ==========================================
    UI_PrintStringSmallBold("UV-K1 CUSTOM", 0, 127, 0);

    // ==========================================
    // LINE 2: Large Frequency Font in the Middle
    // ==========================================
    uint32_t frequency = gRxVfo->Frequency;
    sprintf(string, "%lu.%05lu", frequency / 100000, frequency % 100000);
    UI_PrintStringBig(string, 2, 16, 2);

    // ==========================================
    // LINE 3: Bottom Status Bar (Split into 4 columns)
    // ==========================================
    // Column 1: Channel / VFO mode indicator
    if (IS_MR_CHANNEL(gRxVfo->CHANNEL_SAVE)) {
        sprintf(string, "CH-%03u", gRxVfo->CHANNEL_SAVE + 1);
    } else {
        sprintf(string, "VFO-%u", gEeprom.RX_VFO);
    }
    UI_PrintStringSmall(string, 0, 31, 5);

    // Column 2: Power Level (LOW / MID / HIGH)
    switch (gTxVfo->OUTPUT_POWER) {
        case OUTPUT_POWER_LOW:
            strcpy(string, "LOW");
            break;
        case OUTPUT_POWER_MID:
            strcpy(string, "MID");
            break;
        case OUTPUT_POWER_HIGH:
        default:
            strcpy(string, "HIGH");
            break;
    }
    UI_PrintStringSmall(string, 32, 63, 5);

    // Column 3: CTCSS / Tone info placeholder
    strcpy(string, "TONE");
    UI_PrintStringSmall(string, 64, 95, 5);

    // Column 4: Battery Indicator icon/text
    sprintf(string, "%u%%", gBatteryPercent);
    UI_PrintStringSmall(string, 96, 127, 5);

    // Render the final buffer to the LCD screen
    ST7565_BlitFullScreen();
}