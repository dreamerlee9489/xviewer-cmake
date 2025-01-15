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
#include "xformat.h"
#include <iostream>
#include <thread>
#include "xtools.h"
using namespace std;
extern "C" { //ָ��������c���Ժ��������������������ر�ע
//����ffmpegͷ�ļ�
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
//Ԥ����ָ����
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
using namespace std;

static int TimeoutCallback(void* para)
{
    auto xf = (XFormat*)para;
    if (xf->IsTimeout())return 1;//��ʱ�˳�Read
    return 0; //��������
}

void XFormat::set_c(AVFormatContext* c)
{
    unique_lock<mutex> lock(mux_);
    if (c_) //����ԭֵ
    {
        if (c_->oformat) //���������
        {
            if (c_->pb)
                avio_closep(&c_->pb);
            avformat_free_context(c_);
        }
        else if (c_->iformat)  //����������
        {
            avformat_close_input(&c_);
        }
        else
        {
            avformat_free_context(c_);
        }
    }
    c_ = c;
    if (!c_)
    {
        is_connected_ = false;
        return;
    }
    is_connected_ = true;

    //��ʱ ���ڳ�ʱ�ж�
    last_time_ = NowMs();

    //�趨��ʱ�����ص�
    if (time_out_ms_ > 0)
    {
        AVIOInterruptCB cb = { TimeoutCallback ,this };
        c_->interrupt_callback = cb;
    }

    //���������Ƿ�����Ƶ������Ƶ��
    audio_index_ = -1; 
    video_index_ = -1;
    //��������Ƶstream ����
    for (int i = 0; i < c->nb_streams; i++)
    {
        //��Ƶ
        if (c->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            audio_index_ = i;
            audio_time_base_.den = c->streams[i]->time_base.den;
            audio_time_base_.num = c->streams[i]->time_base.num;
        }
        else if (c->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_index_ = i;
            video_time_base_.den = c->streams[i]->time_base.den;
            video_time_base_.num = c->streams[i]->time_base.num;
            video_codec_id_ = c->streams[i]->codecpar->codec_id;
        }
    }
}

std::shared_ptr<XPara> XFormat::CopyAudioPara()
{
    int index = audio_index();
    shared_ptr<XPara> re;
    unique_lock<mutex> lock(mux_);
    if (index < 0 || !c_)return re;

    re.reset(XPara::Create());
    *re->time_base = c_->streams[index]->time_base;
    avcodec_parameters_copy(re->para, c_->streams[index]->codecpar);
    return re;
}

std::shared_ptr<XPara> XFormat::CopyVideoPara()
{
    int index = video_index();
    shared_ptr<XPara> re;
    unique_lock<mutex> lock(mux_);
    if (index < 0 || !c_)return re;

    re.reset(XPara::Create());
    *re->time_base = c_->streams[index]->time_base;
    avcodec_parameters_copy(re->para, c_->streams[index]->codecpar);
    return re;
}

bool XFormat::CopyPara(int stream_index, AVCodecContext* dts)
{
    unique_lock<mutex> lock(mux_);
    if (!c_)
    {
        return false;
    }
    if (stream_index<0 || stream_index>c_->nb_streams)
        return false;
    auto re = avcodec_parameters_to_context(dts, c_->streams[stream_index]->codecpar);
    if (re < 0)
    {
        return false;
    }
    return true;
}

/// <summary>
/// ���Ʋ��� �̰߳�ȫ
/// </summary>
/// <param name="stream_index">��Ӧc_->streams �±�</param>
/// <param name="dst">�������</param>
/// <returns>�Ƿ�ɹ�</returns>
bool XFormat::CopyPara(int stream_index, AVCodecParameters* dst)
{
    unique_lock<mutex> lock(mux_);
    if (!c_)
    {
        return false;
    }
    if (stream_index<0 || stream_index>c_->nb_streams)
        return false;
    auto re = avcodec_parameters_copy(dst, c_->streams[stream_index]->codecpar);
    if (re < 0)
    {
        return false;
    }
    return true;
}
//��pts dts duration ֵתΪ����
long long XFormat::RescaleToMs(long long pts, int index)
{
    unique_lock<mutex> lock(mux_);
    if (!c_ || index <0 || index > c_->nb_streams)return 0;
    auto in_timebase = c_->streams[index]->time_base;

    AVRational out_timebase = { 1,1000 };//���timebase ����
    return av_rescale_q(pts, in_timebase, out_timebase);
}
bool XFormat::RescaleTime(AVPacket* pkt, long long offset_pts, AVRational* time_base)
{
    if (!pkt || !time_base)return false;
    unique_lock<mutex> lock(mux_);
    if (!c_)return false;
    auto out_stream = c_->streams[pkt->stream_index];
    pkt->pts = av_rescale_q_rnd(pkt->pts - offset_pts, *time_base,
        out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX)
    );
    pkt->dts = av_rescale_q_rnd(pkt->dts - offset_pts, *time_base,
        out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX)
    );
    pkt->duration = av_rescale_q(pkt->duration, *time_base, out_stream->time_base);
    pkt->pos = -1;
    return true;
}
bool XFormat::RescaleTime(AVPacket* pkt, long long offset_pts, XRational time_base)
{
    AVRational in_time_base;
    in_time_base.num = time_base.num;
    in_time_base.den = time_base.den;
    return RescaleTime(pkt, offset_pts,&in_time_base);
}

//�趨��ʱʱ��
void XFormat::set_time_out_ms(int ms)
{
    unique_lock<mutex> lock(mux_);
    this->time_out_ms_ = ms;
    //���ûص�������������ʱ�˳�
    if (c_)
    {
        AVIOInterruptCB cb = { TimeoutCallback ,this };
        c_->interrupt_callback = cb;
    }
}
