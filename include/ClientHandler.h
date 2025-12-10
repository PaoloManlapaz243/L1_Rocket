#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <WiFi.h>

class ClientHandler
{
    private:
    
        WiFiClient client;

    public:
    
        ClientHandler(WiFiClient);
};

#endif