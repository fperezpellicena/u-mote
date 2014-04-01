#include "slzw.h"
#include <stdio.h>
#include <string.h>

void Slzw_writebuf(UINT8 *stream, UINT8 *buf, UINT16 size) {
    UINT16 i;
    for (i = 0; i < size; ++i) {
        stream[i] = buf[i];
    }
}

UINT16 Slzw_readbuf(UINT8 *stream, UINT8 *buf, UINT16 size) {
    UINT16 i;
    for (i = 0; i < size; i++) {
        stream[i] = buf[i];
    }
    return size;
}

/******************************************************************************
 **  Slzw_enc_writebits
 **  --------------------------------------------------------------------------
 **  Write bits INT16o bit-buffer.
 **  The number of bits should not exceed 24.
 **
 **  Arguments:
 **      ctx     - poINT16er to LZW context;
 **      bits    - bits to write;
 **      nbits   - number of bits to write, 0-24;
 **
 **  Return: -
 ******************************************************************************/
static void Slzw_enc_writeCode(Slzw_enc_t * const ctx, UINT16 code, UINT16 nbits) {
    // shift old bits to the left, add new to the right
    ctx->bb.buf = (UINT16) ((ctx->bb.buf << nbits) | (code & (((UINT16) 1 << nbits) - 1)));

    nbits = (UINT16) (nbits + ctx->bb.n);

    // flush whole bytes
    while (nbits >= 8) {
        nbits = (UINT16) (nbits - 8);
        ctx->buff[ctx->lzwn++] = (UINT8) (ctx->bb.buf >> nbits);

        if (ctx->lzwn == sizeof (ctx->buff)) {
            ctx->lzwn = 0;
            Slzw_writebuf(ctx->stream, (UINT8*) ctx->buff, sizeof (ctx->buff));
        }
    }

    ctx->bb.n = nbits;
}

/******************************************************************************
 **  Slzw_hash
 **  --------------------------------------------------------------------------
 **  Hash function is used for searching of <prefix>+<symbol> combination
 **  in the hash table.
 **
 **  Arguments:
 **      code - prefix code;
 **      c    - symbol;
 **
 **  Return: Hash code
 ******************************************************************************/
INT16 Slzw_hash(const INT16 code, const UINT8 c) {
    return (code ^ ((INT16) c << 6)) & (HASH_SIZE - 1);
}

/******************************************************************************
 **  Slzw_enc_init
 **  --------------------------------------------------------------------------
 **  Initializes LZW encoder context.
 **
 **  Arguments:
 **      ctx     - LZW context;
 **      stream  - PoINT16er to Input/Output stream object;
 **
 **  Return: -
 ******************************************************************************/
void Slzw_enc_init(Slzw_enc_t *ctx, void *stream) {
    UINT16 i;
    UINT16 j;

    ctx->code = CODE_NULL; // non-existent code
    ctx->max = 255;
    ctx->codesize = 8;
    ctx->stream = stream;
    // clear hash table
    for (j = 0; j < HASH_SIZE; j++) {
        ctx->hash[j] = CODE_NULL;
    }

    for (i = 0; i < 256; i++) {
        INT16 hash = Slzw_hash(CODE_NULL, (UINT8) i);

        ctx->dict[i].prev = CODE_NULL;
        ctx->dict[i].next = ctx->hash[hash];
        ctx->dict[i].ch = (UINT8) i;
        ctx->hash[hash] = (UINT8) i;
    }
}

/******************************************************************************
 **  Slzw_enc_reset
 **  --------------------------------------------------------------------------
 **  Reset LZW encoder context. Used when the dictionary overflows.
 **  Code size set to 8 bit.
 **
 **  Arguments:
 **      ctx     - LZW encoder context;
 **
 **  Return: -
 ******************************************************************************/
static void Slzw_enc_reset(Slzw_enc_t * const ctx) {
    UINT8 i;
    UINT16 j;

    ctx->max = 255;
    ctx->codesize = 8;

    for (j = 0; j < HASH_SIZE; j++)
        ctx->hash[j] = CODE_NULL;

    for (i = 0; i < 256; i++) {
        INT16 hash = Slzw_hash(CODE_NULL, i);

        ctx->dict[i].next = ctx->hash[hash];
        ctx->hash[hash] = i;
    }
}

