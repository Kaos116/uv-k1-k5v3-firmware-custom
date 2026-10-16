#include <string.h>
#include <stdio.h>
#include "driver/st7565.h"
#include "ui/helper.h"
#include "functions.h"
#include "radio.h"
#include "settings.h"
#include "ui/inputbox.h"
#include "ui/ui.h"

// Fixed X offset starting points for Line 3 status columns
#define COL_CHAN_X   0
#define COL_PWR_X    44
#define COL_TONE_X   76
#define COL_BAT_X    112

void UI_DisplayCustomScreen(void) {
    char str[18];

    // 1. Clear LCD screen frame buffer
    memset(gFrameBuffer, 0, sizeof(gFrameBuffer));

    // =========================================================================
    // LINE 1: Header (16 Characters Max)
    // =========================================================================
    UI_PrintStringSmall("UV-K1 CUSTOM", 0, 127, 0);

    // =========================================================================
    // LINE 2: Large Frequency Display
    // =========================================================================
    sprintf(str, "%u.%05u", 
            gEeprom.VfoInfo[gEeprom.TX_VFO].pRX->Frequency / 100000, 
            gEeprom.VfoInfo[gEeprom.TX_VFO].pRX->Frequency % 100000);
    UI_PrintStringSmall(str, 12, 127, 2);

    // =========================================================================
    // LINE 3: Status Columns (Page 6)
    // =========================================================================

    // Col 1: Channel # or VFO (CH-001 or VFO A)
    if (gEeprom.SCREEN_CHANNEL_MODE == MR_MODE) {
        sprintf(str, "CH-%03d", gEeprom.MrChannel[gEeprom.TX_VFO] + 1);
    } else {
        sprintf(str, "VFO %c", (gEeprom.TX_VFO == 0) ? 'A' : 'B');
    }
    UI_PrintStringSmall(str, COL_CHAN_X, 0, 6);

    // Col 2: Power Level (LOW, MED, HIGH)
    switch (gEeprom.VfoInfo[gEeprom.TX_VFO].OUTPUT_POWER) {
        case OUTPUT_POWER_LOW:  strcpy(str, "LOW"); break;
        case OUTPUT_POWER_MID:  strcpy(str, "MED"); break;
        case OUTPUT_POWER_HIGH: default: strcpy(str, "HIGH"); break;
    }
    UI_PrintStringSmall(str, COL_PWR_X, 0, 6);

    // Col 3: CTCSS Tone (123.0 - blank if none set)
    if (gEeprom.VfoInfo[gEeprom.TX_VFO].SCRAMBLER_TYPE == 0 && 
        gEeprom.VfoInfo[gEeprom.TX_VFO].CodeType == CODE_TYPE_CTCSS) {
        uint16_t tone = CTCSS_Options[gEeprom.VfoInfo[gEeprom.TX_VFO].Code];
        sprintf(str, "%u.%u", tone / 10, tone % 10);
        UI_PrintStringSmall(str, COL_TONE_X, 0, 6);
    }

    // Col 4: Battery Icon
    UI_DrawBattery(COL_BAT_X, 6);

    // Push buffer to physical LCD
    ST7565_Render();
}