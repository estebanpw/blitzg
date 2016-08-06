


void terror(const char * err_msg);
inline void translate(const char c);
uint64_t hashOfWord(const char * word, uint32_t KSIZE);
void hashToWord(char * word, uint32_t KSIZE, uint64_t hash);
int wordcmp(const unsigned char *w1, const unsigned char *w2, int bytes_to_check);
void shift_word_left(unsigned char * b);
void shift_word_right(unsigned char * b);
void addNucleotideToWord(unsigned char * b, char strand, unsigned char c);
void showWord(const unsigned char * b, char * kmer, uint16_t WORD_LENGTH);
unsigned char getLastChar(const unsigned char * b);
unsigned char getFirstChar(const unsigned char * b);
char bitsToChar(unsigned char b);
void pmemcpy(unsigned char * d, const unsigned char * s, int bytes);
