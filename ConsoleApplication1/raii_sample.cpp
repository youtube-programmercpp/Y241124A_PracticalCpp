#include <utility>
//��O���S
//��O�iexception�j�������ithrow�j����Ă��A
//�I�u�W�F�N�g�̃��[�N�ileak�j���������Ȃ��悤
//����������Ă���
class sample {
	char* p;
public:
	//������1�����ŁA���ȁi���Ȃւ̎Q�Ɓj�ł͂Ȃ�����
	//�ϊ��R���X�g���N�^ converting constructor
	sample(char* p) noexcept
		: p{p}
	{
	}
	//�f�X�g���N�^ destructor
	~sample()
	{
		delete[]p;
	}

	//C++ �Ŕ��ɑ�؁c�ړ��R���X�g���N�^�ƈړ�������Z�q

	sample(sample&& r) noexcept
		: p{r.p}
	{
		r.p = nullptr;//�ړ�������������
	}
	sample& operator=(sample&& r) noexcept
	{
		std::swap(p, r.p);
		return *this;
	}
};
void f()
{
		throw 12345;//��O�I�u�W�F�N�g�͉��^�ł��ǂ�
}
#include <crtdbg.h>
int main()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	try {
		sample p{new char [10]};
		sample q{ nullptr };
		q = std::move(p);//�ړ��Ȃ���v
		f();
	}
	catch (int error_code) {
	}
}
