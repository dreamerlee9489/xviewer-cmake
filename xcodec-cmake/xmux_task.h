/*/*******************************************************************************
**                                                                            **
**                     Jiedi(China nanjing)Ltd.                               **
**	               创建：丁宋涛 夏曹俊，此代码可用作为学习参考                **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       :FFmpeg 4.2 从基础实战-多路H265监控录放开发 实训课

** Contact       : xiacaojun@qq.com
**  博客   : http://blog.csdn.net/jiedichina
**	视频课程 : 网易云课堂	http://study.163.com/u/xiacaojun		
			   腾讯课堂		https://jiedi.ke.qq.com/				
			   csdn学院               http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961	
**             51cto学院              http://edu.51cto.com/lecturer/index/user_id-12016059.html	
** 			   老夏课堂		http://www.laoxiaketang.com 
**                              更多资料请在此网页下载            http://ffmpeg.club
**  FFmpeg 4.2 从基础实战-多路H265监控录放开发 实训课  课程群 ：639014264加入群下载代码和学员交流
**                           微信公众号  : jiedi2007
**		头条号	 : 夏曹俊
**
*****************************************************************************
//！！！！！！！！！FFmpeg 4.2 从基础实战-多路H265监控录放开发 实训课 课程  QQ群：639014264下载代码和学员交流*/
#pragma once
#include "xtools.h"
#include "xmux.h"
class XCODEC_API XMuxTask :public XThread
{
public:
    void Main() override;
    /// <summary>
    /// 打开封装文件
    /// </summary>
    /// <param name="url">输出地址</param>
    /// <param name="video_para">视频参数</param>
    /// <param name="video_time_base">视频时间基数</param>
    /// <param name="audio_para">音频参数</param>
    /// <param name="audio_time_base">音频的时间基础</param>
    /// <returns></returns>
    bool Open(const char* url,
        AVCodecParameters* video_para = nullptr,
        AVRational *video_time_base = nullptr,
        AVCodecParameters *audio_para = nullptr,
        AVRational *audio_time_base = nullptr
        );

    //接收数据
    void Do(AVPacket* pkt);
private:
    XMux xmux_;
    XAVPacketList pkts_;
    std::mutex mux_;

};

