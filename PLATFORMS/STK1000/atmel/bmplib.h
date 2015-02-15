#ifndef BMPLIB_H_
#define BMPLIB_H_

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


void swap_endian_blk(unsigned char *pblock, int size);
void fill_frame_buffer_bm(bm_info_header_t *bm_info_header,unsigned char *data_start);
int display_bm(void * file_start);

#endif /*BMPLIB_H_*/

