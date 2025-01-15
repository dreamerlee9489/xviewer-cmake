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
#include "xsdl.h"
#include <SDL2/SDL.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"SDL2.lib")
static bool InitVideo()
{
    static bool is_first = true;
    static mutex mux;
    unique_lock<mutex> sdl_lock(mux);
    if (!is_first)return true;
    is_first = false;
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        cout << SDL_GetError() << endl;
        return false;
    }
    //�趨�����㷨������������,���Բ�ֵ�㷨
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    return true;
}
bool XSDL::IsExit()
{
    SDL_Event ev;
    SDL_WaitEventTimeout(&ev, 1);
    if (ev.type == SDL_QUIT)
        return true;
    return false;
}
void XSDL::Close()
{
    //ȷ���̰߳�ȫ
    unique_lock<mutex> sdl_lock(mtx_);
    if (texture_)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
    if (render_)
    {
        SDL_DestroyRenderer(render_);
        render_ = nullptr;
    }
    if (win_)
    {
        SDL_DestroyWindow(win_);
        win_ = nullptr;
    }
}

bool XSDL::Init(int w, int h,Format fmt)
{
    if (w <= 0 || h <= 0)return false;
    //��ʼ��SDL ��Ƶ��
    InitVideo();

    //ȷ���̰߳�ȫ
    unique_lock<mutex> sdl_lock(mtx_);
    width_ = w;
    height_ = h;
    fmt_ = fmt;

    if (texture_)
        SDL_DestroyTexture(texture_);
    if (render_)
        SDL_DestroyRenderer(render_);

    ///1 ��������
    if (!win_)
    {
        if (!win_id_)
        {
            //�½�����
            win_ = SDL_CreateWindow("",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
            );
        }
        else
        {
            //��Ⱦ���ؼ�����
            win_ = SDL_CreateWindowFrom(win_id_);
        }
    }
    if (!win_)
    {
        cerr << SDL_GetError() << endl;
        return false;
    }

    /// 2 ������Ⱦ��

    render_ = SDL_CreateRenderer(win_, -1, SDL_RENDERER_ACCELERATED);
    if (!render_)
    {
        cerr << SDL_GetError() << endl;
        return false;
    }
    //�������� ���Դ棩
    unsigned int sdl_fmt = SDL_PIXELFORMAT_RGBA8888;
    switch (fmt)
    {
    case XVideoView::RGBA:
        sdl_fmt = SDL_PIXELFORMAT_RGBA32;
        break;
    case XVideoView::BGRA:
        sdl_fmt = SDL_PIXELFORMAT_BGRA32;
        break;
    case XVideoView::ARGB:
        sdl_fmt = SDL_PIXELFORMAT_ARGB32;
        break;
    case XVideoView::YUV420P:
        sdl_fmt = SDL_PIXELFORMAT_IYUV;
        break;
    case XVideoView::NV12:
        sdl_fmt = SDL_PIXELFORMAT_NV12;
        break;
    default:
        break;
    }

    texture_ = SDL_CreateTexture(render_, 
        sdl_fmt,                        //���ظ�ʽ
        SDL_TEXTUREACCESS_STREAMING,    //Ƶ���޸ĵ���Ⱦ��������
        w, h                            //���ʴ�С
    );
    if (!texture_)
    {
        cerr << SDL_GetError() << endl;
        return false;
    }
    return true;
}
bool XSDL::Draw(
    const unsigned  char* y, int y_pitch,
    const unsigned  char* u, int u_pitch,
    const unsigned  char* v, int v_pitch
)
{
    //�������
    if (!y || !u || !v)return false;
    unique_lock<mutex> sdl_lock(mtx_);
    if (!texture_ || !render_ || !win_ || width_ <= 0 || height_ <= 0)
        return false;
    
    //�����ڴ浽���Դ�
    auto re = SDL_UpdateYUVTexture(texture_,
        NULL, 
        y,y_pitch,
        u,u_pitch,
        v,v_pitch);
    if (re != 0)
    {
        cout << SDL_GetError() << endl;
        return false;
    }
    //�����Ļ
    SDL_RenderClear(render_);

    //���ʸ��Ƶ���Ⱦ��

    SDL_Rect rect;
    SDL_Rect* prect = nullptr;
    if (scale_w_ > 0)  //�û��ֶ���������
    {
        rect.x = 0; rect.y = 0;
        rect.w = scale_w_;//��Ⱦ�Ŀ��ߣ�������
        rect.h = scale_w_;
        prect = &rect;
    }
    re = SDL_RenderCopy(render_, texture_, NULL, prect);
    if (re != 0)
    {
        cout << SDL_GetError() << endl;
        return false;
    }
    SDL_RenderPresent(render_);
    return true;
}
bool XSDL::Draw(const unsigned char* data,int linesize)
{
    if (!data)return false;
    unique_lock<mutex> sdl_lock(mtx_);
    if (!texture_ || !render_ || !win_ || width_ <= 0 || height_ <= 0)
        return false;
    if (linesize <= 0)
    {
        switch (fmt_)
        {
        case XVideoView::RGBA:
        case XVideoView::ARGB:
            linesize = width_ * 4;
            break;
        case XVideoView::YUV420P:
            linesize = width_;
            break;
        default:
            break;
        }
    }
    if (linesize <= 0)
        return false;
    //�����ڴ浽���Դ�
    auto re = SDL_UpdateTexture(texture_, NULL, data, linesize);
    if (re != 0)
    {
        cout << SDL_GetError() << endl;
        return false;
    }
    //�����Ļ
    SDL_RenderClear(render_);

    //���ʸ��Ƶ���Ⱦ��

    SDL_Rect rect;
    SDL_Rect* prect = nullptr;
    if (scale_w_ > 0 )  //�û��ֶ���������
    {
        rect.x = 0; rect.y = 0;
        rect.w = scale_w_;//��Ⱦ�Ŀ��ߣ�������
        rect.h = scale_w_;
        prect = &rect;
    }
    re = SDL_RenderCopy(render_, texture_, NULL, prect);
    if (re != 0)
    {
        cout << SDL_GetError() << endl;
        return false;
    }
    SDL_RenderPresent(render_);
    return true;
}