#pragma once

#include <stdint.h>
#include <phy/frame.h>

class DataPdu {

private:
	const char *payload;
	int payloadSize;
	bool smd, cmd;


public:

	DataPdu(const char *payload, int size) {
		DataPdu(payload, size, false);
	}

	DataPdu(const char *payload, int size, bool smd)
	 : payload(payload), payloadSize(size), smd(smd), cmd(true)  {
	}

//	DataPdu(Frame frame) {
//
//		DataPdu dataPdu = (DataPdu) *frame.payloadBytes();
//
//		this->payload = dataPdu.payload;
//		this->payload_size = dataPdu.payload_size;
//		this->smd = dataPdu.smd;
//		this->cmd = dataPdu.cmd;
//	}

	bool isMoreData() {
		return smd && cmd;
	}

	int getPayloadSize() {
		return payloadSize;
	}

	const char * getPayloadBytes() const {
		return payload;
	}

};
