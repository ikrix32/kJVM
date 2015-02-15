/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Small bitmap library.
 *
 *
 * - Compiler:           GNU GCC for AVR32
 * - Supported devices:  All AVR32AP devices with a SIDSA LCD controller
 * - AppNote:            AVR32114 Using the AVR32 LCD controller
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 * $Name$
 * $Revision: 595M $
 * $RCSfile$
 * $Date: 2008-05-07 15:58:16 +0200 (Wed, 07 May 2008) $
 *****************************************************************************/

/* Copyright (c) 2007, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "lcdc.h"
#include "usart.h"
#include <string.h>


/*! \brief File header of a bitmap file. 14 bytes long, little endian representation and packed. 
 * See bitmap specification for more details.
 */
typedef struct bm_file_header_s {
	/*! \brief Magic number used to identify the bitmap file. 
	 * Typical values for these 2 bytes are 0x42 0x4D (ASCII code points for B and M)
	 */
	unsigned short bfType;
	/*! \brief Size of the bitmap file */
	unsigned int bfSize;
	/*! \brief Reserved. May be used by some software */
	unsigned int bfReserved;
	/*! \brief Offset in bytes where the bitmap data can be found */
	unsigned int bfOffBits;
} __attribute__((__packed__)) bm_file_header_t ;

/**
 * \brief Bitmap file information header. Starts after the file header and is 40 bytes long, in little endian and packed.
 */ 
typedef struct bm_info_header_s {
	/*! \brief Size of the information header */
 	unsigned int biSize;
 	/*! \brief Width of the bitmap in pixels */
	int biWidth;
	/*! \brief Height of the bitmap in pixels 
	 * 	If this value is positiv -> "bottom-up"-bitmap (picture data starts with the bottom 
	 * line and ends with the top line)
     * If this value is negativ ->"top-down"-Bitmap (picture data starts with the top line 
     * and ends with the bottom line)
	 */
	int biHeight; 
	/*! \brief Number of color planes being used. Not often used. */
	unsigned short 	biPlanes;
	/*! \brief Number of bits per pixel, which is the color depth of the image.
	 *  Typical values are 1, 4, 8, 16, 24 and 32.
	 */
	unsigned short 	biBitCount;
	/*! \brief Define the compression method being used
	 *  0 (BI_RGB): No compression
	 *  1 (BI_RLE8): 
	 *  2 (BI_RLE4): 
	 *  3 (BI_BITFIELDS):
	 */
	unsigned int 	biCompression;

	/*! \brief Image size. This is the size of the raw bitmap data. */
	unsigned int 	biSizeImage;
	/*! \brief Horizontale resolution */
	int 	biXPelsPerMeter;
	/*! \brief Vertical resolution */
	int 	biYPelsPerMeter;
 	unsigned int 	biClrUsed; 	
	unsigned int 	biClrImportant; 	
}__attribute__((__packed__)) bm_info_header_t;

/**
 * \brief Swaps a block of data to the other endian representation.
 *
 * \param pblock Pointer to the first byte of the data block
 * \param size Size of the data bock in bytes
 */
void swap_endian_blk(unsigned char *pblock, int size)
{
        unsigned char *pend = &pblock[size - 1];
        unsigned char temp;
        while(pblock < pend)
        {
                temp = *pblock;
                *pblock = *pend;
                *pend = temp;
                pblock++;
                pend--;
        }
}
/*! \brief Magnifies a bitmap file by two and prints it into the virtual frame buffer
 *  Size of the image 320*240; Virtual frame buffer 640*480
 *  \param lcdc_conf Pointer to the LCD controller setup structure
 *  \param bm_info_header Pointer to the bit map file information structure
 *  \param data_start Pointer to the beginning of the bit map file
 */
