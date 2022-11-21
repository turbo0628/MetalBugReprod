Build command:
```
clang++ -Wall -std=c++17 -I./metal-cpp  -fno-objc-arc -O2  -framework Metal -framework Foundation -framework Cocoa  ./reprod.cpp -o reprod
```

You could observe the following crash on MacOS 12+:
```
Assertion failed: (false), function test_compute_pipeline, file reprod.cpp, line 46.
Compiler encountered an internal error[1]    9971 abort      ./reprod
```
