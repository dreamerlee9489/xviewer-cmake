#pragma once
#include "xcodec.h"
struct AVBufferRef;
class XCODEC_API XDecode :public XCodec
{
public:
	bool Send(const AVPacket* pkt);  //���ͽ���
	bool Recv(AVFrame* frame);       //��ȡ����
	std::vector<AVFrame*> End();    //��ȡ����

	bool InitHW(int type = 4);

};

