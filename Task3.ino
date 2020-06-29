byte num_array[10][7] = { { 0,0,0 }, // 0
                         { 1,0,0 }, // 1
                         { 0,1,0 }, // 2
                         { 1,1,0 }, // 3
                         { 0,0,1 }, // 4
                         { 1,0,1 }, // 5
                         { 0,1,1 }, // 6
                         { 1,1,1 }};  // 7 
byte FloorArray[8] = {0,0,0,0,0,0,0,0};                          
unsigned long PreviousMillis =0,previousMillisM=0,previousOrder=0;                                      
byte CurrentFloor=0,NextFloor=0,ToMove =0,lastStep=0,LS=0,Order=0,Alarm=0,Min=7,UpDown=1 ;            
  
void setup(){
  for(byte i=18;i<=19;i++)pinMode(i,INPUT);
  for(byte i=0;i<=2;i++)pinMode(i,OUTPUT);
  for(byte i=13;i<=17;i++)pinMode(i,OUTPUT);
  for(byte i=3;i<=11;i++)pinMode(i,INPUT);
  attachInterrupt(digitalPinToInterrupt(3), OrderFloor, CHANGE);
}

void loop(){
    if(Order == 2 && millis() >= previousOrder + 220){
      Order = 1; 
    }
    if(Order==1)
    {
      CheckKeypad();
      if(digitalRead(4)==HIGH)
        FloorArray[7]=1;
      else if(digitalRead(5)==HIGH)
        FloorArray[6]=1;  
      else if(digitalRead(6)==HIGH)
        FloorArray[5]=1;
      else if(digitalRead(7)==HIGH)
        FloorArray[4]=1;    
      else if(digitalRead(8)==HIGH)
        FloorArray[3]=1;
      else if(digitalRead(9)==HIGH)
        FloorArray[2]=1;
      else if(digitalRead(10)==HIGH)
        FloorArray[1]=1;
      else if(digitalRead(11)==HIGH)
        FloorArray[0]=1;                    
    }
            
  if(Alarm==0)
  {
     for(byte i=0;i<9;i++)
      if(FloorArray[i]==1)
      {
        if(abs(i-CurrentFloor)<Min){
          NextFloor=i;
          Min=abs(i-CurrentFloor);
          if(i-CurrentFloor>0)
            UpDown=1;
          else
            UpDown=0;  
        }
        ToMove=1;
      }
      if(ToMove&&millis()-PreviousMillis>=2000)
        if(UpDown==1&&CurrentFloor!=NextFloor)
          Move(14,15,16,17);
        else if(UpDown==0&&CurrentFloor!=NextFloor)
          Move(17,16,15,14); 
        if(LS>=16*Min)
        {
          FloorArray[NextFloor]=0;
          Min=10;
          LS=0;
          ToMove=0;
          Order=0;
          CurrentFloor=NextFloor;
          for (byte k=0;k<=2;k++) {
            digitalWrite(k, num_array[CurrentFloor][k]);
          }
          PreviousMillis=millis();
        }
  }
    CheckWeight();
}

void CheckWeight(){
  if(analogRead(18)>715)
  {
    digitalWrite(13,HIGH);
    Alarm=1;
  }
  else{
    digitalWrite(13,LOW); 
    Alarm=0; 
  }
}

void CheckKeypad(){
  if(analogRead(19)==838)
    FloorArray[0]=1;
  else if(analogRead(19)==859)
    FloorArray[2]=1;
  else if(analogRead(19)==881)
    FloorArray[4]=1;
  else if(analogRead(19)==905)
    FloorArray[6]=1;
  else if(analogRead(19)==913)
    FloorArray[1]=1;
  else if(analogRead(19)==938)
    FloorArray[3]=1;
  else if(analogRead(19)==964)
    FloorArray[5]=1;
  else if(analogRead(19)==993)
    FloorArray[7]=1;              
}

void Move(int i, int j ,int k , int m)
{
  if(millis() - previousMillisM > 100)
    { 
      switch(lastStep)
      {
          case 0:
            digitalWrite(i, HIGH);  
            digitalWrite(j, LOW);
            digitalWrite(k, LOW);   
            digitalWrite(m, LOW);   
            break;
          case 1:
            digitalWrite(i, LOW);  
            digitalWrite(j, HIGH);
            digitalWrite(k, LOW);   
            digitalWrite(m, LOW);
            break;
          case 2:
            digitalWrite(i, LOW);  
            digitalWrite(j, LOW);
            digitalWrite(k, HIGH);   
            digitalWrite(m, LOW);
            break;
          case 3:
            digitalWrite(i, LOW);  
            digitalWrite(j, LOW);
            digitalWrite(k, LOW);   
            digitalWrite(m, HIGH);
            break;
   }
   previousMillisM = millis();  
      lastStep++;
      LS++;
    }
   if(lastStep>3)
      lastStep=0;
}

void OrderFloor(){
  if(digitalRead(3) == HIGH){
   if(!Order ){
      previousOrder = millis();    
      Order = 2;
    }
  }
  else{
    if(Order == 2 && millis() < previousOrder + 220){
      Order = 0;
    }
  }
}
