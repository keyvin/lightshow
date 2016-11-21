
#include <Adafruit_NeoPixel.h>

#define NUM_PIXELS 30
#define PROGRAM_LENGTH 40 //(40 5 byte instructions)
//SET is R*2, others are not doubled

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, 8, NEO_GRB + NEO_KHZ800);


typedef struct PIXEL {
  byte num;
  byte r;
  byte g;
  byte b;
  byte doing;
  byte ir;
  byte ig;
  byte ib;
  unsigned long till_frame;
  byte *last_instruction;
} pixel;

unsigned long current_frame;

char *program = "\x00\x00\x20\x00\x0A\x01\x00\x20\x00\x0A\x02\x00\x20\x00\x0A\x03\x00\x20\x00\x0A\x04\x00\x20\x00\x0A\x05\x00\x20\x00\x0A\x06\x00\x20\x00\x0A\x07\x00\x20\x00\x0A\x08\x00\x20\x00\x0A\x09\x00\x20\x00\x0A\x0A\x00\x20\x00\x0A\x0B\x00\x20\x00\x0A\x0C\x00\x20\x00\x0A\x0D\x00\x20\x00\x0A\x0E\x00\x20\x00\x0A\x0F\x00\x20\x00\x0A\x10\x00\x20\x00\x0A\x11\x00\x20\x00\x0A\x12\x00\x20\x00\x0A\x13\x00\x20\x00\x0A\x14\x00\x20\x00\x0A\x15\x00\x20\x00\x0A\x16\x00\x20\x00\x0A\x17\x00\x20\x00\x0A\x18\x00\x20\x00\x0A\x19\x00\x20\x00\x0A\x1A\x00\x20\x00\x0A\x1B\x00\x20\x00\x0A\x1C\x00\x20\x00\x0A\x1D\x00\x20\x00\x0A\x00\x2F\x00\x00\x01\x01\x2F\x00\x00\x01\x02\x2F\x00\x00\x01\x03\x2F\x00\x00\x01\x04\x2F\x00\x00\x01\x05\x2F\x00\x00\x01\x06\x2F\x00\x00\x01\x07\x2F\x00\x00\x01\x08\x2F\x00\x00\x01\x09\x2F\x00\x00\x01\x0A\x2F\x00\x00\x01\x0B\x2F\x00\x00\x01\x0C\x2F\x00\x00\x01\x0D\x2F\x00\x00\x01\x0E\x2F\x00\x00\x01\x0F\x2F\x00\x00\x01\x10\x2F\x00\x00\x01\x11\x2F\x00\x00\x01\x12\x2F\x00\x00\x01\x13\x2F\x00\x00\x01\x14\x2F\x00\x00\x01\x15\x2F\x00\x00\x01\x16\x2F\x00\x00\x01\x17\x2F\x00\x00\x01\x18\x2F\x00\x00\x01\x19\x2F\x00\x00\x01\x1A\x2F\x00\x00\x01\x1B\x2F\x00\x00\x01\x1C\x2F\x00\x00\x01\x1D\x2F\x00\x00\x01\x00\x4F\x00\x00\x01\x01\x4F\x00\x00\x01\x02\x4F\x00\x00\x01\x03\x4F\x00\x00\x01\x04\x4F\x00\x00\x01\x05\x4F\x00\x00\x01\x06\x4F\x00\x00\x01\x07\x4F\x00\x00\x01\x08\x4F\x00\x00\x01\x09\x4F\x00\x00\x01\x0A\x4F\x00\x00\x01\x0B\x4F\x00\x00\x01\x0C\x4F\x00\x00\x01\x0D\x4F\x00\x00\x01\x0E\x4F\x00\x00\x01\x0F\x4F\x00\x00\x01\x10\x4F\x00\x00\x01\x11\x4F\x00\x00\x01\x12\x4F\x00\x00\x01\x13\x4F\x00\x00\x01\x14\x4F\x00\x00\x01\x15\x4F\x00\x00\x01\x16\x4F\x00\x00\x01\x17\x4F\x00\x00\x01\x18\x4F\x00\x00\x01\x19\x4F\x00\x00\x01\x1A\x4F\x00\x00\x01\x1B\x4F\x00\x00\x01\x1C\x4F\x00\x00\x01\x1D\x4F\x00\x00\x01";
char *program_end = program + (30*5)*3;

