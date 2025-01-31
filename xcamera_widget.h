#pragma once
#include <QWidget>
class XDecodeTask;
class XDemuxTask;
class XVideoView;
class XCameraWidget :public QWidget
{
	Q_OBJECT

public:
	XCameraWidget(QWidget* p = nullptr);

	//��ק����
	void dragEnterEvent(QDragEnterEvent* e) override;

	//��ק�ɿ�
	void dropEvent(QDropEvent* e) override;

	//��Ⱦ
	void paintEvent(QPaintEvent* p);

	//��rtsp ��ʼ���װ ����
	bool Open(const char* url);


	//��Ⱦ��Ƶ
	void Draw();

	//������Դ
	~XCameraWidget();
private:
	XDecodeTask* decode_ = nullptr;
	XDemuxTask* demux_ = nullptr;
	XVideoView* view_ = nullptr;
};

