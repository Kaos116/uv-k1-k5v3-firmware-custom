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
    // LINE 2: Frequency in the Middle
    // ==========================================
    uint32_t frequency = gRxVfo->pRX->Frequency;
    unsigned int MHz = frequency / 100000;
    unsigned int kHz = frequency % 100000;
    sprintf(string, "%u.%05u", MHz, kHz);
    UI_PrintString(string, 2, 16, 2, true);

    // ==========================================
    // LINE 3: Bottom Status Bar (Split into 4 columns)
    // ==========================================
    // Column 1: Channel / VFO mode indicator
    if (IS_MR_CHANNEL(gRxVfo->CHANNEL_SAVE)) {
        sprintf(string, "CH-%03u", gRxVfo->CHANNEL_SAVE + 1);
    } else {
        sprintf(string, "VFO-%u", gEeprom.RX_VFO);
    }
    UI_PrintStringSmallBold(string, 0, 31, 5);

    // Column 2: Power Level
    switch (gTxVfo->OUTPUT_POWER) {
        case OUTPUT_POWER_LOW1:
        case OUTPUT_POWER_LOW2:
            strcpy(string, "PWR:L");
            break;
        case OUTPUT_POWER_MID:
            strcpy(string, "PWR:M");
            break;
        case OUTPUT_POWER_HIGH:
        default:
            strcpy(string, "PWR:H");
            break;
    }
    UI_PrintStringSmallBold(string, 32, 63, 5);

    // Column 3: CTCSS / Tone info placeholder
    strcpy(string, "TONE");
    UI_PrintStringSmallBold(string, 64, 95, 5);

    // Column 4: Battery Indicator
    sprintf(string, "%uV", gBatteryVoltages[0] / 100);
    UI_PrintStringSmallBold(string, 96, 127, 5);

    // Render the final buffer to the LCD screen
    ST7565_BlitFullScreen();
}