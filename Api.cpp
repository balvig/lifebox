#include <Arduino.h>
#include "Api.h"

namespace Lifebox {
  Api::Api(String endpoint) {
    _endpoint = endpoint;
    _http.setReuse(true);
    _http.setTimeout(10000);
  }
  
  String Api::fetch() {
    _http.begin(_endpoint);

    String result = "";
    int httpCode = _http.GET();
  
    if (httpCode == HTTP_CODE_OK) {
      status = SUCCESS;
      result = _http.getString();
    }
    else {
      status = FAILED;
    }
    
    _http.end();
    return result;
  }
}

