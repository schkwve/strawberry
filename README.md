# Strawberry

Strawberry is a configurable SMTP server meant to be small, fast and reliable.

## Running

### Prerequisites

- `GNU Make`
- `GNU Autotools` (`autoconf`, `automake`, `autoreconf`)

### Building

```bash
# Generate all necessary build files
autoreconf -i

# (Optional) Create a build directory
mkdir build
cd build

# Configure and build
../configure
make

# (Optional) Install
sudo make install
```

# License

|![gplv3-only](https://www.gnu.org/graphics/gplv3-with-text-136x68.png) |![gplv3-or-later](https://www.gnu.org/graphics/gplv3-or-later.png)|
|-|-|

Strawberry is free and, more importantly, libre software, being licensed under either GPL-3.0-only or GPL-3.0-or-later, which secures the rights of end users of software incorporating Strawberry. The main implication of this license is that pieces of code taken from (even if modified) Strawberry cannot be used in non-GPL-licensed software. However, GPL-licensed code can still be combined with non-GPL code if the licenses are compatible (visit [https://gplv3.fsf.org/wiki/index.php/Compatible_licenses](https://gplv3.fsf.org/wiki/index.php/Compatible_licenses)) license and you license the resulting software combination under GPL-3.0-only or GPL-3.0-or-later (preferably both.)

**Notice:** The *GPL-3.0-only or GPL-3.0-or-later* license only applies to this software. The files you create using this software are your own original works, subject to your copyright and whatever purpose/license you choose.

Below is a copy of the legally-binding disclaimer you implicitly agree to by using this software.

> This program (Strawberry) is free software: you can redistribute it
> and/or modify it under the terms of the GNU General Public License as
> published by the Free Software Foundation, either version 3 of the
> License, or (at your opinion) any later version.
>
> This program is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
> GNU General Public License for more details.
>
> You should have received a copy of the GNU General Public License
> along with this program.  If not, see <https://www.gnu.org/licenses/>.
