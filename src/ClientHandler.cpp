#include <ClientHandler.h>
#include <WiFi.h>

ClientHandler::ClientHandler(WiFiClient client)
{
    this->client = client;
}

char* ClientHandler::readRequest()
{
    //TODO
    while(client.connected() && client.available())
    {
        char c = client.read();
    }

    return "";
}

void ClientHandler::sendData(String data)
{
    client.println("<h1>" + data + "</h1>");
}

void ClientHandler::startSendData()
{
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println(); 

    client.println("<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
    client.println("<body>");
}

void ClientHandler::endSendData()
{
    client.println("</body>");
}

