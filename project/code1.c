#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char r, g, b, a;
} Color;

typedef unsigned char BYTE;

#pragma pack(push, 1)
typedef struct {
    BYTE signature[2];
    unsigned int fileSize;
    unsigned int reserved;
    unsigned int dataOffset;
} BMPFileHeader;

typedef struct {
    unsigned int size;
    int width, height;
    unsigned short planes;
    unsigned short bitCount;
    unsigned int compression;
    unsigned int imageSize;
    int xPelsPerMeter, yPelsPerMeter;
    unsigned int clrUsed;
    unsigned int clrImportant;
} BMPInfoHeader;
#pragma pack(pop)

//func1
void convertBMPToHex(const char* inputFileName, const char* outputFileName) {
    FILE *inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL) {
        fprintf(stderr, "Cannot open input file.\n");
        return;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    fread(&fileHeader, sizeof(BMPFileHeader), 1, inputFile);
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, inputFile);

    if (infoHeader.bitCount != 32) {
        fprintf(stderr, "Unsupported file format. Only 32-bit BMP files are supported.\n");
        fclose(inputFile);
        return;
    }

    int pixelCount = infoHeader.width * infoHeader.height;
    unsigned char* imageData = (unsigned char*)malloc(pixelCount * 4);

    fseek(inputFile, fileHeader.dataOffset, SEEK_SET);
    fread(imageData, 4, pixelCount, inputFile);
    fclose(inputFile);

    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Cannot open output file.\n");
        free(imageData);
        return;
    }

    for (int i = 0; i < pixelCount; i++) {
        fprintf(outputFile, "%02X%02X%02X%02X\n",
                imageData[4*i + 2], imageData[4*i + 1], imageData[4*i], imageData[4*i + 3]);
    }

    fclose(outputFile);
    free(imageData);
}

//func2
void processHexToReducedHex(const char* inputHexFile, const char* outputHexFile) {
    FILE* inputFile = fopen(inputHexFile, "r");
    FILE* outputFile = fopen(outputHexFile, "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Color pixel;
    int count = 960 * 640;

    for (int i = 0; i < count; i++) {
        if (fscanf(inputFile, "%2hhx%2hhx%2hhx%2hhx", &pixel.r, &pixel.g, &pixel.b, &pixel.a) != 4) {
            printf("Error reading pixel data.\n");
            return;
        }

        unsigned char newPixel = 0;
        newPixel |= (pixel.r & 0xE0);
        newPixel |= (pixel.g & 0xE0) >> 3;
        newPixel |= (pixel.b & 0xC0) >> 6;

        fprintf(outputFile, "%02X\n", newPixel);
    }

    fclose(inputFile);
    fclose(outputFile);
}

//func3
void createColorPalette(FILE* outputFile) {
    // Color palette
    for (int i = 0; i < 256; i++) {
        BYTE red = (i & 0xE0);      // Extract red (3 bits)
        BYTE green = (i & 0x1C) << 3;  // Extract green (3 bits), then shift to align
        BYTE blue = (i & 0x03) << 6;   // Extract blue (2 bits), then shift to align

        BYTE color[4] = { blue, green, red, 0 };
        fwrite(color, 4, 1, outputFile);
    }
}

//func4
void createBMP(const char* inputHexFile, const char* outputBmpFile) {
    FILE* inputFile = fopen(inputHexFile, "r");
    FILE* outputFile = fopen(outputBmpFile, "wb");

    if (!inputFile || !outputFile) {
        printf("Error opening files.\n");
        return;
    }

    int width = 960;
    int height = 640;
    int rowPadded = (width + 3) & (~3);
    unsigned char* image = malloc(rowPadded * height);

    for (int i = 0; i < width * height; i++) {
        unsigned char pixel;
        fscanf(inputFile, "%2hhx", &pixel);
        image[i] = pixel;
    }
    fclose(inputFile);

    BMPFileHeader fileHeader = { {'B', 'M'}, 0, 0, 54 + 256 * 4 };
    BMPInfoHeader infoHeader = { 40, width, height, 1, 8, 0, rowPadded * height, 0, 0, 256, 256 };

    fileHeader.fileSize = fileHeader.dataOffset + infoHeader.imageSize;

    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, outputFile);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, outputFile);

    createColorPalette(outputFile); // Call to create a correct color palette

    for (int i = 0; i < height; i++) {
        fwrite(image + i * width, 1, width, outputFile);
        for (int j = 0; j < rowPadded - width; j++) {
            BYTE zero = 0;
            fwrite(&zero, 1, 1, outputFile);
        }
    }

    free(image);
    fclose(outputFile);
}

