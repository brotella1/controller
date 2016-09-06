int address = 2;
bool dataGetSet = 0;
int dataRegister = 0;
byte registerData[4];
char inByte[6];
char inByteCopy[5];
int dataInByte;
float mashTemp = 152.06;

char inChar;
int index = 0;
bool commsEstablished = false;
String connectionString = "conn";
String inByteString;
unsigned long serialData;


void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
//  establishContact();
}

void loop() {
  getSerial();

  switch (serialData)
  {
    case '0': //Data Get
    {
      getSerial();
      switch (serialData)
      {
        case '0':
        {
          Serial.print("Mash temperature = ");
          Serial.print(mashTemp);
        }
        break;

        case '1':
        {
          Serial.print("Boil temp is 212 degrees");
        }
        break;
        default:
        break;
      }
    }
    break;
    case '1': //Data Set
    {
     Serial.println("Data Set");      
    }

    
    break;
    default:
    Serial.print("Does not match");
    break;
  }
}


long getSerial()
{
  serialData = 0;
  while (dataInByte != '/')
  {
    dataInByte = Serial.read(); 
    if (dataInByte > 0 && dataInByte != '/')
    {
      serialData = dataInByte;
    }
  }
  dataInByte = 0;
  return serialData;
}

void establishContact() {
  while (commsEstablished != true)
  {
    delay(2000);
    if (Serial.available() > 0) {
        Serial.println("Attempting to communication with master");
        for(index = 0; index < 5; index ++) // One less than the size of the array
        {
            inChar = Serial.read(); // Read a character
            inByte[index] = inChar; // Store it
        }
        index++;
        inByte[index] = '\0'; // Null terminate the string

        for(int i = 0; i<4; i++){
          inByteCopy[i] = inByte[i];
        }
        
        inByteString = String(inByteCopy);
        if (inByteString == connectionString){
          commsEstablished = true;
        }
    }
    else {
      Serial.println("No connection to master");
    }
  }
  delay(2000);
  Serial.print("Communications established");
}


