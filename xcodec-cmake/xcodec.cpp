#include "xcodec.h"
#include <iostream>
using namespace std;
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
}
//Ԥ����ָ����
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")


//////////////////////////////////////////
/// ��������������
/// @para codec_id ������ID�ţ���Ӧffmpeg
/// @return ���������� ,ʧ�ܷ���nullptr
AVCodecContext* XCodec::Create(int codec_id,bool isencode)
{
    //1 �ҵ�������
    const AVCodec *codec = nullptr;
    if(isencode)
        codec = avcodec_find_encoder((AVCodecID)codec_id);
    else
        codec = avcodec_find_decoder((AVCodecID)codec_id);
    if (!codec)
    {
        cerr << "avcodec_find_encoder failed!" << codec_id << endl;
        return nullptr;
    }
    //����������
    auto c = avcodec_alloc_context3(codec);
    if (!c)
    {
        cerr << "avcodec_alloc_context3 failed!" << codec_id << endl;
        return nullptr;
    }
    //���ò���Ĭ��ֵ
    c->time_base = { 1,25 };
    c->pix_fmt = AV_PIX_FMT_YUV420P;
    c->thread_count = 16;
    return c;
}

//////////////////////////////////////////
/// ���ö���ı����������� �����Ĵ��ݵ������У���Դ��XEncodeά��
/// ���� �̰߳�ȫ
/// @para c ������������ ���c_��Ϊnullptr������������Դ
void XCodec::set_c(AVCodecContext* c)
{
    unique_lock<mutex>lock(mux_);
    if (c_)
    {
        avcodec_free_context(&c_);
    }
    this->c_ = c;
}


bool XCodec::SetOpt(const char* key, const char* val)
{
    unique_lock<mutex>lock(mux_);
    if (!c_)return false;
    auto re = av_opt_set(c_->priv_data, key, val, 0);
    if (re != 0)
    {
        cerr << "av_opt_set failed!";
        PrintErr(re);
    }
    return true;
}

bool XCodec::SetOpt(const char* key, int val)
{
    unique_lock<mutex>lock(mux_);
    if (!c_)return false;
    auto re = av_opt_set_int(c_->priv_data, key, val, 0);
    if (re != 0)
    {
        cerr << "av_opt_set failed!";
        PrintErr(re);
    }
    return true;
}

//////////////////////////////////////////////////////////////
/// �򿪱����� �̰߳�ȫ
bool XCodec::Open()
{
    unique_lock<mutex>lock(mux_);
    if (!c_)return false;
    auto re = avcodec_open2(c_, NULL, NULL);
    if (re != 0)
    {
        PrintErr(re);
        return false;
    }
    return true;
}


///////////////////////////////////////////////////////////////
//����AVCodecContext ����һ��AVFrame����Ҫ�������ͷ�av_frame_free
AVFrame* XCodec::CreateFrame()
{
    unique_lock<mutex>lock(mux_);
    if (!c_)return nullptr;
    auto frame = av_frame_alloc();
    frame->width = c_->width;
    frame->height = c_->height;
    frame->format = c_->pix_fmt;
    auto re = av_frame_get_buffer(frame, 0);
    if (re != 0)
    {
        av_frame_free(&frame);
        PrintErr(re);
        return nullptr;
    }
    return frame;
}