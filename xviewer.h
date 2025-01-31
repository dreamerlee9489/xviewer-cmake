#pragma once

#include <QtWidgets/QWidget>
#include "ui_xviewer.h"
#include <QMenu>
class XViewer : public QWidget
{
	Q_OBJECT

public:
	XViewer(QWidget* parent = Q_NULLPTR);

	//����¼� �����϶�����
	void mouseMoveEvent(QMouseEvent* ev) override;
	void mousePressEvent(QMouseEvent* ev) override;
	void mouseReleaseEvent(QMouseEvent* ev) override;

	//���ڴ�С��������
	void resizeEvent(QResizeEvent* ev) override;
	//�Ҽ��˵�
	void contextMenuEvent(QContextMenuEvent* event) override;

	//Ԥ����Ƶ����
	void View(int count);

	//ˢ���������б�
	void RefreshCams();

	//�༭�����
	void SetCam(int index);

	//��ʱ����Ⱦ��Ƶ �ص�����
	void timerEvent(QTimerEvent* ev) override;
public slots:
	void MaxWindow();
	void NormalWindow();
	void View1();
	void View4();
	void View9();
	void View16();
	void AddCam();  //�������������
	void SetCam();  //
	void DelCam();  //

	void StartRecord(); //��ʼȫ������ͷ¼��
	void StopRecord();  //ֹͣȫ������ͷ¼��
	void Preview();//Ԥ������
	void Playback();//�طŽ���

	void SelectCamera(QModelIndex index);//ѡ�������
	void SelectDate(QDate date);        //ѡ������
	void PlayVideo(QModelIndex index);  //ѡ��ʱ�䲥����Ƶ
private:
	Ui::XViewerClass ui;
	QMenu left_menu_;
};
