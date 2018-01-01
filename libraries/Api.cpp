#include "Api.h"

namespace Lifeboxes {
  Api::Api(String endpoint, size_t bufferSize) {
    _endpoint = endpoint;
    _bufferSize = bufferSize;
    _http.setTimeout(10000);
  }

  JsonObject& Api::fetchJson() {
    String results = _fetchRaw();
    DynamicJsonBuffer jsonBuffer(_bufferSize);
    return jsonBuffer.parseObject(results);
  }

  // Private
  String Api::_fetchRaw() {
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

