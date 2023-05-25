void connectToWiFi(){
    WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)

    WiFi.mode(WIFI_STA);
    USE_SERIAL.print("Connecting to ");
    USE_SERIAL.println(ssid);
    WiFi.begin(ssid, password);
 
    oled.clearDisplay();
    oled.setTextSize(1);             // Normal 1:1 pixel scale
    oled.setTextColor(WHITE);        // Draw white text
    oled.setCursor(0, 0);             // Start at top-left corner
    oled.print(F("Conect"));
    oled.write(160);
    oled.print(F("ndose a \n"));
    oled.setCursor(0, 16);   
    oled.setTextSize(2);          
    oled.print(ssid);
    oled.drawBitmap( 83, 36, epd_bitmap_wifi_37x41, 37, 41, WHITE);
    oled.display();
    
    while (WiFi.status() != WL_CONNECTED) {
      USE_SERIAL.print(".");
    }
    USE_SERIAL.println("");
    USE_SERIAL.println("Conectado");
    
    oled.clearDisplay();
    oled.setTextSize(2);             // Normal 1:1 pixel scale
    oled.setTextColor(WHITE);        // Draw white text
    oled.setCursor(8, 0);             // Start at top-left corner
    oled.print(F("Conectado \n"));
    oled.drawBitmap( 33, 15, Wifi_connected_bits, Wifi_connected_width, Wifi_connected_height, WHITE);
    oled.display();
    
    USE_SERIAL.print("IP address: ");
    USE_SERIAL.println(WiFi.localIP());  //IP address assigned to your ESP
 
}
