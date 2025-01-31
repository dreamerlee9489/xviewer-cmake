#pragma once
#include "xformat.h"
class XCODEC_API XDemux :public XFormat
{
public:
	/// <summary>
	/// �򿪽��װ
	/// </summary>
	/// <param name="url">���װ��ַ ֧��rtsp</param>
	/// <returns>ʧ�ܷ���nullptr</returns>
	static AVFormatContext* Open(const char* url);

	/// <summary>
	/// ��ȡһ֡����
	/// </summary>
	/// <param name="pkt">�������</param>
	/// <returns>�Ƿ�ɹ�</returns>
	bool Read(AVPacket* pkt);

	bool Seek(long long pts, int stream_index);


};

