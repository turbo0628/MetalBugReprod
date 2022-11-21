Build command:
```
clang++ -Wall -std=c++17  -fno-objc-arc -O2 -I./metal-cpp/ -framework Metal -framework Foundation  ./reprod.cpp -o reprod
```

You could observe the following crash on MacOS 12+:
```
Assertion failed: (false), function test_compute_pipeline, file reprod.cpp, line 38.
```
