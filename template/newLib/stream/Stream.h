#ifndef STREAMBASE_H
#define STREAMBASE_H
#include <stm32f10x.h>

class Stream {

public:
	Stream(){}
	virtual ~Stream(){}
	virtual bool write(uint8_t data)=0;
	virtual bool write(uint8_t *data, int length)=0;
	virtual uint8_t read(void)=0;
	virtual bool read(uint8_t *data, int length)=0;
	virtual int available()=0;
	virtual void flush(){}
protected:
};
#endif
