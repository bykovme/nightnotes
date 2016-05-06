discount-cpp-wrapper
====================

A C++11 wrapper for [discount] markdown library.

## Minimalistic usage example

You need to link your code with the static libraries `libmarkdown` and
`libmarkdown++` and include `discount-wrapper.hpp` header file.

The following example code shows how to compile a Markdown string into HTML:

```C++
#include <stdexcept>
#include <discount-wrapper.hpp>

int main()
{
	Markdown mkd("# Hello Markdown!");
	try {
		// throws a std::runtime_error if discount fails to parse
		// the markdown string, ie. mkd_string() returns a nullptr.
		mkd.compile();
		// html <- "<h1>Hello Markdown!</h1>"
		std::string html = mkd.html();
	}
	catch(const std::runtime_error& e) {
		return 1;
	}

	return 0;
}
```

## Build and install

A GNU Makefile and Arch Linux [PKGBUILD] are provided.

You must have the static [libmarkdown and mkdio.h][discount] file in your
compiler search path.

By default the installation prefix is `/usr/local`, you can customize it with
DESTDIR and PREFIX as in the following example:

```bash
make
make PREFIX=/usr install
```

will install `discount-wrapper.hpp` in `/usr/include` and `libmarkdown++.a`
in `/usr/lib`.

[discount]: https://github.com/Orc/discount "discount GitHub repository"
[PKGBUILD]: https://github.com/soratobukuroneko/abs/blob/master/discount-cpp-wrapper-static-git/PKGBUILD
