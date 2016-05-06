#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include "discount-wrapper.hpp"

int main()
{
	Markdown mkd;

	std::cout << "Non-exhaustive testing of discount wrapper ..." << std::endl;
	std::cout << "Setting AUTOLINK & TOC flags ... ";
	mkd.setMkdFlags(MkdFlags::AUTOLINK, MkdFlags::TOC);
	std::cout << "OK" << std::endl
	          << "Testing flag state" << std::endl
	          << "AUTOLINK set ... ";
	assert(mkd.has(MkdFlags::AUTOLINK));
	std::cout << "OK" << std::endl
	          << "TOC set ... ";
	assert(mkd.has(MkdFlags::TOC));
	std::cout << "OK" << std::endl
	          << "CDATA not set ... ";
	assert(!mkd.has(MkdFlags::CDATA));
	std::cout << "OK" << std::endl
	          << "Clearing flags ... ";
	mkd.clearMkdFlags();
	std::cout << "OK" << std::endl
	          << "AUTOLINK cleared ... ";
	assert(!mkd.has(MkdFlags::AUTOLINK));
	std::cout << "OK" << std::endl
	          << "TOC cleared ... ";
	assert(!mkd.has(MkdFlags::TOC));
	std::cout << "OK" << std::endl
	          << "Setting markdown ... ";
	std::string test("% title\n% author\n% date\n# Hello");
	mkd.setMarkdown(test);
	assert(test.compare(mkd.markdown()) == 0);
	std::cout << "OK" << std::endl
	          << "Compiling markdown ... ";
	try {
		mkd.compile();
	}
	catch(std::exception& e) {
		std::cerr << e.what();
	}
	std::cout << "OK" << std::endl
	          << "Checking Pandoc-style document headers ... ";
	assert(mkd.author().compare("author") == 0);
	assert(mkd.date().compare("date") == 0);
	assert(mkd.title().compare("title") == 0);
	std::cout << "OK" << std::endl
	          << "Checking generated HTML ... ";
	assert(mkd.html().compare("<h1>Hello</h1>") == 0);
	std::cout << "OK" << std::endl;

	return EXIT_SUCCESS;
}
