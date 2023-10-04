/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: vid_image.cpp,v 1.6 2005/08/09 07:00:37 ecco Exp $   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// Implementation Private Includes
/////////////////////////////////////////////////////////////////////////////
#include "defs.h"

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Definitions, Enumeration and Typedefs
/////////////////////////////////////////////////////////////////////////////
#pragma pack(1)
struct image_tgaheader_t
{
    byte  identsize;          // size of ID field that follows 18 byte header (0 usually)
    byte  colourmaptype;      // type of colour map 0=none, 1=has palette
    byte  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    short colourmapstart;     // first colour map entry in palette
    short colourmaplength;    // number of colours in palette
    byte  colourmapbits;      // number of bits per palette entry 15,16,24,32

    short xstart;             // image x origin
    short ystart;             // image y origin
    short width;              // image width in pixels
    short height;             // image height in pixels
    byte  bits;               // image bits per pixel 8,16,24,32
    byte  descriptor;         // image descriptor bits (vh flip bits)
    
    // pixel data follows header
    
};
#pragma pack()

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void image_RGB2BGRA(vid_image_t* img)
{
	byte *charTemp;
	
	// Calculate the byte size of the old image and the new one
	int oldsize = img->width * img->height * img->bpp / 8;
	int newsize = img->width * img->height * 4;

	// Copy old image data outside the struct
    charTemp = (byte*)malloc(oldsize);
	memcpy(charTemp, img->data, oldsize);
	free(img->data);

	// Create memory space for new image in the struct
	img->data = (byte*) malloc( newsize );

	// Convert all pixels from RGB to BGRA
	int i; int pos = 0;

    for( i = 0; i < oldsize; i += (img->bpp / 8) )
	{ 
		img->data[pos] = charTemp[i+2];
		img->data[pos+1] = charTemp[i+1];
		img->data[pos+2] = charTemp[i];

		//if (img->data[pos] == 0 && img->data[pos+1] == 0 && img->data[pos+2] == 0)
			//img->data[pos+3] = 0;
		//else
			img->data[pos+3] = 255;

		pos += 4;
    }

	free(charTemp);

	img->bpp = 32;
}

void image_LoadBitmap( char *pFileName, vid_image_t *pImage )
{
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
	int i;

    if( (pFile = fopen(pFileName, "rb") ) == NULL )
	{
		console_Print("ERROR: getBitmapImageData - %s not found\n",pFileName);
		return;
	}

    // Seek forward to width and height info
    fseek( pFile, 18, SEEK_CUR );

    if( (i = fread(&pImage->width, 4, 1, pFile) ) != 1 )
		console_Print("ERROR: getBitmapImageData - Couldn't read width from %s.\n", pFileName);

    if( (i = fread(&pImage->height, 4, 1, pFile) ) != 1 )
		console_Print("ERROR: getBitmapImageData - Couldn't read height from %s.\n", pFileName);

    if( (fread(&nNumPlanes, 2, 1, pFile) ) != 1 )
		console_Print("ERROR: getBitmapImageData - Couldn't read plane count from %s.\n", pFileName);
	
    if( nNumPlanes != 1 )
		console_Print( "ERROR: getBitmapImageData - Plane count from %s is not 1: %u\n", pFileName, nNumPlanes );

    if( (i = fread(&nNumBPP, 2, 1, pFile)) != 1 )
		console_Print( "ERROR: getBitmapImageData - Couldn't read BPP from %s.\n", pFileName );
	
	pImage->bpp = nNumBPP;
	
    if( nNumBPP != 24 )
		console_Print( "ERROR: getBitmapImageData - BPP from %s is not 24: %u\n", pFileName, nNumBPP );

    // Seek forward to image data
    fseek( pFile, 24, SEEK_CUR );

	// Calculate the image's total size in bytes. Note how we multiply the 
	// result of (width * height) by 3. This is becuase a 24 bit color BMP 
	// file will give you 3 bytes per pixel.
    int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = (byte*) malloc( nTotalImagesize );
	
    if( (i = fread(pImage->data, nTotalImagesize, 1, pFile) ) != 1 )
		console_Print("ERROR: getBitmapImageData - Couldn't read image data from %s.\n", pFileName);

    //
	// Finally, rearrange BGR to RGB
	//

	image_BGR2RGB(pImage);
}

