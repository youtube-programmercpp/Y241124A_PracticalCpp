#pragma once
#include <stdio.h>
class FilePointer
{
	FILE* fp;//�����o�ϐ��� private �ɂ���(����)
public:
	//�������P�������R���X�g���N�^�̓f�t�H���g�R���X�g���N�^
	//�f�t�H���g���������邱�ƂŃf�t�H���g�R���X�g���N�^�̖�������������
	FilePointer(FILE* fp = nullptr) noexcept;//�ɗ� noexcept �ɂ���
	~FilePointer();
	FilePointer(FilePointer&& r) noexcept;//�ړ��R���X�g���N�^
	FilePointer& operator=(FilePointer&& r) noexcept;//�ړ�������Z�q
	FILE* get() const noexcept;//std::unique_ptr �ɂ��� get �����o�Ɠ����l����
	explicit operator bool() const noexcept;
};

/*
�����ȂƂ��� NULL �ƂȂ�̂Ȃ� std::unique_ptr �Ŏ�舵����\������
���O�̃N���X���K�v�ȃP�[�X
�����ȃt�@�C���n���h���� INVALID_HANDLE_VALUE (-1) �ł���B
�����ȃ\�P�b�g�n���h���� INVALID_SOCKET (-1)
*/