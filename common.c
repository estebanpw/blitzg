#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <string.h>
#include "structs.h"

void terror(const char * err_msg){
	fprintf(stderr, "\nERR: %s\n", err_msg);
	exit(-1);
}

/* 	This function compares two arrays of unsigned chars with the same length.
 *  @param w1: first array to be compared.
 *  @param w2: second array to be compared.
 *  @param bytes_to_check: length of BOTH arrays in bytes
 *  @retun a positive number if w1>w2, a negative number if w1<w2 and zero if they are equal.
 */
 
int wordcmp(const unsigned char *w1, const unsigned char *w2, int bytes_to_check) {
	/*
	__m128i vw1 = _mm_loadl_epi64((__m128i *)w1);
	__m128i vw2 = _mm_loadl_epi64((__m128i *)w2);
	__m128i allOnes;
	uint64_t ones[2] = {0xFFFFFFFF, 0xFFFFFFFF};
	_mm_store_si128(&allOnes, *(__m128i*)&ones[0]);
	
	__m128i resMayor = _mm_and_si128 (vw1, allOnes);
	__m128i resMenor = _mm_and_si128 (vw2, allOnes);
	
	if((uint64_t *)&resMayor < (uint64_t *)&resMenor) return -1;
	if((uint64_t *)&resMayor > (uint64_t *)&resMenor) return 1;
	return 0;
	
	*/
	
	/*
	if(*(uint64_t *)&w1[0] < *(uint64_t *)&w2[0]) return -1;
	if(*(uint64_t *)&w1[0] > *(uint64_t *)&w2[0]) return 1;
	return 0;
	*/
	
	int i;
	for (i=0;i<bytes_to_check;i++) {
		if (w1[i]<w2[i]) return -1;
		if (w1[i]>w2[i]) return +1;
	}
	return 0;
	
}

/* 	This function is used to shift left bits in a unsigned char array
 *	@b: char array representing the word using compressed 2-bit characters
 */
 
void shift_word_left(unsigned char * b) {
	
	
	__m128i res;
	unsigned char * ptr;
	
	unsigned char silly_copy[16] = {b[7], b[6], b[5], b[4], b[3], b[2], b[1], b[0], 0,0,0,0,0,0,0,0};
	_mm_store_si128(&res, *(__m128i *)&silly_copy[0]);
	//Shift left 2 bits
	res =  _mm_slli_epi64(res, 2);
	
    /*
    unsigned int i;
    for (i = 0; i < BYTES_IN_WORD - 1; i++) {
        b[i] <<= 2;
        b[i] |= (b[i + 1] >> 6);
    }
    b[BYTES_IN_WORD - 1] <<= 2;
    */
    
	ptr = (unsigned char *) &res;
	
	b[0] = ptr[7];
	b[1] = ptr[6];
	b[2] = ptr[5];
	b[3] = ptr[4];
	b[4] = ptr[3];
	b[5] = ptr[2];
	b[6] = ptr[1];
	b[7] = ptr[0];
	
	
	
	
}


/* 	This function is used to shift right bits in a unsigned char array
 *	@b: char array representing the word using compressed 2-bit characters
 */
void shift_word_right(unsigned char * b) {
    int i;
    for (i = BYTES_IN_WORD - 1; i > 0; i--) {
        b[i] >>= 2;
        b[i] |= (b[i - 1] << 6);
    }
    b[i] >>= 2;
}

/*
	Returns the last character of  a correctly compressed word
*/

unsigned char getLastChar(const unsigned char * b){
	
	return b[BYTES_IN_WORD-1] & 3;
}


/*
	Returns the first char of a compressed word
*/

unsigned char getFirstChar(const unsigned char * b){
	return (b[0] & 192) >> 6;
}


/*	Adds a 2-bit compressed char to a char array (word) previously shifted
	@b:	The word where the nucleotide will be added
	@strand:	'f' if we are calculating the forward kmers or 'r' if reverse
	@c:	The char to be compressed and added
*/

void addNucleotideToWord(unsigned char * b, char strand, unsigned char c){
	switch (c) {
            case 'A': // A = 00
                if (strand == 'r') b[0] |= 192;
                break;
            case 'C': // C = 01
                if (strand == 'f') b[BYTES_IN_WORD - 1] |= 1;
                if (strand == 'r') b[0] |= 128;
                break;
            case 'G': // G = 10
                if (strand == 'f') b[BYTES_IN_WORD - 1] |= 2;
                if (strand == 'r') b[0] |= 64;
                break;
            case 'T': // T = 11
                if (strand == 'f') b[BYTES_IN_WORD - 1] |= 3;
                break;
            default : // Bad formed sequence
                break;
	}	
}

/* Translates an unsigned char word of ACTG letters compressed in 2 bits to a readable char
 *  @b: Unsigned array of chars compressed
 *  @kmer: Char array to receive readable translation
 *  @WORD_LENGTH: Length in bits of the word to translate
 */

void showWord(const unsigned char * b, char * kmer, uint16_t WORD_LENGTH) {
	char Alf[] = { 'A', 'C', 'G', 'T' };
	int i;
	int wsize = WORD_LENGTH/4;
	unsigned char c;
	for (i = 0; i < wsize; i++) {
		c = b[i];
		c = c >> 6;
		kmer[4*i] = Alf[(int) c];
		c = b[i];
		c = c << 2;
		c = c >> 6;
		kmer[4*i+1] = Alf[(int) c];
		c = b[i];
		c = c << 4;
		c = c >> 6;
		kmer[4*i+2] = Alf[(int) c];
		c = b[i];
		c = c << 6;
		c = c >> 6;
		kmer[4*i+3] = Alf[(int) c];
	}
	kmer[32]='\0';
}

/*
	Convert the last two bits of a single compressed letter to uncompressed
	@b:	A byte whose last 2 bits encode a letter
	
	Returns the corresponding char 
*/

char bitsToChar(unsigned char b){
	if((b & 3) == 3) return 'T';
	if((b & 3) == 2) return 'G';
	if((b & 3) == 1) return 'C';
	if((b & 3) == 0) return 'A';
}

/*
	I have experienced some strange memcpy behaviour
	@destination:	pointer to an unsigned char * where the bytes will be copied
	@source:	pointer to an unsigned char * from where to copy the bytes
	@bytes:	bytes to copy from to source
*/

void pmemcpy(unsigned char * d, const unsigned char * s, int bytes){
	int i;
	for(i=0;i<bytes;i++){
		d[i] = s[i];
	}
}


/* Buffered reader to read char by char
 *  @param buffer: An already allocated buffer of chars
 *  @param pos: Current position of last char read in buffer (should be initialized to READBUF+1
 *  @param read: Number of chars read at iteration
 *	@param f: File descriptor from which to read
 *	@return: The read char
 */

char buffered_fgetc(char *buffer, uint64_t *pos, uint64_t *read, FILE *f) {
    if (*pos >= READBUF) {
        *pos = 0;
        memset(buffer, 0, READBUF);
        *read = fread(buffer, 1, READBUF, f);
    }
    *pos = *pos + 1;
    return buffer[*pos-1];
}

