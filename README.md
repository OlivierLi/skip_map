## skip_map

skip_map is a sorted associative container that is backed by a skip list. It
aims to perfectly comply with the definition of `std::map` and to provide similar
performance.

## Code Example
```c++
skip_map<int, std::string> my_map;
my_map.insert({0,""});
my_map[0] = "zero";
my_map[1] = "one";
my_map.erase(1);
```

## Motivation

While this is a serious endeavor it is meant as a learning exercise.

## Installation

This is a header only library.

## Tests

The test executable uses GoogleTest and can be compiled using the provided 
CMakeLists file.

## License

The MIT License (MIT)
Copyright (c) 2016 Olivier Li

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in the 
Software without restriction, including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.