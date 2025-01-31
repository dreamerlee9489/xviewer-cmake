#pragma once
#include "xcodec.h"
class XCODEC_API XEncode :public XCodec
{
public:

	//////////////////////////////////////////////////////////////
	/// �������� �̰߳�ȫ ÿ���´���AVPacket
	/// @para frame �ռ����û�ά��
	/// @return ʧ�ܷ�Χnullptr ���ص�AVPacket�û���Ҫͨ��av_packet_free ����
	AVPacket* Encode(const AVFrame* frame);


	//////////////////////////////////////////////////////////////
	//�������б��뻺����AVPacket
	std::vector<AVPacket*> End();

};

