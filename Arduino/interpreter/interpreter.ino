#define NUM_PIXELS 30
#define PROGRAM_LENGTH 40 //(40 5 byte instructions)
//SET is R*2, others are not doubled


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

char *program = "f" ;
char *program_end;

pixel pixels[NUM_PIXELS];

void setup() {
  // put your setup code here, to run once:
  current_frame = 0;
  for (int a; a < NUM_PIXELS; a++) {
    pixels[a].till_frame = 0;
    pixels[a].last_instruction = NULL;
    pixels[a].doing = 4;
  }
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
    pixels[pixel_to_fetch].till_frame 
    + ((cmd[0] <<3) + (cmd[1]) >> 3);

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
  if (pixels[pixel_to_fetch].till_frame == current_frame){
    //not initialized
    if (!pixels[pixel_to_fetch].last_instruction)
      pixels[pixel_to_fetch].last_instruction = (byte *) program;
    byte *sought = pixels[pixel_to_fetch].last_instruction;
    
    while (seeking && (sought < (byte *)program_end)) {
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
  }
  
  return NULL;
}
void loop() {
  // put your main code here, to run repeatedly:

}
