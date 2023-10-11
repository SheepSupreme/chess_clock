
#define CLK 4
#define DATA 3
#define SW 2

void setup() {
  pinMode(CLK, INPUT);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DATA, INPUT);
  pinMode(DATA, INPUT_PULLUP);
  pinMode(SW, INPUT);
  pinMode(SW, INPUT_PULLUP);
  Serial.begin (115200);
  Serial.println("KY-040 Start:");
}

static uint8_t prevNextCode = 0;
static uint16_t store=0;

void loop() {
static int8_t c,val;
  if(digitalRead(SW)==0){
    delay(10);
    Serial.println("pressed");
    while(digitalRead(SW==0)){
      if(digitalRead(SW)==1){
        delay(10);
        return 0;
      }
      }
  }
  if( val=read_rotary() ) {
     c +=val;
     Serial.println(val);Serial.print(" ");
   }
}

// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary() {
  static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};
  ß
  prevNextCode <<= 2;
  if (digitalRead(DATA)) prevNextCode |= 0x02;
  if (digitalRead(CLK)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

   // If valid then store as 16 bit data.
   if  (rot_enc_table[prevNextCode] ) {
      store <<= 4;
      store |= prevNextCode;
      //if (store==0xd42b) return 1;
      //if (store==0xe817) return -1;
      if ((store&0xff)==0x2b) return -1;
      if ((store&0xff)==0x17) return 1;
   }
   return 0;
}
