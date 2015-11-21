/*
 * txmsg.h
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */

#ifndef TXMSG_H_
#define TXMSG_H_

#include <linux/can.h>
#include <stdint.h>

class txmsg {
private:
public:
	can_frame frame;
	uint32_t  period_ms;

	txmsg();
	txmsg(can_frame, uint32_t);
	txmsg(canid_t addr1, unsigned char len, unsigned char * data, uint32_t period);
};



#endif /* TXMSG_H_ */
