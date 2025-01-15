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
#include "xtools.h"
#include "xdemux.h"
enum XSYN_TYPE
{
    XSYN_NONE = 0,  //����ͬ��
    XSYN_VIDEO = 1, //������Ƶͬ������������Ƶ
};
class XCODEC_API XDemuxTask :public XThread
{
public:
    void Main();
    /// <summary>
    /// �򿪽��װ
    /// </summary>
    /// <param name="url">rtsp��ַ</param>
    /// <param name="timeout_ms">��ʱʱ�� ��λ����</param>
    /// <returns></returns>
    bool Open(std::string url,int timeout_ms = 1000);

    //������Ƶ����
    std::shared_ptr<XPara> CopyVideoPara()
    {
        return demux_.CopyVideoPara();
    }
    std::shared_ptr<XPara> CopyAudioPara()
    {
        return demux_.CopyAudioPara();
    }
    void set_syn_type(XSYN_TYPE t) { syn_type_ = t; }
private:
    XDemux demux_;
    std::string url_;
    int timeout_ms_ = 0;//��ʱʱ��
    XSYN_TYPE syn_type_ = XSYN_NONE;
};

