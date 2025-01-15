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
#include "xdecode.h"
#include <iostream>
using namespace std;
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
}
bool XDecode::Send(const AVPacket* pkt)  //���ͽ���
{
    unique_lock<mutex> lock(mux_);
    if (!c_)return false;
    auto re = avcodec_send_packet(c_, pkt);
    if (re != 0)return false;
    return true;
}

bool XDecode::Recv(AVFrame* frame)      //��ȡ����
{
    unique_lock<mutex> lock(mux_);
    if (!c_)return false;
    auto f = frame;
    if (c_->hw_device_ctx) //Ӳ������
    {
        f = av_frame_alloc();
    }
    auto re = avcodec_receive_frame(c_, f);
    if (re == 0)
    {
        if (c_->hw_device_ctx) //GPU����
        {
            //�Դ�ת�ڴ� GPU =�� CPU
            re = av_hwframe_transfer_data(frame, f, 0); 
            av_frame_free(&f);
            if (re != 0)
            {
                PrintErr(re);
                return false;
            }
        }
        return true;
    }
    if (c_->hw_device_ctx)
        av_frame_free(&f);
    return false;
}
bool XDecode::InitHW(int type)
{
    unique_lock<mutex> lock(mux_);
    if (!c_)return false;
    ;
    AVBufferRef* ctx = nullptr; //Ӳ������������
    auto re = av_hwdevice_ctx_create(&ctx, (AVHWDeviceType)type,NULL,NULL,0);
    if (re != 0)
    {
        PrintErr(re);
        return false;
    }
    c_->hw_device_ctx = ctx;
    cout << "Ӳ�����٣�" << type << endl;
    return true;
}
std::vector<AVFrame*> XDecode::End()    //��ȡ����
{
    std::vector<AVFrame*> res;
    unique_lock<mutex> lock(mux_);
    if (!c_)return res;

    ///ȡ����������
    int ret = avcodec_send_packet(c_, NULL);
    while (ret >= 0)
    {
        auto frame = av_frame_alloc();
        ret = avcodec_receive_frame(c_, frame);
        if (ret < 0)
        {
            av_frame_free(&frame);
            break;
        }
        res.push_back(frame);
    }
    return res;
}