void ChecktoDeleteID(){
  WiFiClient client;
  HTTPClient http;    //Declare object of class HTTPClient
  //Post Data
  postData = "DeleteID=check"; // Add the Fingerprint ID to the Post array in order to send it
  // Post methode
 
  http.begin(client, link); //initiate HTTP request, put your Website URL or Your Computer IP 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
 
  if (payload.substring(0, 6) == "del-id") {
    String del_id = payload.substring(6);
    USE_SERIAL.println(del_id);
    deleteFingerprint(del_id.toInt());
  }
  
  http.end();  //Close connection
}
uint8_t deleteFingerprint( int id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);
 
  if (p == FINGERPRINT_OK) {
    //Serial.println("Deleted!");
    oled.clearDisplay();
    oled.setTextSize(2);             // Normal 2:2 pixel scale
    oled.setTextColor(WHITE);        // Draw white text
    oled.setCursor(0,0);             // Start at top-left corner
    oled.print(F("Huella eliminada\n"));
    oled.display();
  
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    oled.clearDisplay();
    oled.setTextSize(1);             // Normal 1:1 pixel scale
    oled.setTextColor(WHITE);        // Draw white text
    oled.setCursor(0,0);             // Start at top-left corner
    oled.print(F("Communication error!\n"));
    oled.display();
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not delete in that location");
    oled.clearDisplay();
    oled.setTextSize(1);             // Normal 1:1 pixel scale
    oled.setTextColor(WHITE);        // Draw white text
    oled.setCursor(0,0);             // Start at top-left corner
    oled.print(F("Could not delete in that location!\n"));
    oled.display();
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    oled.clearDisplay();
    oled.setTextSize(1);             // Normal 1:1 pixel scale
    oled.setTextColor(WHITE);        // Draw white text
    oled.setCursor(0,0);             // Start at top-left corner
    oled.print(F("Error writing to flash!\n"));
    oled.display();
    return p;
  } else {
    //Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    oled.clearDisplay();
    oled.setTextSize(2);             // Normal 2:2 pixel scale
    oled.setTextColor(WHITE);        // Draw white text
    oled.setCursor(0,0);             // Start at top-left corner
    oled.print(F("Unknown error:\n"));
    oled.display();
    return p;
  }   
}
