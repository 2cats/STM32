/*
 * Buffer.cpp
 *
 *  Created on: 2014Äê7ÔÂ5ÈÕ
 *      Author: Administrator
 */
 
 #include "buffer/buffer.h"

Buffer::Buffer(u8 *_buf,int _fullsize):buf(_buf),fullsize(_fullsize) {
	// TODO Auto-generated constructor stub
	endp=0;
	startp=0;
}

Buffer::~Buffer() {
	// TODO Auto-generated destructor stub
}

void Buffer::pushback(u8 data)
{
	buf[endp%fullsize]=data;
	endp++;
}
void Buffer::pushback(u8 *data, int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		pushback(*(data+i));
	}
}
u8 Buffer::get()
{
	return buf[startp%fullsize];
}
u8 Buffer::pop()
{
	u8 value=get();
	startp++;
	return value;
}
void Buffer::get(u8 *data, int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		*(data+i)=buf[(startp+i)%fullsize];
	}
}
void Buffer::pop(u8 *data, int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		*(data+i)=pop();
	}
}
u8 Buffer::operator[](int posi)
{
	return buf[(startp+posi)%fullsize];
}
int Buffer::length()
{
	return endp-startp;
}
int Buffer::find(u8 value,int startp)
{
	int i;
	for(i=startp;i<length();i++)
	{
		if(value==(*this)[i])
		{
			return i;
		}
	}
	return -1;
}
u8* Buffer::data(int posi)
{
	return buf+(startp+posi)%fullsize;
}

void Buffer::clearFront(int posi)
{
	startp+=posi;
}
