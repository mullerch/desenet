/*
 * Node.hpp
 *
 *  Created on: Dec 10, 2012
 *      Author: desem
 */

#ifndef DATAPDU_HPP_
#define DATAPDU_HPP_

#include <stdint.h>
#include <phy/frame.h>

class DataPdu {

private:
	uint32_t payload;
	int payloadSize;
	bool smd, cmd;


public:
	DataPdu(uint32_t payload, int size, bool smd) {
		this->payload = payload;
		this->payloadSize = size;
		this->smd = smd;
		this->cmd = true;
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
};

#endif /* DATAPDU_HPP_ */
