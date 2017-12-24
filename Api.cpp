#include <Arduino.h>
#include "Api.h"

namespace Lifebox {
  Api::Api(String endpoint) {
    _endpoint = endpoint;
    _http.setReuse(true);
  }
  
  String Api::fetch() {
    _http.begin(_endpoint);

    int httpCode = _http.GET();
  
    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
      if (httpCode == HTTP_CODE_OK) {
        String payload = _http.getString();
        _http.end();
        return payload;
      }
    }
    else {
      _http.end();
      return "";
    }
  }
}

