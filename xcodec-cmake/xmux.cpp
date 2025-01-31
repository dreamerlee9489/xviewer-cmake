#include "xmux.h"

#include <iostream>
#include <thread>
using namespace std;
extern "C" {
#include <libavformat/avformat.h>
}
void PrintErr(int err);

#define BERR(err) if(err!= 0){PrintErr(err);return 0;}
void XMux::set_src_video_time_base(AVRational* tb)
{
	if (!tb)return;
	unique_lock<mutex> lock(mux_);
	if (!src_video_time_base_)
		src_video_time_base_ = new AVRational();
	*src_video_time_base_ = *tb;
}
void XMux::set_src_audio_time_base(AVRational* tb)
{
	if (!tb)return;
	unique_lock<mutex> lock(mux_);
	if (!src_audio_time_base_)
		src_audio_time_base_ = new AVRational();
	*src_audio_time_base_ = *tb;
}
XMux::~XMux()
{
	unique_lock<mutex> lock(mux_);
	if (src_video_time_base_)
		delete src_video_time_base_;
	src_video_time_base_ = nullptr;
	if (src_audio_time_base_)
		delete src_audio_time_base_;
	src_audio_time_base_ = nullptr;
}

//////////////////////////////////////////////////
//// �򿪷�װ
AVFormatContext* XMux::Open(const char* url,
	AVCodecParameters* video_para,
	AVCodecParameters* audio_para)
{
	AVFormatContext* c = nullptr;
	//����������
	auto re = avformat_alloc_output_context2(&c, NULL, NULL, url);
	BERR(re);

	//������Ƶ��Ƶ��
	if (video_para)
	{
		auto vs = avformat_new_stream(c, NULL);   //��Ƶ��
		avcodec_parameters_copy(vs->codecpar, video_para);
	}
	if (audio_para)
	{
		auto as = avformat_new_stream(c, NULL);   //��Ƶ��
		avcodec_parameters_copy(as->codecpar, audio_para);
	}

	//��IO
	re = avio_open(&c->pb, url, AVIO_FLAG_WRITE);
	BERR(re);
	av_dump_format(c, 0, url, 1);
	return c;
}
bool XMux::Write(AVPacket* pkt)
{
	if (!pkt)return false;
	unique_lock<mutex> lock(mux_);
	if (!c_)return false;
	//û��ȡ��pts �ع�����ͨ��duration ����
	if (pkt->pts == AV_NOPTS_VALUE)
	{
		pkt->pts = 0;
		pkt->dts = 0;
	}
	if (pkt->stream_index == video_index_)
	{
		if (begin_video_pts_ < 0)
			begin_video_pts_ = pkt->pts;
		lock.unlock();
		RescaleTime(pkt, begin_video_pts_, src_video_time_base_);
		lock.lock();

	}
	else if (pkt->stream_index == audio_index_)
	{
		if (begin_audio_pts_ < 0)
			begin_audio_pts_ = pkt->pts;
		lock.unlock();
		RescaleTime(pkt, begin_audio_pts_, src_audio_time_base_);
		lock.lock();
	}

	cout << pkt->pts << " " << flush;
	//д��һ֡���ݣ��ڲ���������dts��ͨ��pkt=null ����д�뻺��
	auto re = av_interleaved_write_frame(c_, pkt);
	BERR(re);
	return true;
}

bool XMux::WriteEnd()
{
	unique_lock<mutex> lock(mux_);
	if (!c_)return false;
	int re = 0;
	//auto re = av_interleaved_write_frame(c_, nullptr);//д�����򻺳�
	//BERR(re);
	re = av_write_trailer(c_);
	BERR(re);
	return true;
}
bool XMux::WriteHead()
{
	unique_lock<mutex> lock(mux_);
	if (!c_)return false;
	//��ı�timebase
	auto re = avformat_write_header(c_, nullptr);
	BERR(re);

	//��ӡ���������
	av_dump_format(c_, 0, c_->url, 1);
	this->begin_audio_pts_ = -1;
	this->begin_video_pts_ = -1;


	return true;
}