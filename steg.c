#include "imgUtils.c"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZEX 512
#define SIZEY 512

int find_lsb(unsigned char pixel){
  /*find the LSB (least significant bit) of an unisgned char and return it*/
  if(pixel & 1){
    return 1;
  }
  else{
    return 0;
  }
}

void toggle_lsb(unsigned char *pixel){
  /*Changes the value of the LSB of the inputted pixel adress*/
  *pixel ^= (1 << 0);
}

void static_finder(unsigned char *pixel){
  int lsb;
  lsb = (*pixel >> 0) & 1U;
  *pixel ^= (-lsb ^ *pixel) & (1UL << 7);
}

int max_chars(){
  /*Find the maximum ammount of charachters (not including the null terminator) that the image is able to store*/
  return ((SIZEX*SIZEY)/8)-1;
}

void convert_to_binary(char c, int bits[8]){
  /*Turns any ASCII charachter into binary and stores it in an array*/
  for(int i=0; i!=8; i++){
    bits[7-i]=(c>>i) & 1;
  }
  return;
}

char bin_to_char(int bin[8]){
  /*Takes in an array holding the bits of a char returns them as a char*/
  char bin_str[9];
  char nums[2];
  nums[0]='0';
  nums[1]='1';
  for(int i=0; i!=8; i++){
    bin_str[i]=nums[bin[i]];
  }
  bin_str[8]='\0';
  return strtol(bin_str, 0, 2);
}

int write_to_string(char string[max_chars()+1]){
  /*Goes over a text file and inputs the files characthers into the inputted string array retruns -2 if file cannot be found, -1 if ther are too many characthers, 0 if there is no problem*/
  FILE *fp;
  char ch;
  int str_pos=0;
  int max=max_chars();

  fp=fopen("text_to_hide.txt", "r");
  if(fp==NULL){//if file cannot be found
    return -2;
  }
  while((ch=fgetc(fp))!=EOF){//traversal
    if(str_pos==max){
      return -1;
    }
    string[str_pos]=ch;
    str_pos++;
  }
  fclose(fp);
  return 0;//good
}

void encode(unsigned char inp[SIZEX][SIZEY], unsigned char out[SIZEX][SIZEY], char string[max_chars()+1]){
  /*Takes in an input image (inp) and changes the LSB of the pixels to match the binary representation of the inputted string in out*/
  int length=strlen(string);
  int bits_pos=0;
  int bits[8];
  int string_pos=0;
  int max=max_chars();
  convert_to_binary(string[string_pos],bits);//convert the char to bits
  for(int y=0; y<SIZEY; y++){//image traversal
    for(int x=0; x<SIZEY; x++){

      if(bits_pos==0){//fill in bits array after it has been all used
        if(string_pos==max || string_pos==length+1){//max space or end of string case
          return;
        }
        convert_to_binary(string[string_pos],bits);//convert the char to bits
        string_pos++;
      }

      if(bits[bits_pos]!=find_lsb(inp[y][x])){//if the lsb does not match
        toggle_lsb(&out[y][x]);//flip the LSB to match
      }

      bits_pos=(bits_pos+1)%8;//modulus 8 to loop through
    }
  }
}

void decode(unsigned char inp[SIZEX][SIZEY]){
  /*goes through an image and fines the LSB of each pixel prints each 8 bits as a char*/
  char bin_str[9];
  int bits[8];
  int bits_pos=0;
  char ch;
  printf("\n");
  printf("The decoded message is:\n");
  for(int y=0; y<SIZEY; y++){//image traversal
    for(int x=0; x<SIZEY; x++){
      bits[bits_pos]=find_lsb(inp[y][x]);
      if(bits_pos==7){//bits pos filled with char bits
        ch=bin_to_char(bits);
        if(ch=='\0'){
          printf("\n");
          return;
        }
        printf("%c",ch);
      }
      bits_pos=(bits_pos+1)%8;//modulus 8 to loop through
    }
  }
}

void pic_static(unsigned char inp[SIZEX][SIZEY], unsigned char out[SIZEX][SIZEY]){
  /*Takes in an input image (inp) and changes pixels to only the last 2 lsb's*/
  for(int y=0; y<SIZEY; y++){//image traversal
    for(int x=0; x<SIZEY; x++){
      out[y][x]=inp[y][x];//set the pixel data
      static_finder(&out[y][x]);//keep only the last two LSB
    }
  }
  return;
}

int main() {
  char string[SIZEY*SIZEX];
  unsigned char data[SIZEY][SIZEX];
  unsigned char encoded[SIZEY][SIZEX];
  unsigned char picture_static[SIZEY][SIZEX];
  int res=write_to_string(string);
  if(res==-1){//file length issues
    printf("Your file has too many characthers please shorten the text\n");
    return 0;
  }
  if(res==-2){//file length issues
    printf("text_to_hide.txt cannot be found\n");
    return 0;
  }

  readPGM("cover.pgm", &data[0][0]);
  readPGM("steg.pgm", &encoded[0][0]);
  readPGM("cover.pgm", &picture_static[0][0]);

  int num=0;
  printf("*****************************\n");
  printf("Hide text with steganography!\n");
  printf("Made by: Johnson Su\n");
  printf("Credit to Mustafa Quraish for creating imgUtils.c\n");
  printf("*****************************\n");
  printf("How to use:\n");
  printf("-Have cover.pgm and steg.pgm in the same folder as this program\n");
  printf("-cover.pgm and steg.pgm must be the same image of size 512 x 512 px\n");
  printf("-cover.pgm is your image which will not be changed\n");
  printf("-steg.pgm is the image which your text will be hidden\n");
  printf("-static.pgm will let you see if there is data hidden in an image\n");
  printf("-When wiping an image, you need it's text to be the the same as text_to_hide.txt \n");
  printf("*****************************\n");
  printf("\n");
  while(num!=4){
    printf("-----------------------------\n");
    printf("Please type in your choice\n");
    printf("0 - Write/wipe hidden text to/from an image\n[Writes on empty images and wipes images with hidden info]\n");
    printf("1 - Pull hidden text from image\n");
    printf("2 - View cover image static\n");
    printf("3 - View steg image static\n");
    printf("4 - Exit program and save\n");
    printf("-----------------------------\n");
    printf("Enter choice:");
    scanf("%d",&num);
    getchar();
    printf("-----------------------------\n");

    if(num==0){//encode image
      encode(data,encoded,string);
      printf("The text file has been put in the image succesfully!\n");
      writePGM("steg.pgm", &encoded[0][0]);
    }
    if(num==1){//decode image
      readPGM("steg.pgm", &encoded[0][0]);
      decode(encoded);
    }
    if(num==2){//cover image static
      pic_static(data,picture_static);
      printf("Static written to static.pgm!\n");
      writePGM("static.pgm", &picture_static[0][0]);
    }
    if(num==3){//steg image static
      pic_static(encoded,picture_static);
      printf("Static written to static.pgm!\n");
      writePGM("static.pgm", &picture_static[0][0]);
    }
  }
  writePGM("steg.pgm", &encoded[0][0]);
  return 0;
}
