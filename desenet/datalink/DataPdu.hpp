#pragma once
/**
 * @file DataPdu.hpp
 * @date fall 2012 - 18.01.2013
 * @author Christian Muller & Romain Maffina
 */

#include <stdint.h>
#include <phy/frame.h>

/**
 * @brief Represents a DL data PDU.
 */
class DataPdu {

private:
	const char *payload;
	int payloadSize;
	bool smd, cmd;


public:

	/**
	 * @param payload pointer containing the payload to send
	 * @param size size of the payload
	 */
	DataPdu(const char *payload, int size) {
		DataPdu(payload, size, false);
	}

	/**
	 * @param payload pointer containing the payload to send
	 * @param size size of the payload
	 * @param smd boolean that indicates if the server has more data to send
	 */
	DataPdu(const char *payload, int size, bool smd)
	 : payload(payload), payloadSize(size), smd(smd), cmd(true)  {
	}

	/**
	 * @return SMD (server more data) and CMD (client more data) flags
	 */
	bool isMoreData() {
		return smd || cmd;
	}

	/**
	 * @return the size of the payload
	 */
	int getPayloadSize() {
		return payloadSize;
	}

	/**
	 * @return a pointer to the payload datas
	 */
	const char *getPayloadBytes() const {
		return payload;
	}

};
