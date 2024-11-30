//lambda式の練習
#include <stdio.h>
#include <stdexcept>
#include <Windows.h>
int main()
{
	//ローカル関数は不可
	//[] で始まり、そのあとは通常の関数定義と殆ど同じ
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