pixel pixels[NUM_PIXELS];

void setup() {
  // put your setup code here, to run once:
  current_frame = 0;
  for (int a; a < NUM_PIXELS; a++) {
    pixels[a].till_frame = 0;
    pixels[a].last_instruction = NULL;
    pixels[a].doing = 4;
  }
  strip.begin();
  strip.show(); 
  Serial.begin(115200);

}

void update_pixel(int pixel_to_fetch, byte *cmd)
{
  
  if (cmd == NULL)
    return;
  pixels[pixel_to_fetch].last_instruction = cmd;
  //fetch instruction
  //ignore pixel number
  cmd = cmd + 1;
  unsigned int c[5];
  for (int a = 0; a < 5; a++){
    c[a] = *(cmd + a);
  }
  
  pixels[pixel_to_fetch].doing = cmd[0] >> 5;
  pixels[pixel_to_fetch].till_frame = 
    current_frame 
    + (((c[0] <<3) & B11100000) + ((c[1] >> 5) & B00011111));
    Serial.println((((c[0] <<3) & B11100000) + ((c[1] >> 5) & B00011111)));
//  C   F         R        G        B
 //000 00000|000 00000|00 000000|0 0000000
  //1                  X 
  pixels[pixel_to_fetch].ir = ((c[1] << 2) & B01111100) + ((c[2] >> 6) & B00000011);
  pixels[pixel_to_fetch].ir = pixels[pixel_to_fetch].ir & B01111111;
  pixels[pixel_to_fetch].ig = ((c[2]  << 1) & B01111110) + ((c[3] >> 8) & B00000001);
  pixels[pixel_to_fetch].ig = pixels[pixel_to_fetch].ig & B01111111;
  pixels[pixel_to_fetch].ib = c[3] & B01111111;
}

void do_command(int p_to_do){
  //order of these two affects when it will flip
  byte *cmd = NULL;
  byte doing = 0;
  pixel *p = NULL;
  p = &(pixels[p_to_do]);
  if (pixels[p_to_do].till_frame == current_frame){
    cmd = find_next(p_to_do);
    update_pixel(p_to_do, cmd);
    Serial.println(p->till_frame);
  }
  
  switch (p->doing){
    case 0:
     p->r = p->ir;
     p->g = p->ig;
     p->b = p->ib;
    break;
    case 1:
     p->r = p->r + p->ir;
     p->g = p->g + p->ig;
     p->b = p->b + p->ib;
    break;
    case 2:
     p->r = p->r - p->ir;
     p->g = p->g - p->ig;
     p->b = p->b - p->ib;
    break;
    case 3:
    break;

    
  }
  
  
  
  return;
}

byte * find_next( int pixel_to_fetch){
  bool seeking = true;
    unsigned long p = 0;
    //not initialized
    if (!pixels[pixel_to_fetch].last_instruction)
      pixels[pixel_to_fetch].last_instruction = (byte *) program;
    byte *sought = pixels[pixel_to_fetch].last_instruction+5;
    
    while (true) {
       if (*sought == pixel_to_fetch){
          p = (unsigned long) sought;
          if (pixel_to_fetch == 6)
            Serial.println(p);
          return sought;    
        //found next instruction
       }
       else{
        //PTR Arithmatic
        sought = sought + 5;
        if (sought >  (byte *) program_end){
          if (pixel_to_fetch == 6){
          Serial.println("Wrap Wrap");
          }
          sought = (byte *) program;
          //second pass - this pixel is unprogrammed
          if (seeking == false){
            break;
          }
          seeking = false;
        }
       }
       
    }
  
  
  return NULL;
}
void loop() {
  // put your main code here, to run repeatedly:
  for (int a=0; a < NUM_PIXELS; a++){
    do_command(a);
    strip.setPixelColor(a, strip.Color(pixels[a].r, pixels[a].g, pixels[a].b));
  }
  strip.show();
  current_frame++;
  //Serial.println(current_frame);
  //Serial.println(current_frame);
  delay(3);
}
