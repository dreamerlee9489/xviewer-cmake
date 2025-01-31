#pragma once
#include "xformat.h"

class XCODEC_API XMux :public XFormat
{
public:
	//////////////////////////////////////////////////
	//// �򿪷�װ
	static AVFormatContext* Open(const char* url,
		AVCodecParameters* video_para = nullptr,
		AVCodecParameters* audio_para = nullptr
	);

	bool WriteHead();

	bool Write(AVPacket* pkt);

	bool WriteEnd();

	//����Ƶʱ�����
	void set_src_video_time_base(AVRational* tb);
	void set_src_audio_time_base(AVRational* tb);

	~XMux();
private:
	AVRational* src_video_time_base_ = nullptr;
	AVRational* src_audio_time_base_ = nullptr;

	long long begin_video_pts_ = -1;//ԭ��Ƶ��ʼʱ��
	long long begin_audio_pts_ = -1;//ԭ��Ƶ��ʼʱ��
};

