
void terror(const char * err_msg);

int wordcmp(const unsigned char *w1, const unsigned char *w2, int n);

void shift_word_left(unsigned char * b);

void shift_word_right(unsigned char * b);

void addNucleotideToWord(unsigned char * b, char strand, unsigned char c);

void showWord(const char * b, char * kmer, uint16_t WORD_LENGTH);

unsigned char getLastChar(const unsigned char * b);

char bitsToChar(unsigned char b);