//func5
void createGrayscaleBMPFromHex(const char* inputHexFile, const char* outputBmpFile, int width, int height) {
    FILE* inputFile = fopen(inputHexFile, "r");
    if (!inputFile) {
        printf("Error opening file.\n");
        return;
    }

    BYTE* image = malloc(width * height);  // Allocate memory for 8-bit image
    BYTE* palette = malloc(256 * 4);  // Allocate memory for the grayscale palette

    // Create a grayscale palette
    for (int i = 0; i < 256; i++) {
        palette[4 * i] = i;       // Blue channel
        palette[4 * i + 1] = i;   // Green channel
        palette[4 * i + 2] = i;   // Red channel
        palette[4 * i + 3] = 0;   // Reserved (unused)
    }

    // Read the HEX file and convert each pixel to 8-bit grayscale
    for (int i = 0; i < width * height; i++) {
        unsigned char r, g, b, a;
        fscanf(inputFile, "%2hhx%2hhx%2hhx%2hhx", &b, &g, &r, &a);
        BYTE average = (r + g + b) / 3;
        image[i] = average;
    }
    fclose(inputFile);

    FILE* outputFile = fopen(outputBmpFile, "wb");
    if (!outputFile) {
        free(image);
        free(palette);
        printf("Error opening output file.\n");
        return;
    }

    int rowPadded = (width + 3) & (~3);
    int imageSize = rowPadded * height;

    BMPFileHeader fileHeader = { {'B', 'M'}, 54 + 256 * 4 + imageSize, 0, 54 + 256 * 4 };
    BMPInfoHeader infoHeader = { 40, width, height, 1, 8, 0, imageSize, 0, 0, 256, 0 };

    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, outputFile);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, outputFile);
    fwrite(palette, 4, 256, outputFile);  // Write the grayscale palette

    // Write the image data, padding each row as needed
    for (int i = 0; i < height; i++) {
        fwrite(image + i * width, 1, width, outputFile);
        BYTE padding[3] = { 0 };
        fwrite(padding, 1, rowPadded - width, outputFile);  // Padding to align rows
    }

    fclose(outputFile);
    free(image);
    free(palette);
}


//func6
void convertToBinaryFromHex(const char* inputHexFile, const char* outputBmpFile, int width, int height) {
    FILE* inputFile = fopen(inputHexFile, "r");
    if (!inputFile) {
        printf("Error opening file.\n");
        return;
    }

    int pixels = width * height;
    int rowPadded = (width / 8 + 3) & (~3);
    BYTE* image = malloc(rowPadded * height);
    memset(image, 0, rowPadded * height);

    int byteIndex = 0, bitIndex = 7;
    for (int i = 0; i < pixels; i++) {
        unsigned char r, g, b, a;
        fscanf(inputFile, "%2hhx%2hhx%2hhx%2hhx", &b, &g, &r, &a);
        BYTE gray = (r + g + b) / 3;
        if (gray > 127) {
            image[byteIndex] |= (1 << bitIndex);
        }
        bitIndex--;
        if (bitIndex < 0) {
            bitIndex = 7;
            byteIndex++;
        }
    }
    fclose(inputFile);

    FILE* outputFile = fopen(outputBmpFile, "wb");
    if (!outputFile) {
        free(image);
        printf("Error opening output file.\n");
        return;
    }

    BMPFileHeader fileHeader = { {'B', 'M'}, 0, 0, 62 };
    BMPInfoHeader infoHeader = { 40, width, height, 1, 1, 0, rowPadded * height, 0, 0, 2, 0 };
    fileHeader.fileSize = fileHeader.dataOffset + infoHeader.imageSize;

    BYTE palette[8] = { 0, 0, 0, 0, 255, 255, 255, 0 }; // Palette for black and white

    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, outputFile);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, outputFile);
    fwrite(palette, sizeof(palette), 1, outputFile); // Write palette
    fwrite(image, rowPadded, height, outputFile); // Write image data

    fclose(outputFile);
    free(image);
}

int main() {
    const char* inputFileName = "test.png";
    const char* tempOutputHex = "temp_output.hex";
    const char* reducedOutputHex = "reduced_output.hex";
    const char* finalOutputBmp = "final_image.bmp";
    const char* grayscaleFileName = "grayscale8.bmp";
    const char* binaryFileName = "binary1.bmp";
    const int width = 960, height = 640;

    convertBMPToHex(inputFileName, tempOutputHex);
    processHexToReducedHex(tempOutputHex, reducedOutputHex);
    createBMP(reducedOutputHex, finalOutputBmp);
    createGrayscaleBMPFromHex(tempOutputHex, grayscaleFileName, width, height);
    convertToBinaryFromHex(tempOutputHex, binaryFileName, width, height);
    return 0;
}
