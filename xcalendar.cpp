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
#include "xcalendar.h"
#include <QPainter>
XCalendar::XCalendar(QWidget* p):QCalendarWidget(p)
{

}
void XCalendar::paintCell(QPainter* painter,
    const QRect& rec,
    const QDate& date) const
{
    //����Ƶ������������ʾ
    if (mdate_.find(date) == mdate_.end()) //û����Ƶ
    {
        QCalendarWidget::paintCell(painter, rec, date);
        return;
    }

    

    auto font = painter->font();
    //��������
    font.setPixelSize(40);

    //ѡ��״̬ˢ����ɫ
    if (date == selectedDate())
    {
        painter->setBrush(QColor(118, 178, 224));//ˢ����ɫ
        painter->drawRect(rec);                 //���Ʊ���
    }
    painter->setFont(font);             //�����������ɫ
    painter->setPen(QColor(255, 0, 0)); //����ɫ
    painter->drawText(rec,Qt::AlignCenter,
        QString::number(date.day()));

}