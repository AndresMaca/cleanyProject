    ///140 grados
    
    /// Variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int encoder_pin = 2;             //Pin 2, donde se conecta el encoder       
    unsigned int rpm = 0;           // Revoluciones por minuto calculadas.
    float velocity = 0;                 //Velocidad en [Km/h]
    volatile byte pulses = 0;       // Número de pulsos leidos por el Arduino en un segundo
    unsigned long timeold = 0;  // Tiempo 
    int totalPulses=0;
    unsigned int pulsesperturn = 20; // Número de muescas que tiene el disco del encoder.
    const int wheel_diameter = 64;   // Diámetro de la rueda pequeña[mm]
    static volatile unsigned long debounce = 0; // Tiempo del rebote.

    ////ULtrasonico///////

    const int trigPin = 41;
    const int echoPin = 40;
    long duration;
    int distance;
    //BLuetooth
    #include <SoftwareSerial.h>
    //problema al usar el serial y el bluetoth al mismo tiempo, las pruebas se hacen por separado.
    
    SoftwareSerial bluetooth(10, 11); // TX, RX
    
    #define MAX_BUFFER 4
    
    int motor = 9;    
    int speed = 0;
    char data;
    char* buffer;
    boolean receiving = false;
    int pos;
    
//Transistores 
int waterPump=31;
int vacuumCleaner=30;
int servoMotor=32;  

 //Puente H
int ENB = 53; 
// Motor A
 
int ENA = 52;
int in1 = 42;
int in2 = 43;
 
// Motor B
int in3 = 44;
int in4 = 45;
    ////  Configuración del Arduino /////////////////////////////////////////////////////////
    void setup(){
        Serial.begin(9600); // Configuración del puerto serie  
        pinMode(encoder_pin, INPUT); // Configuración del pin nº2
        attachInterrupt(0, counter, RISING); // Configuración de la interrupción 0, donde esta conectado. 
        pinMode(servoMotor,OUTPUT);
        pinMode(vacuumCleaner,OUTPUT);
        pinMode(waterPump,OUTPUT);
        
        pulses = 0;
        rpm = 0;
        totalPulses=0;
        timeold = 0;
        
        //Ultrasonico////
        pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
        pinMode(echoPin, INPUT); // Sets the echoPin as an Input
        //Bluetooth
        // bluetooth.begin(9600);
        //bluetooth.println("Bluetooth On");
        // pinMode(motor, OUTPUT);
        
       //  buffer = new char[MAX_BUFFER];
       pinMode (ENB, OUTPUT); 

      
 
    }
    ////  Programa principal ///////////////////////////////////////////////////////////////////////
    void loop(){
        ///Ultrasonico
        if (millis() - timeold >= 500){  // Se actualiza cada segundo  After approximately 50 days.
        noInterrupts(); //Don't process interrupts during calculations // Desconectamos la interrupción para que no actué en esta parte del programa.

        timeold = millis(); // Almacenamos el tiempo actual.
        Serial.println(totalPulses);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(echoPin, HIGH);
        // Calculating the distance
        distance= duration*0.034/2;
        // Prints the distance on the Serial Monitor
        
        //Ultrasonico
        Serial.print("Distance: ");
        Serial.println(distance);
        if(distance<10){
          stopMotor();
          digitalWrite(waterPump,HIGH);
          digitalWrite(vacuumCleaner,HIGH);
          digitalWrite(servoMotor,HIGH);
            
        }else{
               forward();
               digitalWrite(waterPump,LOW);
               digitalWrite(vacuumCleaner,LOW);
               digitalWrite(servoMotor,LOW);
               

        }
        //BT
        /*
        if (bluetooth.available()){
            
            data=bluetooth.read();
            
             switch(data) {
                //3: End of transmission
                case 3:  receiving = false;  
                        speed = buffer2int(buffer);
                                   
                        bluetooth.print("Received: ");
                        bluetooth.print(buffer);
                        bluetooth.print(", Speed: ");
                        bluetooth.println(speed);
     
                         break; //end message
                default: if (receiving == false) resetData();
                        buffer[pos] = data;
                        pos++;
                         receiving = true;          
              }
       }  
      analogWrite(motor, speed); 
      delay(10);  
      */                      
      //
        interrupts(); // Restart the interrupt processing // Reiniciamos la interrupción
        }
    }
    ////Fin de programa principal //////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////Función que cuenta los pulsos buenos ///////////////////////////////////////////


void resetData(){
    for (int i=0; i<=pos; i++) buffer[i] = 0; 
    pos = 0;
  }
     
  int buffer2int(char* buffer){
   int i;
   sscanf(buffer, "%d", &i);
   return i;
   
  }
    void counter(){
    if(  digitalRead (encoder_pin) && (micros()-debounce > 500) && digitalRead (encoder_pin) ) { 
    // Vuelve a comprobar que el encoder envia una señal buena y luego comprueba que el tiempo es superior a 1000 microsegundos y vuelve a comprobar que la señal es correcta.
            debounce = micros(); // Almacena el tiempo para comprobar que no contamos el rebote que hay en la señal.
            totalPulses++;}  // Suma el pulso bueno que entra.
            else ; }

   void forward(){
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
           
                      
            // Turn on motor B
            
           
            digitalWrite(in3, HIGH);
            digitalWrite(in4, LOW);
            analogWrite(ENB,255);
   
            analogWrite(ENA,255);

   }
    void stopMotor(){
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
                      
            // Turn on motor B
           
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);
   }
  