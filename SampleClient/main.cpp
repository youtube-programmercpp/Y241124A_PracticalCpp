//通信プログラムのクライアント側
#include "../SampleServer/NameOfPipe.h"
#include <fstream>
int main()
{
	if (std::ofstream pipe{ SampleServer_NameOfPipe }) {
		pipe << "TEST" << std::endl;
	}
}
//クライアント側
//サーバーを利用する側
//クライアント側は通信していることを忘れても大丈夫
//ファイルの読み書きを行っているイメージ
