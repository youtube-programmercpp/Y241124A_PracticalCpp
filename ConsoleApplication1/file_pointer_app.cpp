#define	_CRT_SECURE_NO_WARNINGS
#include "FilePointer.h"
#include <stdlib.h>
#include <utility>
int main()
{
	FilePointer file1{ fopen(__FILE__, "r") };
	FilePointer file2;
	file2 = std::move(file1);//ˆÚ“®‘ã“ü‰‰ŽZŽq
	if (file2) {
		for (;;) {
			const auto ch = getc(file2.get());
			if (ch == EOF)
				return EXIT_SUCCESS;
			else
				putchar(ch);
		}
	}
	else
		return EXIT_FAILURE;
}
