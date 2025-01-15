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
#include "xplayvideo.h"
void XPlayVideo::timerEvent(QTimerEvent* ev)
{
    if (!view_)return;
    auto f = decode_.GetFrame();
    if (!f)return;
    view_->DrawFrame(f);
    XFreeFrame(&f);
}
void XPlayVideo::Close()
{
    //�ر��ϴ�����
    demux_.Stop();
    decode_.Stop();
    if (view_)
    {
        view_->Close();
        delete view_;
        view_ = nullptr;
    }
}

void XPlayVideo::closeEvent(QCloseEvent* ev)
{
    Close();
}
bool XPlayVideo::Open(const char* url)
{

    if (!demux_.Open(url)) //���װ
    {
        return false;
    }
    auto vp = demux_.CopyVideoPara();
    if (!vp)
        return false;
    if (!decode_.Open(vp->para))//����
    {
        return false;
    }
    demux_.set_next(&decode_);
        
    if (!view_)
        view_ = XVideoView::Create();
    view_->set_win_id((void*)winId());
    if (!view_->Init(vp->para)) //SDL��Ⱦ
        return false;
    demux_.set_syn_type(XSYN_VIDEO);
    demux_.Start();
    decode_.Start();
    return true;
}
XPlayVideo::XPlayVideo(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    startTimer(10);
}

XPlayVideo::~XPlayVideo()
{
    Close();
}
