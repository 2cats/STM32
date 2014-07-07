/*
 * Postman.cpp
 *
 *  Created on: 2014年7月5日
 *      Author: Administrator
 */

#include "postman/Postman.h"
#include "string.h"
/*
使用方法:
	1.创建 stream
	2.创建 postman
	3.不断调用 postman.receive  判断并处理 package

注意事项：
	1.数据大小要小于buf大小，buf里还要装header和checksum等东西
	2.buf分两个：[例如串口]
			stream 		--	 buf[DMA_BUFSIZE]								此buf一般保存多个package
			postman		--	 rx_buffer[Postman_BufSize]			此buf在解析package的时候使用，稍稍大于最大数据包的大小即可
	3.postman.receive(&package)中：package的data指针指向的是postman的内部buf数组，需要及时取出！
		如：AA aa; ... aa=parse(int,postman.data)  //把data数组里的值复制到了aa里，aa此时是安全的


对于NRF:
	1.NRF的接收需要中断支持（即需要配置额外的外部中断，服务函数在NRF.cpp里已给出）
	2.postman对NRF的解析，放在了NRF的接收中断里，	
		nrf.setCallback(&postman,receive_proc);//这句话就绑定了postman来处理数据，同时把处理好的数据放到receive_proc的参数里供用户调用

对于串口：
	1.串口的接收采用DMA,故串口要选择DMA模式（构造时不给出中断回调函数即可）
	2.postman对串口的解析，采用查询方式（因为DMA比中断抢先一步，中断实际上不会有反应）。用户自己决定 “手动查询” 或者 “定时器定时查询”。
		所谓查询就是不断调用  postman.receive(&package)  如果有数据，返回true的同时数据被保存在了package里
	
*/
Postman::Postman(Stream *_stream) :
		stream(_stream){
//	, buffer(buf, Postman_BufSize)
	rx_len=rx_array_inx=0;
	// TODO Auto-generated constructor stub
}

Postman::~Postman() {
	// TODO Auto-generated destructor stub
}
//bool Postman::search() {
//	int startp, i;
//	startp = buffer.find(Postman_START1); //ignore data before startfrom
//	if (startp != -1 && buffer[startp + 1] == Postman_START2
//			&& (buffer.length() - startp) >= 5) //found 2 leader code&type&size=>2+1+2(byte)=5(byte)
//					{
//		int size = ((buffer[startp + 3]) << 8) + buffer[startp + 4];
//		if (buffer.length() - startp - 5 > size) //got full package  &  checksum
//				{
//			Package package;
//			u8 checksum;
//			u8 *data;
//			package.type = buffer[startp + 2];
//			package.size = size;
//			buffer.clearFront(startp + 5);
//			data = (u8*) malloc(size);
//			package.data = data;
//			checksum = size;
//			for (i = 0; i < size; i++) {
//				*(data + i) = buffer.pop();
//				checksum ^= *(data + i);
//			}
//			if (checksum == buffer.pop()) //success
//					{
//				packages.push_back(package);
//				return true;
//			}
//		}
//
//	}
//	return false;
//}
//
//int Postman::available() {
//	dump();
//	int count = 0;
//	while (search())
//		count++;
//	return count;
//}
//Package Postman::pop() {
//
//	Package pack = packages.front();
//	packages.pop_front();
//	return pack;
//
//}
//void Postman::dump() {
//	int i;
//	int ava = stream->available();
//	for (i = 0; i < ava; i++) {
//		buffer.pushback(stream->read());
//	}
//}
//Postman_START1 Postman_START2 type size data checksum
bool Postman::receive(Package *package) {
	//start off by looking for the header bytes. If they were already found in a previous call, skip it.
	if (rx_len == 0) {
		//this size check may be redundant due to the size check below, but for now I'll leave it the way it is.
		if (stream->available() >= 5) {
			//this will block until a 0x06 is found or buffer size becomes less then 3.
			while (stream->read() != Postman_START1) {
				//This will trash any preamble junk in the serial buffer
				//but we need to make sure there is enough in the buffer to process while we trash the rest
				//if the buffer becomes too empty, we will escape and try again on the next call
				if (stream->available() < 5)
					return false;
			}
			if (stream->read() == Postman_START2) {
				rx_type = stream->read();
				rx_len = (stream->read() << 8) ;
				rx_len += stream->read();
				//make sure the binary structs on both Arduinos are the same size.
				if (rx_len < 0) {
					rx_len = 0;
					return false;
				}
			}
		}
	}

	//we get here if we already found the header bytes, the struct size matched what we know, and now we are byte aligned.
	if (rx_len != 0) {
		while (stream->available() && rx_array_inx <= rx_len) {
			rx_buffer[rx_array_inx++] = stream->read();
		}

		if (rx_len == (rx_array_inx - 1)) {
			//seem to have got whole message
			//last uint8_t is CS
			calc_CS = rx_len;
			for (int i = 0; i < rx_len; i++) {
				calc_CS ^= rx_buffer[i];
			}

			if (calc_CS == rx_buffer[rx_array_inx - 1]) {	      //CS good
				package->size=rx_len;
				package->type=rx_type;
				package->data=rx_buffer;
				//memcpy(package, rx_buffer,rx_len);
				rx_len = 0;
				rx_array_inx = 0;
				return true;
			}

			else {
				//failed checksum, need to clear this out anyway
				rx_len = 0;
				rx_array_inx = 0;
				return false;
			}

		}
	}

	return false;
}
void Postman::send(Package &package) {
	uint8_t CS = package.size;
	u8 *data;
	data = (u8*) package.data;
	stream->write(Postman_START1);
	stream->write(Postman_START2);
	stream->write(package.type);
	stream->write((package.size >> 8) & 0xff);
	stream->write(package.size & 0xff);
	for (int i = 0; i < package.size; i++) {
		CS ^= *(data + i);
		stream->write(*(data + i));
	}
	stream->write(CS);
	stream->flush();
}
void  Postman::send(void*data,int size,u8 type)
{
	Package package;
	package.data=data;
	package.size=size;
	package.type=type;
	send(package);
}
//void Postman::setCallback(u8 type, Postman_CallBack_TypeDef cb)
//{
//	cb_map.insert(std::pair<u8, Postman_CallBack_TypeDef>(type, cb));
//}
//void Postman::receive_proc()
//{
//	Package package;
//	if(receive(&package))
//	{
//		std::map<u8, Postman_CallBack_TypeDef>::iterator it = cb_map.begin();
//		while (it != cb_map.end()) {
//			if (package.type==it->first&& it->second != 0) {
//				it->second(&package);
//			}
//			it++;
//		}
//	}
//}
