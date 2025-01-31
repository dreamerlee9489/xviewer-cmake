#include "xtools.h"
#include <sstream>
using namespace std;
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

void PrintErr(int err)
{
	char buf[1024] = { 0 };
	av_strerror(err, buf, sizeof(buf) - 1);
	cerr << buf << endl;
}


void XFreeFrame(AVFrame** frame)
{
	if (!frame || !(*frame))return;
	av_frame_free(frame);
}
void MSleep(unsigned int ms)
{
	auto beg = clock();
	for (int i = 0; i < ms; i++)
	{
		this_thread::sleep_for(1ms);
		if ((clock() - beg) / (CLOCKS_PER_SEC / 1000) >= ms)
			break;
	}
}
long long NowMs()
{
	return clock() / (CLOCKS_PER_SEC / 1000);
}

//�����߳�
void XThread::Start()
{
	unique_lock<mutex> lock(m_);
	static int i = 0;
	i++;
	index_ = i;
	is_exit_ = false;
	//�����߳�
	th_ = thread(&XThread::Main, this);
	stringstream ss;
	ss << "XThread::Start()" << index_;
	LOGINFO(ss.str());
}

//ֹͣ�̣߳������˳���־���ȴ��߳��˳���
void XThread::Stop()
{
	stringstream ss;
	ss << "XThread::Stop() begin" << index_;
	LOGINFO(ss.str());
	is_exit_ = true;
	if (th_.joinable()) //�ж����߳��Ƿ���Եȴ�
		th_.join();     //�ȴ����߳��˳�
	ss.str("");
	ss << "XThread::Stop() end" << index_;
	LOGINFO(ss.str());
}


//��������
XPara* XPara::Create()
{
	return new XPara();
}
XPara::~XPara()
{
	if (para)
	{
		avcodec_parameters_free(&para);
	}
	if (time_base)
	{
		delete time_base;
		time_base = nullptr;
	}
}

//˽���ǽ�ֹ����ջ�ж���
XPara::XPara()
{
	para = avcodec_parameters_alloc();
	time_base = new AVRational();
}

AVPacket* XAVPacketList::Pop()
{
	unique_lock<mutex> lock(mux_);
	if (pkts_.empty())return nullptr;
	auto pkt = pkts_.front();
	pkts_.pop_front();
	return pkt;
}

void XAVPacketList::Push(AVPacket* pkt)
{
	unique_lock<mutex> lock(mux_);
	//�����µ�AVPacket ���� ���ü���+1;
	auto p = av_packet_alloc();
	av_packet_ref(p, pkt);//���ü��� �������ݸ��ƣ��̰߳�ȫ
	pkts_.push_back(p);

	//�������ռ䣬�������ݣ����ؼ�֡λ��
	if (pkts_.size() > max_packets_)
	{
		//������һ֡
		if (pkts_.front()->flags & AV_PKT_FLAG_KEY)//�ؼ�֡
		{
			av_packet_free(&pkts_.front());//����
			pkts_.pop_front();  //����
			return;
		}
		//�������зǹؼ�֮֡ǰ������
		while (!pkts_.empty())
		{
			if (pkts_.front()->flags & AV_PKT_FLAG_KEY)//�ؼ�֡
			{
				return;
			}
			av_packet_free(&pkts_.front());//����
			pkts_.pop_front();  //����
		}
	}

}