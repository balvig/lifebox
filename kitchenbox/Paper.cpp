#include "Paper.h"

namespace Lifeboxes {
  //const GFXfont* FONT = &FreeSerif9pt7b;
  
  Paper::Paper(GxEPD_Class* display) : _display(display) {
    _display->init();
    _display->setRotation(2);
    //_display->setFont(FONT);
    _display->setTextColor(GxEPD_BLACK, GxEPD_WHITE);
  }

  void Paper::renderText(const String text, const uint8_t page, const bool fullRefresh) {
    int offset = (page - 1) * GxEPD_HEIGHT;
    
    _display->setCursor(0, -offset);
    _display->fillScreen(GxEPD_WHITE);
    _display->println(text);

    if (fullRefresh) {
      _display->update();
    }
    else {
      _display->updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
    }
      
  }

  void Paper::renderFull() {
    _display->update();
  }

  void Paper::renderPartial() {
    _display->updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, true);
  }
}
