/*
 * NRF.h
 *
 *  Created on: 2014年7月5日
 *      Author: Administrator
 */

#ifndef NRF_H_
#define NRF_H_
#include "stream/stream.h"
#include "24l01.h"
#include "postman/Postman.h"
#include "EXTIManager.h"
#include "buffer/buffer.h"
#define NRF_BUF_LEN	2048
#define NRF_RX_MODE	0
#define NRF_TX_MODE	1

class NRF: public Stream {
public:
	NRF(u8 mode = NRF_RX_MODE);
	u8 check(void); //检查24L01是否存在
	void setMode(u8 mode);
	virtual bool write(uint8_t data);
	virtual bool write(uint8_t *data, int length);
	virtual uint8_t read(void);
	virtual bool read(uint8_t *data, int length);
	virtual int available();
	virtual ~NRF();
	virtual void flush();
	void setCallback(Postman *postman_p,void(*cb)(Package *package));
	u8 id;
};

void NRF_RecvHandler(struct EXTILineType *triggeredLine);

#endif /* NRF_H_ */
