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
#pragma once

//����Linux  _WIN32 windows 32Ϊ��64λ
#ifdef _WIN32
#ifdef XCODEC_EXPORTS
#define XCODEC_API __declspec(dllexport)
#else
#define XCODEC_API __declspec(dllimport)
#endif
#else
#define XCODEC_API
#endif

#include <thread>
#include <iostream>
#include <mutex>
#include <list>
#include <memory>
struct AVPacket;
struct AVCodecParameters;
struct AVRational;
struct AVFrame;
struct AVCodecContext;




//��־���� DEBUG INFO ERROR FATAL
enum XLogLevel
{
    XLOG_TYPE_DEBUG,
    XLOG_TYPE_INFO,
    XLOG_TPYE_ERROR,
    XLOG_TYPE_FATAL
};
#define LOG_MIN_LEVEL XLOG_TYPE_DEBUG
#define XLOG(s,level) \
    if(level>=LOG_MIN_LEVEL) \
    std::cout<<level<<":"<<__FILE__<<":"<<__LINE__<<":\n"\
    <<s<<std::endl;
#define LOGDEBUG(s) XLOG(s,XLOG_TYPE_DEBUG)
#define LOGINFO(s) XLOG(s,XLOG_TYPE_INFO)
#define LOGERROR(s) XLOG(s,XLOG_TPYE_ERROR)
#define LOGFATAL(s) XLOG(s,XLOG_TYPE_FATAL)


XCODEC_API void MSleep(unsigned int ms);

//��ȡ��ǰʱ��� ����
XCODEC_API long long NowMs();

XCODEC_API void XFreeFrame(AVFrame** frame);

XCODEC_API void PrintErr(int err);

class XCODEC_API XThread
{
public:
    //�����߳�
    virtual void Start();

    //ֹͣ�̣߳������˳���־���ȴ��߳��˳���
    virtual void Stop();

    //ִ������ ��Ҫ����
    virtual void Do(AVPacket* pkt) {}

    //���ݵ���һ������������
    virtual void Next(AVPacket* pkt)
    {
        std::unique_lock<std::mutex> lock(m_);
        if (next_)
            next_->Do(pkt);

    }

    //������������һ���ڵ㣨�̰߳�ȫ��
    void set_next(XThread* xt)
    {
        std::unique_lock<std::mutex> lock(m_);
        next_ = xt;
    }
protected:
    
    //�߳���ں���
    virtual void Main() = 0;

    //��־�߳��˳�
    bool is_exit_ = false;
    
    //�߳�������
    int index_ = 0;


private:
    std::thread th_;
    std::mutex m_;
    XThread *next_ = nullptr;//��������һ���ڵ�

};
class XTools
{
};



//����Ƶ����
class XCODEC_API XPara
{
public:
    AVCodecParameters* para = nullptr;  //����Ƶ����
    AVRational* time_base = nullptr;    //ʱ�����

    //��������
    static XPara* Create();
    ~XPara();
private:
    //˽���ǽ�ֹ����ջ�ж���
    XPara();
};


/// <summary>
/// �̰߳�ȫavpacket list
/// </summary>
class XCODEC_API XAVPacketList
{
public:
    AVPacket* Pop();
    void Push(AVPacket* pkt);
private:
    std::list<AVPacket*> pkts_;
    int max_packets_ = 100;//����б���������������
    std::mutex mux_;
};