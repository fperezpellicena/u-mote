#ifndef D_Lszw_H
#define D_Lszw_H

#include "GenericTypeDefs.h"

#define DEBUG		1


#define DICT_SIZE	512
#define CODE_NULL	DICT_SIZE
#define HASH_SIZE	(DICT_SIZE)

#define SLZW_ERR_DICT_IS_FULL	-1
#define SLZW_ERR_INPUT_BUF		-2
#define SLZW_ERR_WRONG_CODE		-3

typedef struct _bitbuffer
{
	UINT16 buf;	
	UINT16 n;
}
bitbuffer_t;

// LZW encoder node, represents a string
typedef struct _node_enc
{
	INT16           prev;		// prefix code
	INT16           next;		// next child code
	UINT8 ch;		// last symbol
}
node_enc_t;

// LZW decoder node, represents a string
typedef struct _node_dec
{
	INT16           prev;		// prefix code
	UINT8 ch;		// last symbol
}
node_dec_t;

// LZW encoder context
typedef struct _Slzw_enc
{
	INT16           code;				// current code
	UINT16      max;				// maximal code
	UINT16      codesize;			// number of bits in code
	bitbuffer_t   bb;				// bit-buffer struct
	UINT8 *stream;			// poINT16er to the stream object
	UINT16      lzwn;				// buffer byte counter
	UINT16      lzwm;				// buffer size (decoder only)
	node_enc_t    dict[DICT_SIZE];	// code dictionary
	INT16           hash[HASH_SIZE];	// hast table
	UINT8 buff[256];		// output code buffer
}
Slzw_enc_t;

// LZW decoder context
typedef struct _Slzw_dec
{
	INT16           code;				// current code
	UINT16      max;				// maximal code
	UINT16      codesize;			// number of bits in code
	bitbuffer_t   bb;				// bit-buffer struct
	void          *stream;			// poINT16er to the stream object
	UINT16      lzwn;				// input buffer byte counter
	UINT16      lzwm;				// input buffer size
	UINT8 *inbuff;		    // input code buffer
	node_dec_t    dict[DICT_SIZE];	// code dictionary
	UINT8 c;				// first UINT8 of the code
	UINT8 buff[DICT_SIZE];	// output string buffer
}
Slzw_dec_t;

INT16 Slzw_hash(const INT16 code, const UINT8 c);
INT16 Slzw_enc_findstr(Slzw_enc_t *const ctx, INT16 code, UINT8 c);
void Slzw_enc_init(Slzw_enc_t *ctx, void *stream);
void Slzw_dec_init(Slzw_dec_t *ctx, void *stream);

INT16  Slzw_encode  (Slzw_enc_t *ctx, UINT8 buf[], UINT16 size);
INT16  Slzw_decode  (Slzw_dec_t *ctx, UINT8 buf[], UINT16 size);

void Slzw_enc_end (Slzw_enc_t *ctx);

// Application defined stream callbacks
void     Slzw_writebuf(UINT8 *stream, UINT8 *buf, UINT16 size);
UINT16 Slzw_readbuf (UINT8 *stream, UINT8 *buf, UINT16 size);

#endif 
