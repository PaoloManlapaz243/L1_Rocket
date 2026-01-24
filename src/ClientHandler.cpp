// #include <ClientHandler.h>
// #include <WiFi.h>

// ClientHandler::ClientHandler(WiFiClient client)
// {
//     this->client = client;
// }

// ClientHandler::~ClientHandler()
// {
//     client.stop();
// }

// String ClientHandler::readRequest()
// {
//     //TODO
//     while(client.connected() && client.available())
//     {
//         char c = client.read();
//     }

//     return "";
// }

// void ClientHandler::sendData(String data)
// {
//     client.println("<h1>" + data + "</h1>");
// }

// void ClientHandler::startSendData()
// {
//     client.println("HTTP/1.1 200 OK");
//     client.println("Content-type:text/html");
//     client.println("Connection: close");
//     client.println(); 

//     client.println("<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
//     client.println("<body>");
// }

// void ClientHandler::endSendData()
// {
//     client.println("</body>");
//     client.println("</html>");
// }

#include <ClientHandler.h>

ClientHandler::ClientHandler(WiFiClient aclient)
: client(aclient)
{}

ClientHandler::~ClientHandler() {
    client.stop();
}

// -------------------------------------------------------------------
// Read ONLY the first line of the HTTP request
// Example: "GET /data HTTP/1.1"
// -------------------------------------------------------------------
String ClientHandler::readRequest() {
    if (!client.connected()) return "";
    if (!client.available()) return "";

    String line = client.readStringUntil('\r');
    client.readStringUntil('\n'); // consume newline
    return line;
}

// -------------------------------------------------------------------
// Send plain text numeric data for AJAX requests
// -------------------------------------------------------------------
void ClientHandler::sendData(String data) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.print(data);
}

// -------------------------------------------------------------------
// Serve the main HTML page (served only when GET / is requested)
// -------------------------------------------------------------------
void ClientHandler::sendHTML() {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    client.println("<!DOCTYPE html><html><head>");
    client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("</head><body>");

    client.println("<h1>AJAX Live Data</h1>");
    client.println("<p>Value: <span id='val'>0</span></p>");

    client.println("<script>");
    client.println("function update() {");
    client.println("  fetch('/data').then(r => r.text()).then(t => {");
    client.println("    document.getElementById('val').innerText = t;");
    client.println("  });");
    client.println("}");
    client.println("setInterval(update, 1000);");
    client.println("</script>");

    client.println("</body></html>");
}

// -------------------------------------------------------------------
void ClientHandler::send404() {
    client.println("HTTP/1.1 404 Not Found");
    client.println("Connection: close");
    client.println();
}