void fill_virtual_frame_buffer_bm(lcdc_conf_t *lcdc_conf, bm_info_header_t *bm_info_header,unsigned char *data_start)
{
	unsigned int k,l;
	unsigned char * framePtr, *bm;
	
	/* set framebuffer start pointer to non-cached address */
	framePtr = (unsigned char *) (lcdc_conf->dmabaddr1 | 0xA0000000);
	bm = data_start;

	bm = bm + bm_info_header->biBitCount * bm_info_header->biWidth * (bm_info_header->biHeight - 1) / 8;
	for (l = 0 ; l < bm_info_header->biHeight; l++){
		for (k = 0; k < bm_info_header->biWidth; k++) {
			framePtr++; /* empty */
			*framePtr++ = *( bm + 0 );        /* blue */
			*framePtr++ = *( bm + 1 );    /* green */
			*framePtr++ = *( bm + 2 );    /* red */
			framePtr++; 
			*framePtr++ = *( bm + 0 );
			*framePtr++ = *( bm + 1 );
			*framePtr++ = *( bm + 2 );	
			bm += 3;
		}
		bm = bm - bm_info_header->biBitCount * bm_info_header->biWidth / 8;
		for (k = 0; k < bm_info_header->biWidth; k++) {
			framePtr++;
			*framePtr++ = *( bm + 0 );
			*framePtr++ = *( bm + 1 );
			*framePtr++ = *( bm + 2 );
			framePtr++;
			*framePtr++ = *( bm + 0 );
			*framePtr++ = *( bm + 1 );
			*framePtr++ = *( bm + 2 );	
			bm += 3;
		}
		bm = bm - bm_info_header->biBitCount *2* bm_info_header->biWidth / 8;
	}
}
/**
 * \brief Fill the frame buffer with a bitmap
 * \param lcdc_conf Pointer to the LCD controller configuration
 * \param bm_info_header Pointer to the bitmap file information (with correct endianess) 
 * \param data_start Pointer to the beginning of the bitmap data
 */





// pixelsize 32
void myfill(lcdc_conf_t *lcdc_conf)
{
	unsigned int k;
	unsigned int * framePtr;
	unsigned int * pframe;
	unsigned int red,green,blue;
	framePtr = (unsigned int *) (lcdc_conf->dmabaddr1 | 0xA0000000);
		pframe = (unsigned int *) framePtr;	
				for (k = 0; k < 240; k++) {
					
					blue = k+15;
					green = k+15 ;
					red = k+15;
					*(pframe+k*640+k) = ((blue << 16) & 0xff0000) | ((green << 8) & 0x00ff00) | (red & 0x000000fF);
				}

}



void fill_frame_buffer_bm(lcdc_conf_t *lcdc_conf, bm_info_header_t *bm_info_header,unsigned char *data_start)
{
	
	unsigned int k,l;
	unsigned char * framePtr, *bm;
	unsigned short * pframe;
	unsigned short red,green,blue;
	
	framePtr = (unsigned char *) (lcdc_conf->dmabaddr1 | 0xA0000000);
	bm = data_start;
	if(lcdc_conf->pixelsize == BPP_24){
	if(bm_info_header->biHeight < 0){
		for (l = 0; l < bm_info_header->biHeight; l++){
			for (k = 0; k < bm_info_header->biWidth; k++) {
				*framePtr++ = *( bm + 0 );        /* blue */
				*framePtr++ = *( bm + 1 );    /* green */
				*framePtr++ = *( bm + 2 );    /* red */
				bm += 3;
			}
		if(bm_info_header->biWidth < lcdc_conf->xres)
			framePtr += (lcdc_conf->xres - bm_info_header->biWidth) * 3; 

		}
	}
	else {
		bm = bm + bm_info_header->biBitCount * bm_info_header->biWidth * (bm_info_header->biHeight - 1) / 8;
		for (l = 0 ; l < bm_info_header->biHeight; l++){
			for (k = 0; k < bm_info_header->biWidth; k++) {
				*framePtr++ = *(bm + 0);
				*framePtr++ = *(bm + 1);
				*framePtr++ = *(bm + 2);
				bm += 3;
			}
			bm = bm - bm_info_header->biBitCount *2* bm_info_header->biWidth / 8;
			if(bm_info_header->biWidth < lcdc_conf->xres)
				framePtr += (lcdc_conf->xres - bm_info_header->biWidth) * 3; 

		}
	}
	}
	if(lcdc_conf->pixelsize == BPP_16){
		pframe = (unsigned short *) framePtr;
		if(bm_info_header->biHeight > 0) {
			
			bm = bm + bm_info_header->biBitCount * bm_info_header->biWidth * (bm_info_header->biHeight - 1) / 8;
			for (l = 0 ; l < bm_info_header->biHeight; l++){
				for (k = 0; k < bm_info_header->biWidth; k++) {
					
					blue = *(bm + 2) >> 3;
					green = *(bm +1) >> 3;
					red = *bm >> 3;
					*pframe++ = ((blue << 10) & 0x7C00) | ((green << 5) & 0x03E0) | (red & 0x001F);
					bm += 3;
				}
				bm = bm - bm_info_header->biBitCount *2* bm_info_header->biWidth / 8;
				if(bm_info_header->biWidth < lcdc_conf->xres)
					pframe += (lcdc_conf->xres - bm_info_header->biWidth); 

			}
		}
	}
}
/*! \brief Dispays the content of a bitmap file in the virtual frame buffer
 *  \param lcdc_conf Pointer to the LCD controller configuration
 *  \param file_start Pointer to the beginning of the file
 */
