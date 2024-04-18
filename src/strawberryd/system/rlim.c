/* SPDX-License-Identifier: GPL-3.0-only or GPL-3.0-or-later */
/*
 * Strawberry is a configurable SMTP server meant to be small, fast
 * and reliable.
 *
 * Copyright (C) 2024 Jozef Nagy <schkwve@gmail.com>.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <sys/time.h>
#include <sys/resource.h>

#include <system/rlim.h>

void limit_memory_usage(int limit)
{
	struct rlimit rlimit;
	rlim_t new_limit = (rlim_t)limit;

	getrlimit(RLIMIT_AS, &rlimit);
	if (new_limit > rlimit.rlim_max) {
		// @todo: log this
		// specified soft limit is larger than the hard limit, so we lower
		// the soft limit to the maximum possible value
		new_limit = rlimit.rlim_max;
	}

	rlimit.rlim_cur = new_limit;
	setrlimit(RLIMIT_AS, &rlimit);
}
