//ラムダ式を直接呼び出すには
#include <stdio.h>
int main()
{
	//末尾にある () は関数呼び出しを表している
	[]() {printf("ラムダ式１\n");}();//関数呼び出し
	const auto 変数 = []() {printf("ラムダ式２\n");};//関数
	変数();//ここでラムダ式を呼び出す
}
