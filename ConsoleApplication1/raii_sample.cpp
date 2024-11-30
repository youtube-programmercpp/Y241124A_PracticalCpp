#include <utility>
//例外安全
//例外（exception）が投入（throw）されても、
//オブジェクトのリーク（leak）が発生しないよう
//正しく作られている
class sample {
	char* p;
public:
	//引数が1個だけで、自己（自己への参照）ではないもの
	//変換コンストラクタ converting constructor
	sample(char* p) noexcept
		: p{p}
	{
	}
	//デストラクタ destructor
	~sample()
	{
		delete[]p;
	}

	//C++ で非常に大切…移動コンストラクタと移動代入演算子

	sample(sample&& r) noexcept
		: p{r.p}
	{
		r.p = nullptr;//移動元を消し込む
	}
	sample& operator=(sample&& r) noexcept
	{
		std::swap(p, r.p);
		return *this;
	}
};
void f()
{
		throw 12345;//例外オブジェクトは何型でも良い
}
#include <crtdbg.h>
int main()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	try {
		sample p{new char [10]};
		sample q{ nullptr };
		q = std::move(p);//移動なら大丈夫
		f();
	}
	catch (int error_code) {
	}
}
