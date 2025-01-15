/*/*******************************************************************************
**                                                                            **
**                     Jiedi(China nanjing)Ltd.                               **
**	               ������������ �Ĳܿ����˴��������Ϊѧϰ�ο�                **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       :FFmpeg 4.2 �ӻ���ʵս-��·H265���¼�ſ��� ʵѵ��

** Contact       : xiacaojun@qq.com
**  ����   : http://blog.csdn.net/jiedichina
**	��Ƶ�γ� : �����ƿ���	http://study.163.com/u/xiacaojun		
			   ��Ѷ����		https://jiedi.ke.qq.com/				
			   csdnѧԺ               http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961	
**             51ctoѧԺ              http://edu.51cto.com/lecturer/index/user_id-12016059.html	
** 			   ���Ŀ���		http://www.laoxiaketang.com 
**                              �����������ڴ���ҳ����            http://ffmpeg.club
**  FFmpeg 4.2 �ӻ���ʵս-��·H265���¼�ſ��� ʵѵ��  �γ�Ⱥ ��639014264����Ⱥ���ش����ѧԱ����
**                           ΢�Ź��ں�  : jiedi2007
**		ͷ����	 : �Ĳܿ�
**
*****************************************************************************
//������������������FFmpeg 4.2 �ӻ���ʵս-��·H265���¼�ſ��� ʵѵ�� �γ�  QQȺ��639014264���ش����ѧԱ����*/
#include "xmux_task.h"
extern "C"
{
#include <libavformat/avformat.h>
}
using namespace std;
void XMuxTask::Do(AVPacket* pkt)
{
    pkts_.Push(pkt);
    Next(pkt);
}
void XMuxTask::Main()
{
    xmux_.WriteHead();

    //�ҵ��ؼ�֡
    while (!is_exit_)
    {
        unique_lock<mutex> lock(mux_);
        auto pkt = pkts_.Pop();
        if (!pkt)
        {
            MSleep(1);
            continue;
        }
        if(pkt->stream_index == xmux_.video_index()
            && pkt->flags & AV_PKT_FLAG_KEY) //�ؼ�֡
        {
            xmux_.Write(pkt);
            av_packet_free(&pkt);
            break;
        }
        //��������Ƶ�ؼ�֡
        av_packet_free(&pkt);
    }


    while (!is_exit_)
    {
        unique_lock<mutex> lock(mux_);
        auto pkt = pkts_.Pop();
        if (!pkt)
        {
            MSleep(1);
            continue;
        }

        xmux_.Write(pkt);
        cout << "W"<< flush;

        av_packet_free(&pkt);
    }
    xmux_.WriteEnd();
    xmux_.set_c(nullptr);
}
bool XMuxTask::Open(const char* url,
    AVCodecParameters* video_para,
    AVRational* video_time_base,
    AVCodecParameters* audio_para ,
    AVRational* audio_time_base 
)
{
    auto c = xmux_.Open(url,video_para,audio_para);
    if (!c)return false;
    xmux_.set_c(c);
    xmux_.set_src_video_time_base(video_time_base);
    xmux_.set_src_audio_time_base(audio_time_base);
    return true;
}