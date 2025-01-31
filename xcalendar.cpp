#include "xcalendar.h"
#include <QPainter>
XCalendar::XCalendar(QWidget* p) :QCalendarWidget(p)
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
	painter->drawText(rec, Qt::AlignCenter,
		QString::number(date.day()));

}