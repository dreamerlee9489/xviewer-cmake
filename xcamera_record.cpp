#include "xcamera_record.h"
#include "xdemux_task.h"
#include "xmux_task.h"
#include <chrono>
#include <iomanip>
#include <sstream>
using namespace std;
using namespace chrono;

/// ���ɴ洢����Ƶ�ļ���
static std::string GetFileName(string path)
{
	stringstream ss;
	//��ȡ��ǰʱ��
	auto t = system_clock::to_time_t(system_clock::now());
	auto time_str = put_time(localtime(&t), "%Y_%m_%d_%H_%M_%S");
	ss << path << "/" << "cam_" << time_str << ".mp4";
	return ss.str();
}

void XCameraRecord::Main()
{
	XDemuxTask demux;
	XMuxTask mux;
	if (rtsp_url_.empty())
	{
		LOGERROR("open rtsp url failed!");
		return;
	}

	//�Զ�����
	while (!is_exit_)
	{
		if (demux.Open(rtsp_url_)) //��������1��
		{
			break;
		}
		MSleep(3000);
		continue;
	}
	//����Ƶ����
	auto vpara = demux.CopyVideoPara();
	if (!vpara)
	{
		LOGERROR("demux.CopyVideoPara failed!");
		//��Ҫ����demux ��Դ�ͷŵ�����
		demux.Stop();
		return;
	}
	//�������װ�̣߳���ǰ��������ֹ��ʱ
	demux.Start();

	auto apara = demux.CopyAudioPara();

	AVCodecParameters* para = nullptr;  //��Ƶ����
	AVRational* timebase = nullptr;     //��Ƶʱ�����
	if (apara)
	{
		para = apara->para;
		timebase = apara->time_base;
	}

	//���˷�װ
	if (!mux.Open(GetFileName(save_path_).c_str(),
		vpara->para, vpara->time_base,//��Ƶ����
		para, timebase))            //��Ƶ����
	{
		LOGERROR("mux.Open rtsp_url_ failed!");
		demux.Stop();
		mux.Stop();
		return;
	}
	demux.set_next(&mux);
	mux.Start();

	//��ǰʱ��
	auto cur = NowMs();

	while (!is_exit_)
	{
		//��ʱ�����µ��ļ�
		if (NowMs() - cur > file_sec_ * 1000)
		{
			cur = NowMs();
			mux.Stop(); //ֹͣ�洢��д������
			//���˷�װ
			if (!mux.Open(GetFileName(save_path_).c_str(),
				vpara->para, vpara->time_base,//��Ƶ����
				para, timebase))            //��Ƶ����
			{
				LOGERROR("mux.Open rtsp_url_ failed!");
				demux.Stop();
				mux.Stop();
				return;
			}
			mux.Start();
		}

		MSleep(10);
	}

	mux.Stop();
	demux.Stop();
}