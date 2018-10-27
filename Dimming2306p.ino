/*
programa diseñado para EFIS500 control automatico de dimming con otodiodo esd1-e
12/07/2018 
by: JasaVM to: REDCorp.

+se agrega tomar X muestras y sacar promedio para evitar variaciones y sea
mas constante la indicacion... 
+se agrega promedio de promedios
+se agrega potenciometro de ajuste para offset
+se toman limites infeior y superior
 */

#define MUESTRAS 100

// These constants won't change.  They're used to give names
// to the pins used:
int previousValue = 21;  //Value previous
int Sig = 0;  //para indicar si es valor ascendente o descendente
boolean Ven = false;

long sum = 0;
long sumap = 0;
int arrayM[MUESTRAS];
int promedio[MUESTRAS];

int offsetValue ;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
    pinMode(10, OUTPUT);
    for(int i = 0; i < MUESTRAS; i++)
      arrayM[i] = 0;
}

void loop() {
  Serial.print("-----------------nueva lectura--------------");
  //read the offset in Pin A2 and add with the value outputValue
   
  analogReference(DEFAULT);
   delay(100);
  offsetValue = map(analogRead(A5),0,1023,-25,25);
  offsetValue = map(analogRead(A5),0,1023,-25,25);
  offsetValue = map(analogRead(A5),0,1023,-25,25);
  
  Serial.print("\toffsetValue = ");   Serial.println(offsetValue); 

    analogReference(EXTERNAL);
   delay(100);
    //bucle for to obtain average 
    for(int k = 0; k < MUESTRAS; k++){
        for(int i = 0; i < MUESTRAS; i++){
            arrayM[i] = analogRead(A4);
            arrayM[i] = analogRead(A4);
            arrayM[i] = analogRead(A4);
            //Serial.print(" arrayM["); Serial.print(i);Serial.print("] = "); 
            //Serial.println(arrayM[i]); 
        }
        
        sum = 0;
        for(int i= 0; i < MUESTRAS; i++){
          sum = sum + arrayM[i];
        }
       // Serial.print(" sum = ");   Serial.println(sum); 
        
        promedio[k] = sum / MUESTRAS;
        //Serial.print("--> promedio[");Serial.print(k);Serial.print("] = ");  
        //Serial.println(promedio[k]); 
    }

  //add averages
  sumap = 0;
  for(int i = 0; i < MUESTRAS; i++)
      sumap = sumap + promedio[i];
  //Serial.print(" --> suma promedios = ");   Serial.println(sumap); 

// average of average
  sumap = sumap / MUESTRAS;
  //Serial.print("-----> promedio de promedios = ");Serial.println(sumap);
  
  // map it to the range of the analog out:
  int outputValue = map(sumap, 90, 580, 20, 255); 
  outputValue += offsetValue;       //adjust with offset value
  
  int tot = (outputValue - previousValue);
  Sig = (abs(tot)/tot);
  
    Serial.print("  previousValue = ");   Serial.print(previousValue);
    Serial.print("\t ***outputValue = ");   Serial.print(outputValue); 
    
    Serial.print("***\t Sig = ");   Serial.print(Sig);
    Serial.print("\t tot = ");   Serial.println(tot);
    //delay (1000);

        while ( previousValue < outputValue || previousValue > outputValue ){
  
                previousValue = previousValue + Sig;
                analogWrite(10, previousValue);
                //if is more lower than 3, output PWM is max value and break function
                if (previousValue < 3){
                  analogWrite(10, 2);
                break;
                }
                //if is more greather than 255, output PWM is max value and break function
                  if (previousValue > 255){
                      analogWrite(10, 255);
                      break;
                  }

                  Serial.print("outputValue = ");
                  Serial.print(outputValue);
                  Serial.print("\t previousValue = ");
                  Serial.println(previousValue);
                  delay(50); 
                   Ven = true;
        }
        delay(1000);
  if (Ven){
    previousValue = outputValue;
    Ven = false;
  }
}
