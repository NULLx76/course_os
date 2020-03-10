![course_os test suite](https://github.com/rellermeyer/course_os/workflows/course_os%20test%20suite/badge.svg)

# Course OS

This is a 'toy' kernel created by students from multiple universities. 
Currently it is mainly developed by students of the TU Delft. 
It is targeted to run on the raspberry pi zero, b+ and 2. 

# Building

## Toolchain
To build the c toolchain for arm, and qemu version 4.20, run
```
make requirements
```     
from the root project directory. We build qemu from source as this gives us better support
for attaching debuggers. Any up to date version of `qemu-system-arm` should work and if you already have this installed 
through your package manager, just running 
```
make toolchain
```
should be sufficient.

## Running

After you built the toolchain, the kernel can be ran with 
```
make run
``` 
from the root project directory, or from the `kernel` directory.

## Running tests

To test the entire kernel, run 

```
make test
```
from the root project directory, or from the `kernel` directory.


## Debugging

To debug the kernel, you have to perform two steps. First you have to build and start the kernel with
```
make debug
```

from the `kernel` directory. This prepares qemu so it waits for a debugger to be attached.

now, if you have `clion` you can run the supplied run configuration called `debug` which attaches a debugger, loads the sourcemap and runs the kernel. Now you can create breakpoints from within clion.

If however you don't have `clion`, or want to use gdb from a terminal, one can run the following command:
```bash
gdb -ex "target remote localhost:1234" -ex "symbol-file kernel/build/kernel.sym"
```
  
# Editors

## Opening the project in clion

To open this project in clion, you can simply create a new `c` project with the supplied `CMakeLists.txt` file. This file can *not* be used to actually run the kernel but it does give clion the right instructions to make code completion etc. work. 

# Copyright

Copyright (c) 2015, <OWNER>
 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

