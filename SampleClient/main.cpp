//�ʐM�v���O�����̃N���C�A���g��
#include "../SampleServer/NameOfPipe.h"
#include <fstream>
int main()
{
	if (std::ofstream pipe{ SampleServer_NameOfPipe }) {
		pipe << "TEST" << std::endl;
	}
}
//�N���C�A���g��
//�T�[�o�[�𗘗p���鑤
//�N���C�A���g���͒ʐM���Ă��邱�Ƃ�Y��Ă����v
//�t�@�C���̓ǂݏ������s���Ă���C���[�W