/******************************************************************************
 **  Slzw_enc_findstr
 **  --------------------------------------------------------------------------
 **  Searches a string in LZW dictionaly. It is used only in encoder.
 **  Fast search is performed by using hash table.
 **  Full search is performed by using embedded linked lists.
 **
 **  Arguments:
 **      ctx  - LZW context;
 **      code - code for the string beginning (already in dictionary);
 **      c    - last symbol;
 **
 **  Return: code representing the string or CODE_NULL.
 ******************************************************************************/
INT16 Slzw_enc_findstr(Slzw_enc_t * const ctx, INT16 code, UINT8 c) {
    INT16 nc;

    // hash search
    for (nc = ctx->hash[Slzw_hash(code, c)]; nc != CODE_NULL; nc = ctx->dict[nc].next) {
        if (ctx->dict[nc].prev == code && ctx->dict[nc].ch == c) {
            break;
        }
    }

    return nc;
}

/******************************************************************************
 **  Slzw_enc_addstr
 **  --------------------------------------------------------------------------
 **  Adds string to the LZW dictionaly.
 **
 **  Arguments:
 **      ctx  - LZW context;
 **      code - code for the string beginning (already in dictionary);
 **      c    - last symbol;
 **
 **  Return: code representing the string or CODE_NULL if dictionary is full.
 ******************************************************************************/
static INT16 Slzw_enc_addstr(Slzw_enc_t * const ctx, INT16 code, UINT8 c) {
    INT16 hash;

    if (++ctx->max == CODE_NULL)
        return CODE_NULL;

    hash = Slzw_hash(code, c);

    // add new code
    ctx->dict[ctx->max].prev = code;
    ctx->dict[ctx->max].next = ctx->hash[hash];
    ctx->dict[ctx->max].ch = c;
    // add the new code INT16o hash table
    ctx->hash[hash] = (INT16) ctx->max;

    return (INT16) ctx->max;
}

/******************************************************************************
 **  Slzw_encode
 **  --------------------------------------------------------------------------
 **  Encode buffer by LZW algorithm. The output data is written by application
 **  specific callback to the application defined stream inside this function.
 **
 **  Arguments:
 **      ctx  - LZW encoder context;
 **      buf  - input byte buffer;
 **      size - size of the buffer;
 **
 **  Return: Number of processed bytes.
 ******************************************************************************/
INT16 Slzw_encode(Slzw_enc_t *ctx, UINT8 buf[], UINT16 size) {
    UINT16 i;
    if (!size) return 0;

    for (i = 0; i < size; i++) {
        UINT8 c = buf[i];
        INT16 nc = Slzw_enc_findstr(ctx, ctx->code, c);
        // printf("nc: %u\n", nc);
        // printf("nc is null?: %u\n", nc == CODE_NULL);
        if (nc == CODE_NULL) {
            // the string was not found - write <prefix>
            Slzw_enc_writeCode(ctx, (UINT16) ctx->code, ctx->codesize);

            // increase the code size (number of bits) if needed
            if (ctx->max + 1 == (1 << ctx->codesize))
                ctx->codesize++;

            // add <prefix>+<current symbol> to the dictionary
            if (Slzw_enc_addstr(ctx, ctx->code, c) == CODE_NULL) {
                // dictionary is full - reset encoder
                Slzw_enc_reset(ctx);
            }

            ctx->code = c;
        } else {
            ctx->code = nc;
        }
    }
    return (INT16) size;
}

/******************************************************************************
 **  Slzw_enc_end
 **  --------------------------------------------------------------------------
 **  Finish LZW encoding process. As output data is written INT16o output stream
 **  via bit-buffer it can contain unsaved data. This function flushes
 **  bit-buffer and padds last byte with zero bits.
 **
 **  Arguments:
 **      ctx  - LZW encoder context;
 **
 **  Return: -
 ******************************************************************************/
void Slzw_enc_end(Slzw_enc_t *ctx) {
    // write last code
    Slzw_enc_writeCode(ctx, (UINT16) ctx->code, ctx->codesize);
    // flush bits in the bit-buffer
    if (ctx->bb.n) {
        Slzw_enc_writeCode(ctx, (UINT16) 0, (UINT16) (8 - ctx->bb.n));
    }
    Slzw_writebuf(ctx->stream, (UINT8*) ctx->buff, ctx->lzwn);
}
