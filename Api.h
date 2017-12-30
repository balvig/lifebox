#ifndef Lifebox_api_h
#define Lifebox_api_h

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

namespace Lifebox  {
  class Api {
    public:
      Api(String endpoint, const size_t bufferSize);
      JsonObject& fetchJson();
      enum {PENDING, SUCCESS, FAILED} status;
    private:
      HTTPClient _http;
      String _endpoint;
      size_t _bufferSize;
      String _fetchRaw();
  };
}
#endif
