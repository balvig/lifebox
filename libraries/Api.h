#ifndef Lifeboxes_api_h
#define Lifeboxes_api_h

#include "Arduino.h"
#include "ArduinoJson.h"

#ifdef ESP_PLATFORM
  #include "HTTPClient.h"
#else
  #include "ESP8266HTTPClient.h"
#endif

namespace Lifeboxes  {
  class Api {
    public:
      Api(String endpoint, const size_t bufferSize = 0);
      void post(const String body);
      JsonObject& fetchJson(const String query = "");
      enum {PENDING, SUCCESS, FAILED} status;
    private:
      HTTPClient _http;
      String _endpoint;
      size_t _bufferSize;
      String _fetchRaw(const String url);
  };
}
#endif
