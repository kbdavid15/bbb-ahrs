/*
 * txmsg.h
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */

#ifndef TXMSG_H_
#define TXMSG_H_

#include <stdint.h>
#include <linux/can.h>

class txmsg {
private:
	bool		is_tx_on;
public:
	txmsg();
	txmsg(can_frame, uint32_t);
	txmsg(canid_t addr1, unsigned char len, unsigned char * data, uint32_t period);

	canid_t 	arbid;	// not necessary if having the frame
	can_frame frame;
	uint32_t 	period_ms;
};



#endif /* TXMSG_H_ */
