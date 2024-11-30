#define	_CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdexcept>
#include <sstream>
#include <Windows.h>
#define error_check_not(xpr, unexpected_literal) \
[](auto retval)\
{\
	if (retval == unexpected_literal)\
		throw std::runtime_error((std::ostringstream()\
			<< __FILE__ "(" _CRT_STRINGIZE(__LINE__) "): " \
			<< #xpr << " �����ҊO�̒l " << unexpected_literal\
			<< " ��Ԃ��܂����B").str());\
	else\
		return retval;\
}(xpr)
#define error_check_equ(xpr, expected_literal) \
[](auto retval)\
{\
	if (retval == expected_literal)\
		return retval;\
	else\
		throw std::runtime_error((std::ostringstream()\
			<< __FILE__ "(" _CRT_STRINGIZE(__LINE__) "): " \
			<< #xpr << " �� " << expected_literal\
			<< " ��Ԃ��܂����B").str());\
}(xpr)


#include <Windows.h>
#include <memory> //std::unique_ptr
int main()
{
	try {
		const auto p = fclose;
		//try�u���b�N
		std::unique_ptr<FILE, decltype(fclose)*> fp
		{ error_check_not(fopen(__FILE__, "r"), nullptr)
		, fclose
		};
		int a, b;
		printf("a = "); error_check_equ(scanf("%d", &a), 1);
		printf("b = "); error_check_equ(scanf("%d", &b), 1);
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
		OutputDebugStringA("\n");
		//�����ŗ�O��ߑ�
		printf("��O���������܂����B\n"
			"�G���[���b�Z�[�W�F%s\n"
			, e.what());
	}
}
