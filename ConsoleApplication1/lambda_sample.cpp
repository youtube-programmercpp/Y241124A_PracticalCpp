//lambda���̗��K
#include <stdio.h>
#include <stdexcept>
#include <Windows.h>
int main()
{
	//���[�J���֐��͕s��
	//[] �Ŏn�܂�A���̂��Ƃ͒ʏ�̊֐���`�Ɩw�Ǔ���
	const auto input = [](const char* prompt)
	{
		fputs(prompt, stdout);
		int n;
		if (scanf_s("%d", &n) == 1)
			return n;
		else
			throw std::runtime_error(__FILE__ "(" _CRT_STRINGIZE(__LINE__) "): input error");
	};
	try {
		const auto a = input("a = ");
		const auto b = input("b = ");
		printf("a + b = %d\n", a + b);
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
		OutputDebugStringA("\n");
	}
}
