#pragma once
#include <vector>
#include <fstream>

#include <GL/GL.h>
#include <GL/GLU.h>

using namespace std;


typedef union PixelInfo {
    uint32_t Col;
    struct {
        uint8_t R, G, B, A;
    };
} *PPixelInfo;

class Model {
private:
    vector<std::uint8_t> Pixels;
    bool ImgComp;
    uint32_t width, height, size, BitsPerPixel;

public:
    Model(const char* FilePath);
    vector<std::uint8_t> GetPixels() { return this->Pixels; }
    uint32_t GetWidth() const { return this->width; }
    uint32_t GetHeight() const { return this->height; }
    bool HasAlphaChannel() { return BitsPerPixel == 32; }
};

Model::Model(const char* FilePath) {
    fstream hFile(FilePath, ios::in | ios::binary);
    if (!hFile.is_open()) { throw std::invalid_argument("File Not Found."); }

    uint8_t Header[18] = { 0 };
    vector<uint8_t> ImageData;
    static uint8_t DeCompressed[12] = { 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
    static uint8_t IsCompressed[12] = { 0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    hFile.read(reinterpret_cast<char*>(&Header), sizeof(Header));

    if (!memcmp(DeCompressed, &Header, sizeof(DeCompressed))) {
        BitsPerPixel = Header[16];
        width = Header[13] * 256 + Header[12];
        height = Header[15] * 256 + Header[14];
        size = ((width * BitsPerPixel + 31) / 32) * 4 * height;

        if ((BitsPerPixel != 24) && (BitsPerPixel != 32)) {
            hFile.close();
            throw invalid_argument("Invalid File Format. Required: 24 or 32 Bit Image.");
        }

        ImageData.resize(size);
        ImgComp = false;
        hFile.read(reinterpret_cast<char*>(ImageData.data()), size);
    }
    else if (!memcmp(IsCompressed, &Header, sizeof(IsCompressed))) {
        BitsPerPixel = Header[16];
        width = Header[13] * 256 + Header[12];
        height = Header[15] * 256 + Header[14];
        size = ((width * BitsPerPixel + 31) / 32) * 4 * height;

        if ((BitsPerPixel != 24) && (BitsPerPixel != 32)) {
            hFile.close();
            throw invalid_argument("Invalid File Format. Required: 24 or 32 Bit Image.");
        }

        PixelInfo Pixel = { 0 };
        int CurrentByte = 0;
        size_t CurrentPixel = 0;
        ImgComp = true;
        uint8_t ChunkHeader = { 0 };
        int BytesPerPixel = (BitsPerPixel / 8);
        ImageData.resize(width * height * sizeof(PixelInfo));

        do {
            hFile.read(reinterpret_cast<char*>(&ChunkHeader), sizeof(ChunkHeader));

            if (ChunkHeader < 128) {
                ++ChunkHeader;
                for (int I = 0; I < ChunkHeader; ++I, ++CurrentPixel) {
                    hFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

                    ImageData[CurrentByte++] = Pixel.R;
                    ImageData[CurrentByte++] = Pixel.B;
                    ImageData[CurrentByte++] = Pixel.G;
                    if (BitsPerPixel > 24) ImageData[CurrentByte++] = Pixel.A;
                }
            }
            else {
                ChunkHeader -= 127;
                hFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

                for (int I = 0; I < ChunkHeader; ++I, ++CurrentPixel) {
                    ImageData[CurrentByte++] = Pixel.R;
                    ImageData[CurrentByte++] = Pixel.B;
                    ImageData[CurrentByte++] = Pixel.G;
                    if (BitsPerPixel > 24) ImageData[CurrentByte++] = Pixel.A;
                }
            }
        } while (CurrentPixel < (width * height));
    }
    else {
        hFile.close();
        throw invalid_argument("Invalid File Format. Required: 24 or 32 Bit TGA File.");
    }

    hFile.close();
    this->Pixels = ImageData;
}
