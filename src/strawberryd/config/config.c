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

#include <config/config.h>

struct strawberry_config config = {
	// default configuration path
	.config_file = "/etc/strawberry/strawberryd.conf",

	// Set the RAM limit to 4GB
	.max_mem = 4096
};

void config_load_config(void)
{
}

void config_set_config_path(char *filepath)
{
	config.config_file = filepath;
}

void config_set_mem_limit(int limit)
{
	config.max_mem = limit;
}
