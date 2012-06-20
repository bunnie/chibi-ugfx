/*
 * console.c
 *
 *  Created on: 20 Jun 2012
 *      Author: Thomas Saunders AKA "Badger"
 */

#include "ch.h"

#include "fonts.h"
#include "glcd.h"
#include "console.h"


msg_t lcdConsoleInit(GLCDConsole *console, uint16_t x0, uint16_t y0, uint16_t x1,
		uint16_t y1, const uint8_t *font, uint8_t *buffer, uint16_t bkcolor, uint16_t color) {
	const uint8_t* ptr;
	uint16_t chi;
	uint16_t x,y;
	/* read font, get size */
	/* TODO: this is messy. improve font handling.
	 * this assumes that all characters are as wide as A */
	console->fy = font[FONT_TABLE_HEIGHT_IDX];
	chi = *(uint16_t*)(&font[FONT_TABLE_CHAR_LOOKUP_IDX + ('A'-0x20)*2]);
	ptr = font + chi;
	console->fx = *(ptr++);

	/* calculate the size of the console in characters */
	console->sx = (x1-x0)/console->fx;
	console->sy = (y1-y0)/console->fy;

	console->cx = 0;
	console->cy = 0;

	console->buf = buffer;
	console->bidx = 0;
}


/*
 * Interface implementation. The interface is write only
 */

static size_t writes(void *ip, const uint8_t *bp, size_t n) {
	(void)ip;
	return lcdWriteString(bp, n);
}

static size_t reads(void *ip, uint8_t *bp, size_t n) {
	(void)ip;
	(void)bp;
	(void)n;
	return 0;
}

static msg_t put(void *ip, uint8_t b) {
	(void)ip;
	return lcdDrawChar((char)b);
}

static msg_t get(void *ip) {
	(void)ip;
	return RDY_OK;
}

static msg_t putt(void *ip, uint8_t b, systime_t timeout) {
	(void)ip;
	(void)timeout;
	/* TODO: handle timeout */
	return lcdDrawChar((char)b);
}

static msg_t gett(void *ip, systime_t timeout) {
	(void)ip;
	(void)timeout;
	return RDY_OK;
}

static size_t writet(void *ip, const uint8_t *bp, size_t n, systime_t time) {
	(void)ip;
	(void)time;
	return lcdWriteString(bp, n);
}

static size_t readt(void *ip, uint8_t *bp, size_t n, systime_t time) {
	(void)ip;
	(void)bp;
	(void)n;
	(void)time;
	return 0;
}

static chnflags_t getflags(void *ip) {
  _chn_get_and_clear_flags_impl(ip);
}

static const struct GLCDConsoleVMT vmt = {
  writes, reads, put, get,
  putt, gett, writet, readt,
  getflags
};


