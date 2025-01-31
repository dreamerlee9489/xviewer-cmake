#pragma once
#include "xtools.h"
#include "xmux.h"
class XCODEC_API XMuxTask :public XThread
{
public:
	void Main() override;
	/// <summary>
	/// �򿪷�װ�ļ�
	/// </summary>
	/// <param name="url">�����ַ</param>
	/// <param name="video_para">��Ƶ����</param>
	/// <param name="video_time_base">��Ƶʱ�����</param>
	/// <param name="audio_para">��Ƶ����</param>
	/// <param name="audio_time_base">��Ƶ��ʱ�����</param>
	/// <returns></returns>
	bool Open(const char* url,
		AVCodecParameters* video_para = nullptr,
		AVRational* video_time_base = nullptr,
		AVCodecParameters* audio_para = nullptr,
		AVRational* audio_time_base = nullptr
	);

	//��������
	void Do(AVPacket* pkt);
private:
	XMux xmux_;
	XAVPacketList pkts_;
	std::mutex mux_;

};

