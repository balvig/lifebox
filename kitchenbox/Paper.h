#ifndef Lifeboxes_paper_h
#define Lifeboxes_paper_h

#include "Arduino.h"
#include <GxEPD.h>
#include <GxGDE0213B1/GxGDE0213B1.h>      // 2.13" b/w
//#include <GxIO/GxIO_SPI/GxIO_SPI.h>
//#include <GxIO/GxIO.h>

//#include <Fonts/TomThumb.h>
//#include <Fonts/FreeMonoBold9pt7b.h>


namespace Lifeboxes  {
  class Paper {
    public:
      Paper(GxEPD_Class* display);
      void renderText(const String text, const uint8_t page = 1, const bool fullRefresh = true);
      void renderPartial();
      void renderFull();
    private:
      GxEPD_Class* _display;
  };
}
#endif
