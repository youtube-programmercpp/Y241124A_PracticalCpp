#pragma once
#include <stdio.h>
class FilePointer
{
	FILE* fp;//メンバ変数は private にする(原則)
public:
	//引数が１個も無いコンストラクタはデフォルトコンストラクタ
	//デフォルト引数を入れることでデフォルトコンストラクタの役割を持たせる
	FilePointer(FILE* fp = nullptr) noexcept;//極力 noexcept にする
	~FilePointer();
	FilePointer(FilePointer&& r) noexcept;//移動コンストラクタ
	FilePointer& operator=(FilePointer&& r) noexcept;//移動代入演算子
	FILE* get() const noexcept;//std::unique_ptr にある get メンバと同じ考え方
	explicit operator bool() const noexcept;
};

/*
無効なときに NULL となるのなら std::unique_ptr で取り扱える可能性が大
自前のクラスが必要なケース
無効なファイルハンドルは INVALID_HANDLE_VALUE (-1) である。
無効なソケットハンドルは INVALID_SOCKET (-1)
*/