/*
 * Postman.h
 *
 *  Created on: 2014Äê7ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef POSTMAN_H_
#define POSTMAN_H_
#include "Stream/Stream.h"
#include "buffer/Buffer.h"
#include <list>
#include <map>
#include <stdlib.h>
#define Postman_BufSize		2048
#define Postman_START1		0x06
#define Postman_START2		0x85
#define Package_Type_Any	0xfe

#define detail(x)		&x,sizeof(x)
#define parse(T,px)	(*(T*)(px))
struct Package {
	u8 type;
	uint16_t size;
	void* data;
};
typedef void (*Postman_CallBack_TypeDef)(Package*package) ;
class Postman {
public:
	Postman(Stream *_stream);
	void send(Package& package);
	void send(void*data,int size,u8 type);
//	int available();
//	Package pop();
	virtual ~Postman();
//	std::list<Package>packages;
	bool receive(Package *package);
//	void receive_proc();
//	void setCallback(u8 type, Postman_CallBack_TypeDef);
private:
	Stream *stream;
//	bool search();
//	Buffer buffer;
//	void dump();
//	u8 buf[Postman_BufSize];
	u8 rx_type;
	int rx_len;
	u8 rx_buffer[Postman_BufSize];
	int rx_array_inx;
	uint8_t calc_CS;
//	std::map<u8, Postman_CallBack_TypeDef> cb_map;
};

#endif /* POSTMAN_H_ */
