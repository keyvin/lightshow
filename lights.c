#include <stdio.h>

//we unpack in order of pixel. If two pixels will be needed they will fetched in order

#define STRIP_LENGTH 30

unsigned void *cmdptr;

typedef struct PIXEL {
  unsigned short cmd;
  unsigned short till_frame;
  unsigned short R;
  unsigned short G;
  unsigned short B;

} pixel_state;
  
unsigned int frame;

pixel_state pixels[STRIP_LENGTH];

int main(int argv, char **argc) {
  unsigned int i;
  pixel_state cmd;
  //first frame
  if (frame == 0){
    for (i=0; i < STRIP_LENGTH; i++){
      cmd = fetch_parse_command();
      pixels[i] = cmd;

    }
  }

  //fetch commands
  for (i=0; i < STRIP_LENGTH; i++){
    if (frame > pixels[i].till_frame){
      pixels[i] = fetch_parse_command();
  }

  
  for (i=0; i < STRIP_LENGTH; i++){
     do_cmd(pixels[i]);
  }

  frame++;
  return 0;
}




