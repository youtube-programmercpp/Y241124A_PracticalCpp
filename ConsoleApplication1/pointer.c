#define	_CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <crtdbg.h>

bool input_string(char** p)
{
	char buf[256];//ローカル変数（自動変数）…この関数から帰る際、無効になる
	//"%s"は空白類で区切られた単語１個を読み取る
	if (scanf_s("%s", buf, (unsigned)sizeof buf) == 1) {
		//成功
		const size_t n = strlen(buf);
		char* const new_memory = malloc(n + 1);
		if (new_memory) {
			memcpy(new_memory, buf, n);
			new_memory[n] = '\0';//null終端
			*p = new_memory;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}
bool input_name_and_address(char** name, char** address)
{
	printf("名前：");
	if (input_string(name)) {
		printf("住所：");
		if (input_string(address)) {
			return true;
		}
		else {
			free(*name);
			return false;
		}
	}
	else
		return false;
}
int main()
{
	//メモリ解放忘れを顕在化させる
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	char* 名前, *住所;
	if (input_name_and_address(&名前, &住所)) {
		printf("入力された名前は「%s」です。\n", 名前);
		printf("入力された住所は「%s」です。\n", 住所);
		free(名前);
		free(住所);
	}
	else {
		//失敗（入力が失敗したか、入力が成功したとしてもメモリ割り付けに失敗したか、のどちらか）
	}
}