bool image_LoadTarga(char *pFileName, vid_image_t *pImage)
{
	image_tgaheader_t header;
    FILE *pFile = NULL;
	int i;

	pFile = fopen(pFileName, "rb");

    if (pFile == NULL)
	{
		return false;
	}

    // Seek forward to width and height info
    //fseek( pFile, 12, SEEK_CUR );

    if( (i = fread(&header, sizeof(header), 1, pFile) ) != 1 )
		console_Print("ERROR: getTargaImageData - Couldn't read header from %s.\n", pFileName);

	pImage->height = header.height;
	pImage->width = header.width;

	if (header.bits != 32 && header.bits != 24)
		console_Print( "ERROR: getTargaImageData - BPP from %s is not 24 or 32: %u\n", pFileName, header.bits );
	
	pImage->bpp = header.bits;

	// Calculate the image's total size in bytes. Note how we multiply the 
	// result of (width * height) by 3. This is becuase a 24 bit color BMP 
	// file will give you 3 bytes per pixel.
    int nTotalImagesize = (pImage->width * pImage->height) * 4;

    pImage->data = (byte*) malloc( nTotalImagesize );
	
    if( (i = fread(pImage->data, nTotalImagesize, 1, pFile) ) != 1 )
		printf("ERROR: getTargaImageData - Couldn't read image data from %s.\n", pFileName);

	fclose(pFile);

    //
	// Finally, rearrange BGR to RGB
	//.
	image_BGR2RGB(pImage);

	return true;
}

void image_BGR2RGB(vid_image_t *img)
{
	char charTemp;
	int i;
	int nTotalImagesize = img->width * img->height * img->bpp / 8;

    for( i = 0; i < nTotalImagesize; i += img->bpp / 8 )
	{ 
		charTemp = img->data[i];
		img->data[i] = img->data[i+2];
		img->data[i+2] = charTemp;
    }
}

void image_SaveTarga(char *filename, vid_image_t *img)
{
    image_tgaheader_t th;
	FILE *pFile = NULL;

	pFile = fopen(filename, "wb");
	
	th.identsize = 0;
	th.colourmaptype = 0;
	th.imagetype = 2;
	th.colourmapstart = 0;
	th.colourmaplength = 0;
	th.colourmapbits = 0;
	th.xstart = 0;
	th.ystart = 0;
	th.width = img->width;
	th.height = img->height;
	th.bits = img->bpp;
	th.descriptor = 15;
	//th.descriptor = 68;
	//th.descriptor = 47;

	int datasize = img->width * img->height * (img->bpp / 8);

	fwrite(&th, sizeof(th), 1, pFile);
	//
	fwrite(img->data, datasize, 1, pFile);

	fclose(pFile);

	console_Print("image: %s saved successfully.", filename);
}

void image_Resize(vid_image_t *img, int width, int height)
{
	int paddingx, paddingy;
	char *olddata;
	int pixelsize = img->bpp / 8;

	olddata = (char *)malloc(img->width * img->height * pixelsize);

	memcpy(olddata, img->data, img->width * img->height * pixelsize);

	paddingx = width - img->width;
	paddingy = height - img->height;

	free(img->data);
	img->data = (byte *)malloc(width * height * pixelsize);
	memset(img->data, 0, width * height * pixelsize);

	int i;
	int ooffset = 0;
	int offset = 0;
	
	offset = (paddingy / 2) * width * pixelsize;
	offset += (paddingx / 2) * pixelsize;

	for (i = 0; i < img->height; i++)
	{
		memcpy(img->data+offset, olddata+ooffset, img->width * pixelsize);
		offset += width * pixelsize;
		ooffset += img->width * pixelsize;
	}

	img->width = width;
	img->height = height;

	free(olddata);

	console_Print("PadXY %d %d", paddingx, paddingy);
}

void image_Crop(vid_image_t *imgin, vid_image_t *imgout, int x, int y, int width, int height)
{
	imgout->width = width;
	imgout->height = height;
	imgout->bpp = imgin->bpp;
	
	int pixelsize = imgin->bpp / 8;
	int pos = 0;
	int linesize = imgin->width * pixelsize;

	imgout->data = (byte *)malloc(width * height * pixelsize);

	int i, j;
	for (i = y; i < y + height; i++)
	{
		for (j = x; j < x + width; j++)
		{
			//console_Print("%d, %d", j, i);
			int curinpos = (linesize * i) + (j * pixelsize);

			imgout->data[pos] = imgin->data[curinpos];
			imgout->data[pos+1] = imgin->data[curinpos+1];
			imgout->data[pos+2] = imgin->data[curinpos+2];
			imgout->data[pos+3] = imgin->data[curinpos+3];
			pos += pixelsize;
		}
	}

	free(imgin->data);
}
