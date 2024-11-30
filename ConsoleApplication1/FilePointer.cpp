#include "FilePointer.h"
#include <utility>
FilePointer::FilePointer(FILE* fp) noexcept
	: fp{ fp }//���
{
}
FilePointer::~FilePointer()
{
	if (fp)
		fclose(fp);
}
FilePointer::FilePointer(FilePointer&& r) noexcept
	: fp{r.fp}//�����ň�U�R�s�[
{
	r.fp = nullptr;//�R�s�[������������
}
FilePointer& FilePointer::operator=(FilePointer&& r) noexcept
{
	std::swap(fp, r.fp);
	return *this;//�K�� *this ��Ԃ�
}
FILE* FilePointer::get() const noexcept
{
	return fp;
}
FilePointer::operator bool() const noexcept
{
	return fp != nullptr;
}
