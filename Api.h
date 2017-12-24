#ifndef Lifebox_api_h
#define Lifebox_api_h

#include <Arduino.h>
#include <HTTPClient.h>

namespace Lifebox  {
  class Api {
    public:
      Api(String endpoint);
      String fetch();
    private:
      HTTPClient _http;
      String _endpoint;
  };
}
#endif
