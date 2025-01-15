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
#include "xencode.h"
#include <iostream>
using namespace std;
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
}


//////////////////////////////////////////////////////////////
/// �������� �̰߳�ȫ ÿ���´���AVPacket
/// @para frame �ռ����û�ά��
/// @return ʧ�ܷ�Χnullptr ���ص�AVPacket�û���Ҫͨ��av_packet_free ����
AVPacket* XEncode::Encode(const AVFrame* frame)
{
    if (!frame)return nullptr;
    unique_lock<mutex>lock(mux_);
    if (!c_)return nullptr;
    av_frame_make_writable((AVFrame*)frame);
    //���͵������߳�
    auto re = avcodec_send_frame(c_, frame);
    if (re != 0)return nullptr;
    auto pkt = av_packet_alloc();
    //���ձ����߳�����
    re = avcodec_receive_packet(c_, pkt);
    if (re == 0)
    {
        return pkt;
    }
    av_packet_free(&pkt);
    if (re == AVERROR(EAGAIN) || re == AVERROR_EOF)
    {
        return nullptr;
    }
    if (re < 0)
    {
        PrintErr(re);
    }
    return nullptr;

}

//////////////////////////////////////////////////////////////
//�������б��뻺����AVPacket
std::vector<AVPacket*> XEncode::End()
{
    std::vector<AVPacket*> res;
    unique_lock<mutex>lock(mux_);
    if (!c_)return res;
    auto re = avcodec_send_frame(c_, NULL); //����NULL ��ȡ����
    if (re != 0)return res;
    while (re >= 0)
    {
        auto pkt = av_packet_alloc();
        re = avcodec_receive_packet(c_, pkt);
        if (re != 0)
        {
            av_packet_free(&pkt);
            break;
        }
        res.push_back(pkt);
    }
    return res;
}
