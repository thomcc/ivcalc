# IVCalc

This is an old project of mine, added in an effort to put more of my code on github (again).  I doubt it will be any use to anyone.

It's an interval math evaluator and calculator.

It's written in C++11, and (optionally) uses LLVM-JIT to compile the expressions.  It will also do derivation for single argument functions.  It has a few bugs, but I'm unlikely to work on it again. In particular: YMMV in getting this to build.

It also performs automatic derivation of the supplied expressions, however it chokes on functions calling other functions, if the called functions take multiple arguments.

Syntax is more or less

```
funcname(a, r, g, u, m, e, n, t, s) = u*s + e/(a + r + g * s)
```

which does more or less what it looks like. There's also an s-expression based input mode, which can be used with the -sexp flag. There are several other command line options, which may be viewed with the -help flag (-h should work as well).


## License

It's unrecommended that you use this for anything, but MIT.

Copyright (c) 2012 Thom Chiovoloni

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
