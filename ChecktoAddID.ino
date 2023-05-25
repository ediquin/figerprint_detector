void ChecktoAddID(){
  WiFiClient client;
  HTTPClient http;    //Declare object of class HTTPClient
  //Post Data
  postData = "Get_Fingerid=get_id"; // Add the Fingerprint ID to the Post array in order to send it
  // Post methode
 
  http.begin(client, link); //initiate HTTP request, put your Website URL or Your Computer IP 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
  if (payload.substring(0, 6) == "add-id") {
    String add_id = payload.substring(6);
    USE_SERIAL.println(add_id);
    id = add_id.toInt();
    getFingerprintEnroll();
  }
  http.end();  //Close connection
}
//******************Enroll a Finpgerprint ID*****************
uint8_t getFingerprintEnroll() {
 
  int p = -1;
  oled.clearDisplay();
  oled.drawBitmap( 34, 6, FinPr_scan_bits, FinPr_scan_width, FinPr_scan_height, WHITE);
  oled.display();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      oled.clearDisplay();
      oled.drawBitmap( 34, 6, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
      oled.display();
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      oled.setTextSize(1);             // Normal 2:2 pixel scale
      oled.setTextColor(WHITE);        // Draw white text
      oled.setCursor(0,0);             // Start at top-left corner
      oled.print(F("Ponga su huella..."));
      oled.display();
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      oled.clearDisplay();
      oled.drawBitmap( 34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, WHITE);
      oled.display();
      break;
    case FINGERPRINT_IMAGEFAIL:
      USE_SERIAL.println("Imaging error");
      break;
    default:
      USE_SERIAL.println("Unknown error");
      break;
    }
  }
 
  // OK success!
 
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      oled.clearDisplay();
      oled.drawBitmap( 34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
      oled.display();
      break;
    case FINGERPRINT_IMAGEMESS:
      oled.clearDisplay();
      oled.drawBitmap( 34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, WHITE);
      oled.display();
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  oled.clearDisplay();
  oled.setTextSize(2);             // Normal 2:2 pixel scale
  oled.setTextColor(WHITE);        // Draw white text
  oled.setCursor(0,0);             // Start at top-left corner
  oled.print(F("Levante su"));
  oled.setCursor(0,20);
  oled.print(F("Dedo"));
  oled.display();
  //Serial.println("Remove finger");

  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  oled.clearDisplay();
  oled.drawBitmap( 34, 6, FinPr_scan_bits, FinPr_scan_width, FinPr_scan_height, WHITE);
  oled.display();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      oled.clearDisplay();
      oled.drawBitmap( 34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
      oled.display();
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      oled.setTextSize(1);             // Normal 2:2 pixel scale
      oled.setTextColor(WHITE);        // Draw white text
      oled.setCursor(0,0);             // Start at top-left corner
      oled.print(F("Ponga su huella..."));
      oled.display();
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }
 
  // OK success!
 
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      oled.clearDisplay();
      oled.drawBitmap( 34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
      oled.display();
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Prints matched!");
    oled.clearDisplay();
    oled.drawBitmap( 34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
    oled.display();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    //Serial.println("Stored!");
    oled.clearDisplay();
    oled.drawBitmap( 34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
    oled.display();
    confirmAdding();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}
//******************Check if there a Fingerprint ID to add******************
void confirmAdding(){
  WiFiClient client;
  HTTPClient http;    //Declare object of class HTTPClient
  //Post Data
  postData = "confirm_id=" + String(id); // Add the Fingerprint ID to the Post array in order to send it
  // Post methode
 
  http.begin(client, link); //initiate HTTP request, put your Website URL or Your Computer IP 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
 
  oled.clearDisplay();
  oled.setTextSize(1.5);             // Normal 1:1 pixel scale
  oled.setTextColor(WHITE);        // Draw white text
  oled.setCursor(0,0);             // Start at top-left corner
  oled.print(payload);
  oled.display();

  Serial.println(payload);
  
  http.end();  //Close connection
}
