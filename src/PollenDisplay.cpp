// #include "PollenDisplay.h"

// void PollenDisplay::onBClick()
// {
//     this->forceFullDraw();
// }

// void PollenDisplay::onBLongPress()
// {
// }

// void PollenDisplay::refreshDisplay()
// {
//     for (uint8_t ix = 0; ix < MAX_POLLEN_COUNTS; ix++)
//     {
//         if (Status::pollenCounts[ix].type != NULL)
//         {
//             this->printPollen(Status::pollenCounts[ix], this->getAreaX(ix), this->getAreaY(ix));
//         }
//     }
// }

// void PollenDisplay::printPollen(PollenCount pollenCount, uint16_t x, uint16_t y)
// {
//     uint16_t midSectionOffset = 115;

//     switch (pollenCount.type)
//     {
//     case POLLEN_CLADOSPORIUM:
//         snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Cladosporium");
//         break;
//     case POLLEN_ALTERNARIA:
//         snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Alternaria");
//         break;
//     case POLLEN_OAK:
//         snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Oak");
//         break;
//     case POLLEN_GRASS:
//         snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Grass");
//         break;
//     case POLLEN_SORREL:
//         snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Sorrell");
//         break;
//     case POLLEN_PINE:
//         snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Pine");
//         break;
//     case POLLEN_PLATAN:
//         snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Platan");
//         break;
//     case POLLEN_PLANTAGO:
//         snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Plantago");
//         break;
//     case POLLEN_OTHER:
//         snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "???");
//         break;
//     }

//     this->printValue(Peripherals::buffer, x, y + 20, midSectionOffset + 310, 40, MAIN_DISPLAY_LABEL_FONT);

//     uint8_t *trendIcon = NULL;

//     switch (pollenCount.trend)
//     {
//     case POLLEN_TREND_EQ:
//         trendIcon = (uint8_t *)trend_eq_data;
//         break;
//     case POLLEN_TREND_U:
//         trendIcon = (uint8_t *)trend_u_data;
//         break;
//     case POLLEN_TREND_UU:
//         trendIcon = (uint8_t *)trend_uu_data;
//         break;
//     case POLLEN_TREND_D:
//         trendIcon = (uint8_t *)trend_d_data;
//         break;
//     case POLLEN_TREND_DD:
//         trendIcon = (uint8_t *)trend_dd_data;
//         break;
//     case POLLEN_TREND_END:
//         trendIcon = (uint8_t *)trend_end_data;
//         break;
//     }

//     if (trendIcon != NULL)
//     {
//         this->showIcon(x + midSectionOffset, y + 80, TREND_EQ_WIDTH, TREND_EQ_WIDTH, trendIcon);
//     }

//     snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%d", pollenCount.count);
//     this->printValue(Peripherals::buffer, x, y + 50, 110, 110, MAIN_DISPLAY_LARGE_FONT, DIS_RIGHT);
// }

// uint16_t PollenDisplay::getAreaX(uint8_t areaNumber)
// {
//     return 105 + (areaNumber % 3) * 300;
// }

// uint16_t PollenDisplay::getAreaY(uint8_t areaNumber)
// {
//     return 50 + ((areaNumber / 3) * 150);
// }