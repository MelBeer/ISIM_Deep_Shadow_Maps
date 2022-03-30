//
// Created by melbeer on 29/03/2022.
//

#include "Image.h"

void Image::saveImage(const std::string& fileName)
{
    // Create file
    ofstream file(fileName);

    // Init file with basic information
    file << "P6\n" << this->w << " " << this->h << " " << 255 << "\n";

    // Add pixels
    for (int i = 0; i < this->h; i++)
    {
        for (int j = 0; j < this->w; j++)
        {
            int index = i * this->w + j;
            aiColor3D pix = this->pixels[index];

            file << char(pix.r) << char(pix.g) << char(pix.b);
        }
    }

    file.close();

    printf("Image saved");
}