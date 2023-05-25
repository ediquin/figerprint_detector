void socket_emit(String name_event, String data);
void SendFingerprintID( int finger ){
  WiFiClient client;
  HTTPClient http;    //Declare object of class HTTPClient
  //Post Data
 
  // Post methode
 
  http.begin(client ,link); //initiate HTTP request, put your Website URL or Your Computer IP 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  postData = "FingerID=" + String(finger); // Add the Fingerprint ID to the Post array in order to send it
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
  
  USE_SERIAL.println(httpCode);    //
  USE_SERIAL.println(payload);    //Print request response payload
  USE_SERIAL.println(postData);   //Post Data
  USE_SERIAL.println(finger);     //Print fingerprint ID

  postData = "";
  http.end();  //Close connection

  

 if (payload.substring(0, 5) == "login") {
 // if (payload.substring(0, 7) == "\n\nlogin") {
 // if (payload.substring(0, 6) == "\nlogin") {
   String user_name = payload.substring(5);
   // String user_name = payload.substring(7);
    //String user_name = payload.substring(6);
    USE_SERIAL.println(user_name);
    oled.clearDisplay();
    oled.setTextSize(2);             // Normal 2:2 pixel scale
    oled.setTextColor(WHITE);        // Draw white text
    oled.setCursor(0,0);             // Start at top-left corner
    oled.print(F("Bienvenido"));
    oled.setCursor(0,20);
    oled.print(user_name);
    oled.display();
    socket_emit("saludo_socket", user_name);
    ahora = millis();
    tiempoAux = 0;
    while(ahora + t_huella > millis()){
      yield(); // para evitar bloqueos por WDT watchdog timer
      }  
    oled.clearDisplay();
    socket_emit("temperatura_socket", "indicación");
    do{
      delay(200);
      oled.clearDisplay();
      oled.setTextSize(2);             // Normal 2:2 pixel scale
      oled.setTextColor(WHITE);        // Draw white text
      oled.setCursor(0,0);             // Start at top-left corner
      oled.print(F("Ponga su dedo en el sensor de temperatura"));
      oled.display();
      movim_1 = digitalRead(movim_1pin);
      yield();
      } while (movim_1==0);
      ahora = millis();
      while(ahora + t_temperatura > millis()){
      Oled_mlx();
      yield(); // para evitar bloqueos por WDT watchdog timer
      }
      String Temperatura_str = "";     // empty string
      Temperatura_str.concat(temperatura);
      USE_SERIAL.println(temperatura); 
      socket_emit("medir_temperatura_socket", Temperatura_str);

    if(temperatura > 37.5 || temperatura < 34.5){
      socket_emit("temperatura_socket2", "indicación");
      do{
      delay(300);
      oled.clearDisplay();
      oled.setTextSize(2);             // Normal 2:2 pixel scale
      oled.setTextColor(WHITE);        // Draw white text
      oled.setCursor(0,0);             // Start at top-left corner
      oled.print(F("Temperatura irregular, repetir medicion"));
      oled.display();
      delay(1500); 
      movim_1 = digitalRead(movim_1pin);
      yield();
      } while (movim_1==0);
      ahora = millis();
      while(ahora + t_temperatura > millis()){
        Oled_mlx();
        yield(); // para evitar bloqueos por WDT watchdog timer
        }
      Temperatura_str = "";     // empty string
      Temperatura_str.concat(temperatura);
      USE_SERIAL.println(temperatura); 
      socket_emit("medir_temperatura_socket", Temperatura_str);
     if(temperatura > 37.5 || temperatura < 34.5){
        socket_emit("temperatura_socket3", "indicación");
        oled.clearDisplay();
        oled.setTextSize(1);             // Normal 2:2 pixel scale
        oled.setTextColor(WHITE);        // Draw white text
        oled.setCursor(2,0);             // Start at top-left corner
        oled.print(F("Temperatura irregular, por favor visite su médico de cabecera"));
        oled.display();
        delay(2000);
        }
      }
     oled.clearDisplay();
     oled.setTextSize(2);             // Normal 2:2 pixel scale
     oled.setTextColor(WHITE);        // Draw white text
     oled.setCursor(0,0);             // Start at top-left corner
     oled.print(F("Temperatura:"));
     oled.setCursor(0,20);
     oled.print(temperatura);
     oled.display();
     WiFiClient client;
     HTTPClient http;    //Declare object of class HTTPClient
  http.begin(client ,link); //initiate HTTP request, put your Website URL or Your Computer IP 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  String datos_a_enviar = "FingerID_t=" + String(finger) + "&temp=" + Temperatura_str; // Add the Fingerprint ID to the Post array in order to send it
  int respuesta = http.POST(datos_a_enviar);   //Send the request
      String payload1 = http.getString();    //Get the response payload
      USE_SERIAL.println(datos_a_enviar);    //Print request response payload
      USE_SERIAL.println(respuesta);    //Print request response payload
      USE_SERIAL.println(payload1);    //Print request response payload
  datos_a_enviar = "";
  http.end();  //Close connection
       
     delay(1500);
     socket_emit("desinfeccion_m", "desinfeccion1");
      do{
      delay(300);
      oled.clearDisplay();
      oled.setTextSize(2);             // Normal 2:2 pixel scale
      oled.setTextColor(WHITE);        // Draw white text
      oled.setCursor(0,0);             // Start at top-left corner
      oled.print(F("Ponga sus manos en el dispositivo de desinfección"));
      oled.display();
      yield();
      movim_2 = digitalRead(movim_2pin);
      } while (movim_2==1);
      ahora = millis();
      digitalWrite(salida_manos,HIGH);
      USE_SERIAL.println("desinfeccion_manos");  
      while(ahora + t_desinfeccion > millis()){
           //
        yield(); // para evitar bloqueos por WDT watchdog timer
      }
      
      
      digitalWrite(salida_manos,LOW);
      socket_emit("desinfeccion_c", "desinfeccion2");
      do{
      delay(300);
      oled.clearDisplay();
      oled.setTextSize(2);             // Normal 2:2 pixel scale
      oled.setTextColor(WHITE);        // Draw white text
      oled.setCursor(0,0);             // Start at top-left corner
      oled.print(F("Permanezca en el punto de desinfección"));
      oled.display();
      yield();
      movim_3 = analogRead(analogPIN);
      } while (movim_3<=300);
      ahora = millis();
      digitalWrite(salida_cuerpo,HIGH);
      USE_SERIAL.println("desinfeccion de cuerpo");     //
      while(ahora + t_desinfec_cuerpo > millis()){
        
         yield(); // para evitar bloqueos por WDT watchdog timer
      }
      digitalWrite(salida_cuerpo,LOW); 
      socket_emit("terminar", "listo"); 
      yield();
      socket_emit("limpiar_pantalla", "indicación");    
      } else if (payload.substring(0, 6) == "logout") {
       // else if (payload.substring(0, 8) == "\n\nlogout") {
       // else if (payload.substring(0, 7) == "\nlogout") {
      String user_name = payload.substring(6);
       //String user_name = payload.substring(8);
       //String user_name = payload.substring(7);
       USE_SERIAL.println(user_name);

       oled.clearDisplay();
       oled.setTextSize(2);             // Normal 2:2 pixel scale
       oled.setTextColor(WHITE);        // Draw white text
       oled.drawBitmap( 0, 0, epd_bitmap_hasta_luego_120x16, 120, 16, WHITE);
       oled.setCursor(0,22);
       oled.print(user_name);
       oled.display();
       socket_emit("despido_socket", user_name);
       delay(2500);

  }
  socket_emit("limpiar_pantalla", "indicación");
  yield();
  }
void socket_emit(String name_event, String data){
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();
    // add evnet name
    // Hint: socket.on('event_name', ....
    array.add(name_event);
    // add payload (parameters) for the event
    JsonObject param1 = array.createNestedObject();
    param1["now"] =  data;
    // JSON to String (serializion)
    String output;
    serializeJson(doc, output);
    // WDT 
    yield(); // para evitar bloqueos por WDT watchdog timer
    // Send event        
    socketIO.sendEVENT(output);
  }
 
  
