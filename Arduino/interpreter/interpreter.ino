
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
  unsigned int till_frame;
  byte *last_instruction;
} pixel;

unsigned int current_frame;

char *program = "\x00\x03\xDE\x00\x00\x01\x03\xDE\x00\x00\x02\x03\xDE\x00\x00\x03\x03\xDE\x00\x00\x04\x03\xDE\x00\x00\x05\x03\xDE\x00\x00\x06\x03\xDE\x00\x00\x07\x03\xDE\x00\x00\x08\x03\xDE\x00\x00\x09\x03\xDE\x00\x00\x0A\x03\xDE\x00\x00\x0B\x03\xDE\x00\x00\x0C\x03\xDE\x00\x00\x0D\x03\xDE\x00\x00\x0E\x03\xDE\x00\x00\x0F\x03\xDE\x00\x00\x10\x03\xDE\x00\x00\x11\x03\xDE\x00\x00\x12\x03\xDE\x00\x00\x13\x03\xDE\x00\x00\x14\x03\xDE\x00\x00\x15\x03\xDE\x00\x00\x16\x03\xDE\x00\x00\x17\x03\xDE\x00\x00\x18\x03\xDE\x00\x00\x19\x03\xDE\x00\x00\x1A\x03\xDE\x00\x00\x1B\x03\xDE\x00\x00\x1C\x03\xDE\x00\x00\x1D\x03\xDE\x00\x00\x00\x03\xF2\x0C\x32\x01\x03\xF2\x0C\x32\x02\x03\xF2\x0C\x32\x03\x03\xF2\x0C\x32\x04\x03\xF2\x0C\x32\x05\x03\xF2\x0C\x32\x06\x03\xF2\x0C\x32\x07\x03\xF2\x0C\x32\x08\x03\xF2\x0C\x32\x09\x03\xF2\x0C\x32\x0A\x03\xF2\x0C\x32\x0B\x03\xF2\x0C\x32\x0C\x03\xF2\x0C\x32\x0D\x03\xF2\x0C\x32\x0E\x03\xF2\x0C\x32\x0F\x03\xF2\x0C\x32\x10\x03\xF2\x0C\x32\x11\x03\xF2\x0C\x32\x12\x03\xF2\x0C\x32\x13\x03\xF2\x0C\x32\x14\x03\xF2\x0C\x32\x15\x03\xF2\x0C\x32\x16\x03\xF2\x0C\x32\x17\x03\xF2\x0C\x32\x18\x03\xF2\x0C\x32\x19\x03\xF2\x0C\x32\x1A\x03\xF2\x0C\x32\x1B\x03\xF2\x0C\x32\x1C\x03\xF2\x0C\x32\x1D\x03\xF2\x0C\x32\x00\x59\x00\x00\x01\x01\x59\x00\x00\x01\x02\x59\x00\x00\x01\x03\x59\x00\x00\x01\x04\x59\x00\x00\x01\x05\x59\x00\x00\x01\x06\x59\x00\x00\x01\x07\x59\x00\x00\x01\x08\x59\x00\x00\x01\x09\x59\x00\x00\x01\x0A\x59\x00\x00\x01\x0B\x59\x00\x00\x01\x0C\x59\x00\x00\x01\x0D\x59\x00\x00\x01\x0E\x59\x00\x00\x01\x0F\x59\x00\x00\x01\x10\x59\x00\x00\x01\x11\x59\x00\x00\x01\x12\x59\x00\x00\x01\x13\x59\x00\x00\x01\x14\x59\x00\x00\x01\x15\x59\x00\x00\x01\x16\x59\x00\x00\x01\x17\x59\x00\x00\x01\x18\x59\x00\x00\x01\x19\x59\x00\x00\x01\x1A\x59\x00\x00\x01\x1B\x59\x00\x00\x01\x1C\x59\x00\x00\x01\x1D\x59\x00\x00\x01";
char *program_end = program +((30*5)*3);

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
  Serial.begin(9600);

}

void update_pixel(int pixel_to_fetch, byte *cmd)
{
  
  if (cmd == NULL)
    return;
  pixels[pixel_to_fetch].last_instruction = cmd;
  //fetch instruction
  //ignore pixel number
  cmd = cmd + 1;
  
  pixels[pixel_to_fetch].doing = cmd[0] >> 5;
  pixels[pixel_to_fetch].till_frame = 
    current_frame 
    + ((cmd[0] <<3) + (cmd[1] >> 3));
  //Serial.println(pixels[pixel_to_fetch].till_frame);
 //000 00000|000 00000|00 000000|0 0000000
  //1  
  pixels[pixel_to_fetch].ir = (cmd[1] << 3) + (cmd[2] >> 6);
  pixels[pixel_to_fetch].ig = (cmd[2] << 2) + (cmd[3] >> 7);
  pixels[pixel_to_fetch].ib = (cmd[3] << 1) >> 1; //shift out high
                                                  //bit
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
  delay(30);
}
