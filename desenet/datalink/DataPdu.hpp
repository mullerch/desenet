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

	bool isMoreData() {
		return smd || cmd;
	}

	int getPayloadSize() {
		return payloadSize;
	}

	const char *getPayloadBytes() const {
		return payload;
	}

};
