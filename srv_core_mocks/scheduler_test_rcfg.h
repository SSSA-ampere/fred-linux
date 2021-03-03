/*
 * Fred for Linux. Experimental support.
 *
 * Copyright (C) 2018-2021, Marco Pagani, ReTiS Lab.
 * <marco.pag(at)outlook.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#ifndef SCHEDULER_TEST_RCFG_H_
#define SCHEDULER_TEST_RCFG_H_

#include <sys/queue.h>

#include "../parameters.h"
#include "../srv_core/accel_req.h"
#include "../srv_core/devcfg.h"
#include "../srv_core/scheduler.h"

//---------------------------------------------------------------------------------------------

struct scheduler_test_rcfg {
	// ------------------------//
	struct scheduler scheduler;
	// ------------------------//

	// Partitions queues heads
	struct accel_req_queue part_queues_heads[MAX_PARTITIONS];

	// Reconfiguration device queue
	// Partition queue head
	struct accel_req_queue fri_queue_head;

	int serving;

	// Reconfiguration device
	struct devcfg *devcfg;
};


//---------------------------------------------------------------------------------------------

int sched_test_rcfg_init(struct scheduler **self, struct devcfg *devcfg);

//---------------------------------------------------------------------------------------------

#endif /* SCHEDULER_TEST_RCFG_H_ */
