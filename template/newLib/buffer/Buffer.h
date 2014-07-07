/*
 * Buffer.h
 *
 *  Created on: 2014Äê7ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef BUFFER_H_
#define BUFFER_H_
#include <stm32f10x.h>

class Buffer {
public:
	Buffer(u8 *buf,int fullsize);
	virtual ~Buffer();
	void pushback(u8 data);
	void pushback(u8 *data, int length);
	u8 pop();
	void pop(u8 *data, int length);
	u8 get();
	void get(u8 *data, int length);
	u8 operator[](int posi);
	int length();
	int find(u8 value,int startp=0);
	u8* data(int posi);
	void clearFront(int posi);
private:
	u8 *buf;
	int endp;
	int startp;
	int fullsize;
};

#endif /* BUFFER_H_ */
