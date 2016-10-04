
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

char *program = "\x00\x00\x20\x05\x0A\x01\x03\xC0\x00\x00\x02\x00\x20\x05\x0A\x03\x03\xC0\x00\x00\x04\x00\x20\x05\x0A\x05\x03\xC0\x00\x00\x06\x00\x20\x05\x0A\x07\x03\xC0\x00\x00\x08\x00\x20\x05\x0A\x09\x03\xC0\x00\x00\x0A\x00\x20\x05\x0A\x0B\x03\xC0\x00\x00\x0C\x00\x20\x05\x0A\x0D\x03\xC0\x00\x00\x0E\x00\x20\x05\x0A\x0F\x03\xC0\x00\x00\x10\x00\x20\x05\x0A\x11\x03\xC0\x00\x00\x12\x00\x20\x05\x0A\x13\x03\xC0\x00\x00\x14\x00\x20\x05\x0A\x15\x03\xC0\x00\x00\x16\x00\x20\x05\x0A\x17\x03\xC0\x00\x00\x18\x00\x20\x05\x0A\x19\x03\xC0\x00\x00\x1A\x00\x20\x05\x0A\x1B\x03\xC0\x00\x00\x1C\x03\xC0\x00\x00\x1D\x03\xC0\x00\x00\x00\x39\x00\x00\x81\x01\x0C\x80\x00\x00\x02\x39\x00\x00\x81\x03\x0C\x80\x00\x00\x04\x39\x00\x00\x81\x05\x0C\x80\x00\x00\x06\x39\x00\x00\x81\x07\x0C\x80\x00\x00\x08\x39\x00\x00\x81\x09\x0C\x80\x00\x00\x0A\x39\x00\x00\x81\x0B\x0C\x80\x00\x00\x0C\x39\x00\x00\x81\x0D\x0C\x80\x00\x00\x0E\x39\x00\x00\x81\x0F\x0C\x80\x00\x00\x10\x39\x00\x00\x81\x11\x0C\x80\x00\x00\x12\x39\x00\x00\x81\x13\x0C\x80\x00\x00\x14\x39\x00\x00\x81\x15\x0C\x80\x00\x00\x16\x39\x00\x00\x81\x17\x0C\x80\x00\x00\x18\x39\x00\x00\x81\x19\x0C\x80\x00\x00\x1A\x39\x00\x00\x81\x1B\x0C\x80\x00\x00\x1C\x0C\x80\x00\x00\x1D\x0C\x80\x00\x00\x00\x59\x00\x00\x81\x01\x0C\x80\x00\x00\x02\x59\x00\x00\x81\x03\x0C\x80\x00\x00\x04\x59\x00\x00\x81\x05\x0C\x80\x00\x00\x06\x59\x00\x00\x81\x07\x0C\x80\x00\x00\x08\x59\x00\x00\x81\x09\x0C\x80\x00\x00\x0A\x59\x00\x00\x81\x0B\x0C\x80\x00\x00\x0C\x59\x00\x00\x81\x0D\x0C\x80\x00\x00\x0E\x59\x00\x00\x81\x0F\x0C\x80\x00\x00\x10\x59\x00\x00\x81\x11\x0C\x80\x00\x00\x12\x59\x00\x00\x81\x13\x0C\x80\x00\x00\x14\x59\x00\x00\x81\x15\x0C\x80\x00\x00\x16\x59\x00\x00\x81\x17\x0C\x80\x00\x00\x18\x59\x00\x00\x81\x19\x0C\x80\x00\x00\x1A\x59\x00\x00\x81\x1B\x0C\x80\x00\x00\x1C\x0C\x80\x00\x00\x1D\x0C\x80\x00\x00";
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
 // Serial.begin(9600);

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
    + ((c[0] <<3) + (c[1] >> 5));
//  C   F         R        G        B
 //000 00000|000 00000|00 000000|0 0000000
  //1                  X 
  pixels[pixel_to_fetch].ir = ((c[1] << 2) & B01111100) + ((c[2] >> 6) & B00000011);
  pixels[pixel_to_fetch].ir = pixels[pixel_to_fetch].ir & B01111111;
  pixels[pixel_to_fetch].ig = ((c[2]  << 1) & B01111110) + ((c[3] >> 8) & B00000001);
  pixels[pixel_to_fetch].ig = pixels[pixel_to_fetch].ig & B01111111;
  pixels[pixel_to_fetch].ib = c[3] & B01111111;
// Serial.println("Debug:");
 //Serial.println(((c[0] <<3) + (c[1] >> 5)));
 //Serial.println(pixels[pixel_to_fetch].ir);
 //Serial.println(pixels[pixel_to_fetch].ig);
 //Serial.println(pixels[pixel_to_fetch].ib);//bit
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
  
    //not initialized
    if (!pixels[pixel_to_fetch].last_instruction)
      pixels[pixel_to_fetch].last_instruction = (byte *) program;
    byte *sought = pixels[pixel_to_fetch].last_instruction+5;
    
    while (true) {
       if (*sought == pixel_to_fetch){
          return sought;    
        //found next instruction
       }
       else{
        //PTR Arithmatic
        sought = sought + 5;
        if (sought >  (byte *) program_end){
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
