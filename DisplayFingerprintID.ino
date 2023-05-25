void DisplayFingerprintID(){
  //Fingerprint has been detected 
  if (FingerID > 0){
    oled.clearDisplay();
    oled.drawBitmap( 34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
    oled.display();
    SendFingerprintID( FingerID ); // Send the Fingerprint ID to the website.
      
  }
  //---------------------------------------------
  //No finger detected
  else if (FingerID == 0){
    oled.clearDisplay();
    oled.drawBitmap( 32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
    oled.display();
      
  }
  //---------------------------------------------
  //Didn't find a match
  else if (FingerID == -1){
    oled.clearDisplay();
    oled.drawBitmap( 34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, WHITE);
    oled.display();
    delay(500);
  }
  //---------------------------------------------
  //Didn't find the scanner or there an error
  else if (FingerID == -2){
    oled.clearDisplay();
    oled.drawBitmap( 32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
    oled.display();
  }
}
