#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structs.h"

/*
	Prints an error to the stderr file handler and exists the program
	@err_msg the error itself
*/

void terror(const char * err_msg){
	fprintf(stderr, "\nERR: %s\n", err_msg);
	exit(-1);
}

/*
	Translates from ACGT to 0,1,2,3 
	@c the character to translate
	
	Returns the numerical value
*/

inline uint16_t translate(const char c){
	switch(c){
		case 'A': return 0;
		case 'C': return 1;
		case 'G': return 2;
		case 'T': return 3;
	}
}

uint64_t quick_pow4(uint64_t n){
	static uint64_t pow4[33]={1L, 4L, 16L, 64L, 256L, 1024L, 4096L, 16384L, 65536L, 
	262144L, 1048576L, 4194304L, 16777216L, 67108864L, 268435456L, 1073741824L, 4294967296L, 
	17179869184L, 68719476736L, 274877906944L, 1099511627776L, 4398046511104L, 17592186044416L, 
	70368744177664L, 281474976710656L, 1125899906842624L, 4503599627370496L, 18014398509481984L, 
	72057594037927936L, 288230376151711744L, 1152921504606846976L, 4611686018427387904L};
	return pow4[n];
}

uint64_t quick_pow4byLetter(uint64_t n, const char c){
	static uint64_t pow4_G[33]={2*1L, 2*4L, 2*16L, 2*64L, 2*256L, 2*1024L, 2*4096L, 2*16384L, 2*65536L, 
	(uint64_t)2*262144L, (uint64_t)2*1048576L,(uint64_t)2*4194304L, (uint64_t)2*16777216L, (uint64_t)2*67108864L, (uint64_t)2*268435456L, (uint64_t)2*1073741824L, (uint64_t)2*4294967296L, 
	(uint64_t)2*17179869184L, (uint64_t)2*68719476736L, (uint64_t)2*274877906944L, (uint64_t)2*1099511627776L, (uint64_t)2*4398046511104L, (uint64_t)2*17592186044416L, 
	(uint64_t)2*70368744177664L, (uint64_t)2*281474976710656L, (uint64_t)2*1125899906842624L, (uint64_t)2*4503599627370496L, (uint64_t)2*18014398509481984L, 
	(uint64_t)2*72057594037927936L, (uint64_t) 2*288230376151711744L, (uint64_t) 2*1152921504606846976L, (uint64_t) 2*4611686018427387904L};
	
	static uint64_t pow4_T[33]={3*1, 3*4, 3*16, 3*64, 3*256, 3*1024, 3*4096, 3*16384, 3*65536, 
	(uint64_t)3*262144, 3*1048576, (uint64_t)3*4194304, (uint64_t)3*16777216, (uint64_t)3*67108864, (uint64_t)3*268435456, (uint64_t)3*1073741824, (uint64_t)3*4294967296, 
	(uint64_t)3*17179869184, (uint64_t)3*68719476736, (uint64_t)3*274877906944, (uint64_t)3*1099511627776, (uint64_t)3*4398046511104, (uint64_t)3*17592186044416, 
	(uint64_t)3*70368744177664, (uint64_t)3*281474976710656, (uint64_t)3*1125899906842624, (uint64_t)3*4503599627370496, (uint64_t)3*18014398509481984, 
	(uint64_t)3*72057594037927936, (uint64_t) 3*288230376151711744, (uint64_t) 3*1152921504606846976, (uint64_t) 3*4611686018427387904};
	
	if(c == 'A') return 0;
	if(c == 'C') return quick_pow4(n);
	if(c == 'G') return pow4_G[n];
	if(c == 'T') return pow4_T[n];
}

/*
	Converts a string of nucleotides to its hash value
	@word:	The word composed of nucleotides of length KSIZE
	@KSIZE:	The length of the word
	
	Returns the hash value of the word
*/

uint64_t hashOfWord(const char * word, uint32_t k){
	uint64_t value = 0, jIdx;
	for(jIdx=0;jIdx<k;jIdx++){
		//value += (translate(word[jIdx]) * (uint64_t) pow(4, k-(jIdx+1)));
		//value += (translate(word[jIdx]) * (uint64_t) quick_pow4(k-(jIdx+1)));
		value += quick_pow4byLetter(k-(jIdx+1), word[jIdx]);
	}
	return value;
}

/*
	Returns the word for a given hash value
	@word:	The word that will contain the string of nucleotides obtained from the hash
	@KSIZE:	The length of the word
	@hash:	The computed hash of the word
*/
void hashToWord(char * word, uint32_t k, uint64_t hash){
	uint64_t i = 0, j;
	char alph[4] = "ACGT";
	
	for(j=k-1; j > 0; j--){
		//word[i] = alph[ ((uint64_t) hash/(uint64_t)(pow(4, j))) % 4];
		word[i] = alph[ ((uint64_t) hash/(uint64_t)(quick_pow4(j))) % 4];
		i++;
	}
	word[k-1] = alph[hash%4];
	word[k] = '\0';
}

/*
	Compares two hashes of words
	@h1:	The hash of the first word to compare
	@h2:	The hash of the second word to compare
	
	Returns -1,0,1 depending whether the h1<h2, h1 == h2 or h1>h2
*/

inline int hashcmp(uint64_t h1, uint64_t h2){
	if(h1 == h2) return 0;
	if(h1 < h2) return -1;
	if(h1 > h2) return 1;
}

/* 	This function compares two arrays of unsigned chars with the same length.
 *  @param w1: first array to be compared.
 *  @param w2: second array to be compared.
 *  @param bytes_to_check: length of BOTH arrays in bytes
 *  @retun a positive number if w1>w2, a negative number if w1<w2 and zero if they are equal.
 */
 
int wordcmp(const unsigned char *w1, const unsigned char *w2, int bytes_to_check) {
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
    unsigned int i;
    for (i = 0; i < BYTES_IN_WORD - 1; i++) {
        b[i] <<= 2;
        b[i] |= (b[i + 1] >> 6);
    }
    b[BYTES_IN_WORD - 1] <<= 2;
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
