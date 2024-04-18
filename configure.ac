# SPDX-License-Identifier: GPL-3.0-only or GPL-3.0-or-later
#
# Strawberry is a configurable SMTP server meant to be small, fast
# and reliable.
#
# Copyright (C) 2024 Jozef Nagy <schkwve@gmail.com>.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

AC_INIT([Strawberry], [0.1.0], [schkwve@gmail.com], [strawberry], [])

AM_SILENT_RULES([yes])

# Keep some clutter out of here
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# foreign keeps some junk out of this tree
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects foreign])

# Checks for compilers.
AC_PROG_CC

# Put configuration results in config.h
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile src/strawberryd/Makefile])
AC_OUTPUT
