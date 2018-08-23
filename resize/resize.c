// Copies a BMP file - in progress

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile.bmp outfile.bmp\n");
        return 1;
    }

    // remember filenames
    int factor = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    if (factor > 100 || factor < 1) {
        printf("Factor must be between 1 and 100.");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER, also make variable for the new?
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER, also make variable for the new?
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    BITMAPFILEHEADER bfoutput = bf;
    BITMAPINFOHEADER bioutput = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // here, it would be a good place to determine width, height, padding, file size, ec/
    int outwidth = bioutput.biWidth * factor;
    int absheight = abs(bioutput.biHeight);
    int outheight = absheight * factor;

    printf("Original height: %i, Original width: %i\n", bi.biHeight, bi.biWidth);

    printf("New height: %i, New width: %i\n", outheight, outwidth);

    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outpadding = (4 - (outwidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bioutput.biSizeImage = ((sizeof(RGBTRIPLE) * outwidth) + outpadding) * outheight; // used to be absolute value of outheight
    bfoutput.bfSize = bioutput.biSizeImage + sizeof(bfoutput) + sizeof(bioutput);

    printf("Old image size: %i, Old file size: %i\nNew image size: %i, New file size: %i\n", bi.biSizeImage, bf.bfSize, bioutput.biSizeImage, bfoutput.bfSize);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfoutput, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bioutput, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines --- can make variables for input and output padding??
    // int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // int outpadding = (4 - (outwidth * sizeof(RGBTRIPLE)) % 4) % 4;




   // for (int h = 0; h <= factor; h++) // this is for printing each line factor times
   // {

    // iterate over infile's scanlines
    for (int i = 0; i < absheight; i++) // added *factor
    {
        // iterate over pixels in scanline -- in this loop would be a good place to
        // print rows
        for (int j = 0; j < bi.biWidth; j++) // added *factor
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr); // I PUT THIS BELOW IN THE FOR LOOP but now its out here

            for (int k = 0; k < factor; k++) // this is being temporarily replaced by *factor in the other loops
            {
              // read RGB triple from infile
              // fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                for (int l = 0; l < factor; l++) /// this is a newly added for loop
                {
                // write RGB triple to outfile
                 fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
              printf("rows: %i cols:%i print:%i \n", i, j, k);



            }

          // skip over padding, if any
          fseek(inptr, padding, SEEK_CUR);

          // add padding to outfile
          for (int k = 0; k < outpadding; k++)
          {
              fputc(0x00, outptr);
          }


        }
        // // skip over padding, if any
        // fseek(inptr, padding, SEEK_CUR);

        // // add padding to outfile
        // for (int k = 0; k < outpadding; k++)
        // {
        //     fputc(0x00, outptr);
        // }
      /// here could be an ok place to move skipping the padding?
    }


   // } /// this is for the h loop


    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    printf("Program completed.\n");

    // success
    return 0;
}