int display_virtual_bm(lcdc_conf_t *lcdc_conf, void * file_start){
	
	bm_file_header_t bm_file_header;
	bm_info_header_t bm_info_header;
	unsigned char * data_start;
	
	/* get file header and header information */
	memcpy(&bm_file_header, file_start, sizeof(bm_file_header_t));
	memcpy(&bm_info_header, file_start + 14, sizeof(bm_info_header_t));
	
	/* correct endianess */
	swap_endian_blk((unsigned char *) &bm_file_header.bfType, 2);
	swap_endian_blk((unsigned char *) &bm_file_header.bfSize, 4);
	swap_endian_blk((unsigned char *) &bm_file_header.bfOffBits, 4);
	swap_endian_blk((unsigned char *) &bm_info_header.biWidth, 4);
	swap_endian_blk((unsigned char *) &bm_info_header.biHeight, 4);
	swap_endian_blk((unsigned char *) &bm_info_header.biBitCount, 2);
	swap_endian_blk((unsigned char *) &bm_info_header.biCompression, 4);
	
	
	data_start = file_start + bm_file_header.bfOffBits;	
	fill_virtual_frame_buffer_bm(lcdc_conf, &bm_info_header, data_start);
	
	return 0;		
}
/*! \brief Dispays the content of a bitmap file in the frame buffer
 *  \param lcdc_conf Pointer to the LCD controller configuration
 *  \param file_start Pointer to the beginning of the file
 */
int display_bm(lcdc_conf_t *lcdc_conf, void * file_start){
	
	bm_file_header_t bm_file_header;
	bm_info_header_t bm_info_header;
	unsigned char * data_start;
	
	/* get file header and header information */
	memcpy(&bm_file_header, file_start, sizeof(bm_file_header_t));
	memcpy(&bm_info_header, file_start + 14, sizeof(bm_info_header_t));
	
	/* correct endianess */
	swap_endian_blk((unsigned char *) &bm_file_header.bfType, 2);
	swap_endian_blk((unsigned char *) &bm_file_header.bfSize, 4);
	swap_endian_blk((unsigned char *) &bm_file_header.bfOffBits, 4);
	swap_endian_blk((unsigned char *) &bm_info_header.biWidth, 4);
	swap_endian_blk((unsigned char *) &bm_info_header.biHeight, 4);
	swap_endian_blk((unsigned char *) &bm_info_header.biBitCount, 2);
	swap_endian_blk((unsigned char *) &bm_info_header.biCompression, 4);
	
	
	data_start = file_start + bm_file_header.bfOffBits;	
	fill_frame_buffer_bm(lcdc_conf, &bm_info_header, data_start);
	
	return 0;		
}
