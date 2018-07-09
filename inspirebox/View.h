#ifndef Lifeboxes_view_h
#define Lifeboxes_view_h

#include "Arduino.h"
#include "GxEPD.h"
#include "GxGDEW029Z10/GxGDEW029Z10.cpp"    // 2.9" b/w/r
//#include "GxIO/GxIO_SPI/GxIO_SPI.cpp"
//#include "GxIO/GxIO.cpp"

namespace Lifeboxes  {
  class View {
    public:
      View(GxEPD_Class& display);
      void battery();
    private:
      //GxEPD_Class *_display;
  };
}
#endif
