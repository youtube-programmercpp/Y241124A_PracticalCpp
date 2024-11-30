#define	_CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	//「test.txt」を読み出し用に開く
	static const char filename[] = "test.txt";
	if (FILE* fp = fopen(filename, "r")) {
		//正常
		char buf[256];
		while (fgets(buf, sizeof buf, fp))
			fputs(buf, stdout);
		fclose(fp);
	}
	else {
		//異常
		fprintf(stderr, "ファイル「%s」のオープンに失敗しました。\n", filename);
	}
}
