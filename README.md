# Steg-Hide
A C based program that uses steganography to hide text in images.</br></br>
Currently supports:
-  LSB steganography on PGM (portable grey map) images

Next Updates:
- LSB steganography on BMP (bitmap) images
- LSB steganography on PPM (portable pixmap) images
- Implementing image edge detection to write data in the visual edges of an image to make it harder to detect hidden information
- Encryption to prevent embeded data to be read by a third party

## Getting Started

#### Before you start
Make sure that you have [GCC](https://gcc.gnu.org/) installed on your computer so that you can compile C files.

#### Getting the code and understanding it
After, clone this repository to your computer
```
git clone https://github.com/Johnson-Su/Steg-Hide.git
```
What documents you need to know:
- `cover.pgm`
  - This image will be the original copy of the image that you will write the text data into. Feel free to compare cover.pgm with steg.pgm after you run the program to see if you can spot any differences in the image!
- `steg.pgm`
  - This image should be the same as cover.pgm initially but once you run the program this image will contain the hidden text. Send or use this image when decoding to get your message!
- `text_to_hide.txt` 
  - Type or paste any text into this file! The contents of this file will be written into the pixel data of steg.c
  
#### Running the code
Go into the downloaded repository the following command in the terminal
```
cd Steg-Hide
```
after that compile  and run the main program with
```
gcc steg.c
./a.out
```
Read the instructions that your terminal prints and enjoy!

## How it works
To start it is important to know that all data in a computer can be broken down into a series of 0's and 1's. For PGM images, each pixel is respresented with an `unsigned integer` that can be between 0 (black) and 255 (white). These numbers can be made out of a combination of 8 bits 
> ex. 120 in binary is 0111 1000
</br>
This program changes the LSB (least significant bit) which for the example would be 0111 100[0] (the zero in brackets). This is because the color value shifts by so little that the human eye basically cannot detect the change. This allows the image to store about 1/8th of it's file size with hidden information</br></br>

If we wanted to write in the character 'A' (0100 0001) into the image we would need 8 pixels as each pixel can store 1 bit of hidden data.

## Credits and Acknowledgments
Credit to [Mustafa Quraish](https://github.com/mustafaquraish) for the development of imtUtils.c and for cover.pgm and steg.pgm.</br> </br>
Inspired by content covered in CSC A48 - Intro to Computer Science II, Summer 2020
