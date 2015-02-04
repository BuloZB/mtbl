/*
 * Copyright (c) 2012 by Farsight Security, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mtbl-private.h"

struct mtbl_iter {
	mtbl_iter_next_func	iter_next;
	mtbl_iter_next_func	iter_prev;
	mtbl_iter_free_func	iter_free;
	void			*clos;
};

struct mtbl_iter *
mtbl_iter_init(mtbl_iter_next_func iter_next,
		   mtbl_iter_next_func iter_prev,
	       mtbl_iter_free_func iter_free,
	       void *clos)
{
	assert(iter_next != NULL);
	struct mtbl_iter *it = my_calloc(1, sizeof(*it));
	it->iter_next = iter_next;
	it->iter_prev = iter_prev;
	it->iter_free = iter_free;
	it->clos = clos;
	return (it);
}

void
mtbl_iter_destroy(struct mtbl_iter **it)
{
	if (*it) {
		if ((*it)->iter_free != NULL)
			(*it)->iter_free((*it)->clos);
		free(*it);
		*it = NULL;
	}
}

mtbl_res
mtbl_iter_next(struct mtbl_iter *it,
	       const uint8_t **key, size_t *len_key,
	       const uint8_t **val, size_t *len_val)
{
	if (it == NULL)
		return (mtbl_res_failure);
	return (it->iter_next(it->clos, key, len_key, val, len_val));
}

mtbl_res
mtbl_iter_prev(struct mtbl_iter *it,
	       const uint8_t **key, size_t *len_key,
	       const uint8_t **val, size_t *len_val)
{
	if (it == NULL)
		return (mtbl_res_failure);
	return (it->iter_prev(it->clos, key, len_key, val, len_val));
}
