/* OpenSprinkler Unified (AVR/RPI/BBB/LINUX) Firmware
 * Copyright (C) 2015 by Ray Wang (ray@opensprinkler.com)
 *
 * Utility functions
 * Feb 2015 @ OpenSprinkler.com
 *
 * This file is part of the OpenSprinkler library
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "utils.h"
#include "OpenSprinkler.h"
extern OpenSprinkler os;

#include <FS.h>


void write_to_file(const char *fn, const char *data, ulong size, ulong pos, bool trunc) {
	File f;
	if(trunc) {
		f = SPIFFS.open(fn, "w");
	} else {
		f = SPIFFS.open(fn, "r+");
		if(!f) f = SPIFFS.open(fn, "w");
	}		 
	if(!f) return;
	if(pos) f.seek(pos, SeekSet);
	if(size==0) {
		f.write((byte*)" ", 1);  // hack to circumvent SPIFFS bug involving writing empty file
	} else {
		f.write((byte*)data, size);
	}
	f.close();
}

void read_from_file(const char *fn, char *data, ulong maxsize, ulong pos) {
	File f = SPIFFS.open(fn, "r");
	if(!f) {
		data[0]=0;
		return;  // return with empty string
	}
	if(pos)  f.seek(pos, SeekSet);
	int len = f.read((byte*)data, maxsize);
	if(len>0) data[len]=0;
	if(len==1 && data[0]==' ') data[0] = 0;  // hack to circumvent SPIFFS bug involving writing empty file
	data[maxsize-1]=0;
	f.close();
	return;
}

void remove_file(const char *fn) {
	if(!SPIFFS.exists(fn)) return;
	SPIFFS.remove(fn);
}

bool file_exists(const char *fn) {
	return SPIFFS.exists(fn);
}

// file functions
void file_read_block(const char *fn, void *dst, ulong pos, ulong len) {
	// do not use File.readBytes or readBytesUntil because it's very slow  
	File f = SPIFFS.open(fn, "r");
	if(f) {
		f.seek(pos, SeekSet);
		f.read((byte*)dst, len);
		f.close();
	}
}

void file_write_block(const char *fn, const void *src, ulong pos, ulong len) {
	File f = SPIFFS.open(fn, "r+");
	if(!f) f = SPIFFS.open(fn, "w");
	if(f) {
		f.seek(pos, SeekSet);
		f.write((byte*)src, len);
		f.close();
	}
}

void file_copy_block(const char *fn, ulong from, ulong to, ulong len, void *tmp) {
	// assume tmp buffer is provided and is larger than len
	// todo future: if tmp buffer is not provided, do byte-to-byte copy
	if(tmp==NULL) { return; }

	File f = SPIFFS.open(fn, "r+");
	if(!f) return;
	f.seek(from, SeekSet);
	f.read((byte*)tmp, len);
	f.seek(to, SeekSet);
	f.write((byte*)tmp, len);
	f.close();
}

// compare a block of content
byte file_cmp_block(const char *fn, const char *buf, ulong pos) {
	char c;

	File f = SPIFFS.open(fn, "r");
	if(f) {
		f.seek(pos, SeekSet);
		char c = f.read();
		while(*buf && (c==*buf)) {
			buf++;
			c=f.read();
		}
		f.close();
		return (*buf==c)?0:1;
	}
	return 1;
}

byte file_read_byte(const char *fn, ulong pos) {
	byte v = 0;
	file_read_block(fn, &v, pos, 1);
	return v;
}

void file_write_byte(const char *fn, ulong pos, byte v) {
	file_write_block(fn, &v, pos, 1);
}

// copy n-character string from program memory with ending 0
void strncpy_P0(char* dest, const char* src, int n) {
	byte i;
	for(i=0;i<n;i++) {
		*dest=pgm_read_byte(src++);
		dest++;
	}
	*dest=0;
}

// resolve water time
/* special values:
 * 65534: sunrise to sunset duration
 * 65535: sunset to sunrise duration
 */
ulong water_time_resolve(uint16_t v) {
	if(v==65534) {
		return (os.nvdata.sunset_time-os.nvdata.sunrise_time) * 60L;
	} else if(v==65535) {
		return (os.nvdata.sunrise_time+1440-os.nvdata.sunset_time) * 60L;
	} else	{
		return v;
	}
}

// encode a 16-bit signed water time (-600 to 600)
// to unsigned byte (0 to 240)
byte water_time_encode_signed(int16_t i) {
	i=(i>600)?600:i;
	i=(i<-600)?-600:i;
	return (i+600)/5;
}

// decode a 8-bit unsigned byte (0 to 240)
// to a 16-bit signed water time (-600 to 600)
int16_t water_time_decode_signed(byte i) {
	i=(i>240)?240:i;
	return ((int16_t)i-120)*5;
}


/** Convert a single hex digit character to its integer value */
static unsigned char h2int(char c) {
		if (c >= '0' && c <='9'){
				return((unsigned char)c - '0');
		}
		if (c >= 'a' && c <='f'){
				return((unsigned char)c - 'a' + 10);
		}
		if (c >= 'A' && c <='F'){
				return((unsigned char)c - 'A' + 10);
		}
		return(0);
}

/** Decode a url string e.g "hello%20joe" or "hello+joe" becomes "hello joe" */
void urlDecode (char *urlbuf) {
	if(!urlbuf) return;
	char c;
	char *dst = urlbuf;
	while ((c = *urlbuf) != 0) {
		if (c == '+') c = ' ';
		if (c == '%') {
			c = *++urlbuf;
			c = (h2int(c) << 4) | h2int(*++urlbuf);
		}
		*dst++ = c;
		urlbuf++;
	}
	*dst = '\0';
}

void peel_http_header(char* buffer) { // remove the HTTP header
	uint16_t i=0;
	bool eol=true;
	while(i<ETHER_BUFFER_SIZE) {
		char c = buffer[i];
		if(c==0)	return;
		if(c=='\n' && eol) {
			// copy
			i++;
			int j=0;
			while(i<ETHER_BUFFER_SIZE) {
				buffer[j]=buffer[i];
				if(buffer[j]==0)	break;
				i++;
				j++;
			}
			return;
		}
		if(c=='\n') {
			eol=true;
		} else if (c!='\r') {
			eol=false;
		}
		i++;
	}
}
