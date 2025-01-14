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
#include "xcamera_widget.h"
#include <QStyleOption>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDebug>
#include <QListWidget>
#include "xdemux_task.h"
#include "xdecode_task.h"
#include "xvideo_view.h"
#include "xcamera_config.h"
//��Ⱦ
void XCameraWidget::paintEvent(QPaintEvent* p)
{
    //��Ⱦ��ʽ��
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
XCameraWidget::XCameraWidget(QWidget* p):QWidget(p)
{
    //������ק
    this->setAcceptDrops(true);
}

//��ק����
void XCameraWidget::dragEnterEvent(QDragEnterEvent* e)
{
    //������ק����
    e->acceptProposedAction();
}
//��Ⱦ��Ƶ
void XCameraWidget::Draw()
{
    if (!demux_ || !decode_ || !view_)return;
    auto f = decode_->GetFrame();
    if (!f)return;
    view_->DrawFrame(f);
    XFreeFrame(&f);
}

//������Դ
XCameraWidget::~XCameraWidget()
{
    if (demux_)
    {
        demux_->Stop();
        delete demux_;
        demux_ == nullptr;
    }
    if (decode_)
    {
        decode_->Stop();
        delete decode_;
        decode_ == nullptr;
    }
    if (view_)
    {
        view_->Close();
        delete view_;
        view_ = nullptr;
    }
}

bool XCameraWidget::Open(const char* url)
{
    if (demux_)
        demux_->Stop();
    if (decode_)
        decode_->Stop();
    //�򿪽��װ�߳�
    demux_ = new XDemuxTask();
    if (!demux_->Open(url))
    {
        return false;
    }
    //����Ƶ�������߳�
    decode_ = new XDecodeTask();
    auto para = demux_->CopyVideoPara();
    if (!decode_->Open(para->para))
    {
        return false;
    }
    //�趨�����߳̽��ս��װ����
    demux_->set_next(decode_);

    //��ʼ����Ⱦ����
    view_ = XVideoView::Create();
    view_->set_win_id((void*)winId());
    view_->Init(para->para);

    //�������װ�ͽ����߳�
    demux_->Start();
    decode_->Start();
    return true;
}
//��ק�ɿ�
void XCameraWidget::dropEvent(QDropEvent* e)
{
    //�õ�url
    qDebug()<<e->source()->objectName();
    auto wid = (QListWidget*)e->source();
    qDebug() << wid->currentRow();
    auto cam = XCameraConfig::Instance()->GetCam(wid->currentRow());
    Open(cam.sub_url);

}