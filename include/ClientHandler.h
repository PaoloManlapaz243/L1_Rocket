// #ifndef CLIENT_HANDLER_H
// #define CLIENT_HANDLER_H

// #include <WiFi.h>

// class ClientHandler
// {
//     private:
    
//         WiFiClient client;

//     public:
    
//         ClientHandler(WiFiClient);
//         ~ClientHandler();
//         String readRequest();
//         void sendData(String);
//         void startSendData();
//         void endSendData();
// };

// #endif

#pragma once
#include <WiFi.h>

class ClientHandler {
public:
    ClientHandler(WiFiClient client);
    ~ClientHandler();

    String readRequest();      // Reads request line only
    void sendHTML();
    void sendData(String data);
    void send404();

private:
    WiFiClient client;
};
