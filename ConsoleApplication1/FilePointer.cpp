#include "FilePointer.h"
#include <utility>
FilePointer::FilePointer(FILE* fp) noexcept
	: fp{ fp }//代入
{
}
FilePointer::~FilePointer()
{
	if (fp)
		fclose(fp);
}
FilePointer::FilePointer(FilePointer&& r) noexcept
	: fp{r.fp}//ここで一旦コピー
{
	r.fp = nullptr;//コピー元を消し込む
}
FilePointer& FilePointer::operator=(FilePointer&& r) noexcept
{
	std::swap(fp, r.fp);
	return *this;//必ず *this を返す
}
FILE* FilePointer::get() const noexcept
{
	return fp;
}
FilePointer::operator bool() const noexcept
{
	return fp != nullptr;
}
