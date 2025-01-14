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
#include "xcamera_record.h"
#include "xdemux_task.h"
#include "xmux_task.h"

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
        MSleep(10);
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

    if (!mux.Open(save_path_.c_str(),
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
    MSleep(3000);
    mux.Stop();
    demux.Stop();
}