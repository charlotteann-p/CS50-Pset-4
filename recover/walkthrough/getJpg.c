#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int main (int numOfArguments, string argValues[])
{
  if (numOfArguments == 2)
  {
    char* file = argValues[1];
    FILE* card = fopen(file, "r");
    if (!card)
    {
      printf("No file or nothing in file.");
      return 1;
    }
    // define buffer
    unsigned char buffer[512];
    // keep count
    int imgcount = 0;
    while (fread(buffer, 512, 1, card) == 1)
    {
      if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
      // is this a jpg
      {
        imgcount++;
      }
    }
    printf("Total number of jpg files: %i\n", imgcount);
    fclose(card);
    return 0;
  }
  else
  {
    printf("Usage: ./getJpg filename");
    return 1;
  }
}