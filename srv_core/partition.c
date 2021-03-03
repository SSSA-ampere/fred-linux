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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "partition.h"
#include "slot.h"

//---------------------------------------------------------------------------------------------

int partition_add_slot(struct partition *self, struct slot *slot)
{
	assert(self);
	assert(slot);

	if (self->slots_count < MAX_SLOTS - 1) {
		self->slots[self->slots_count++] = slot;
		return 0;
	}

	return -1;
}

int partition_search_slot(struct partition *self, struct slot **slot,
							const struct hw_task *hw_task)
{
	int slot_idx = -1;
	int rcfg = 1;

	assert(self);
	assert(hw_task);

	// For all slots in the partition
	for (int i = 0; i < self->slots_count; ++i) {
		// If the slot is free
		if (slot_is_available(self->slots[i])) {
			slot_idx = i;
			// And already contain the requested hw-task
			if (slot_match_hw_task(self->slots[i], hw_task)) {
				rcfg = 0;
				break;
			}
		}
	}

	// No slot found
	if (slot_idx < 0) {
		*slot = NULL;
	// If slot found
	} else {
		*slot = self->slots[slot_idx];
	}

	return rcfg;
}

int partiton_register_slots(struct partition *self, struct reactor *reactor)
{
	int retval;

	assert(self);
	assert(reactor);

	for (int i = 0; i < self->slots_count; ++i) {
		retval = reactor_add_event_handler(	reactor,
											slot_get_event_handler(self->slots[i]),
											REACT_NORMAL_HANDLER, REACT_NOT_OWNED);
		if (retval)
			return -1;
	}

	return 0;
}

void partiton_print(const struct partition *self, char *str, int str_size)
{
	assert(self);

	snprintf(str, str_size, "partition %d : %s containing %d slots",
				self->index, self->name, self->slots_count);
}

int partition_init(struct partition **self, const char *name, int index)
{
	*self = calloc(1, sizeof(**self));
	if (!(*self))
		return -1;

	(*self)->index = index;
	strncpy((*self)->name, name, sizeof((*self)->name) - 1);

	return 0;
}

void partition_free(struct partition *self)
{
	assert(self);

	for (int i = 0; i < MAX_SLOTS; ++i) {
		if (self->slots[i])
			event_handler_free(slot_get_event_handler(self->slots[i]));
	}

	free(self);
}

