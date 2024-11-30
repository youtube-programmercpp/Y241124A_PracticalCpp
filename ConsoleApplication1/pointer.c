#define	_CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <crtdbg.h>

bool input_string(char** p)
{
	char buf[256];//���[�J���ϐ��i�����ϐ��j�c���̊֐�����A��ہA�����ɂȂ�
	//"%s"�͋󔒗ނŋ�؂�ꂽ�P��P��ǂݎ��
	if (scanf_s("%s", buf, (unsigned)sizeof buf) == 1) {
		//����
		const size_t n = strlen(buf);
		char* const new_memory = malloc(n + 1);
		if (new_memory) {
			memcpy(new_memory, buf, n);
			new_memory[n] = '\0';//null�I�[
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
	printf("���O�F");
	if (input_string(name)) {
		printf("�Z���F");
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
	//����������Y������݉�������
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	char* ���O, *�Z��;
	if (input_name_and_address(&���O, &�Z��)) {
		printf("���͂��ꂽ���O�́u%s�v�ł��B\n", ���O);
		printf("���͂��ꂽ�Z���́u%s�v�ł��B\n", �Z��);
		free(���O);
		free(�Z��);
	}
	else {
		//���s�i���͂����s�������A���͂����������Ƃ��Ă�����������t���Ɏ��s�������A�̂ǂ��炩�j
	}
}
