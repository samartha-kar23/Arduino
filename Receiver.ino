float msg = 0.0; //contains the message from arduino sender
const int led = 13; //led at pin 13
void setup() {
Serial.begin(9600);//Remember that the baud must be the same on both arduinos
pinMode(led,OUTPUT);
}
void loop() {
//while(Serial.available() > 0) {
           for (byte i=0;i<30;++i){
              while (Serial.available() == 0)
               {}
              msg = Serial.parseFloat();
      //      }
        //   delay(5000);
           //    digitalWrite(led,HIGH);
           //    Serial.println("1");
        //   }
       //    if(msg=='L') {
         //       digitalWrite(led,LOW);
          Serial.println("2");
          Serial.println(msg,1);
           }

}
