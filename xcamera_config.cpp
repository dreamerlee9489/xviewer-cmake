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
#include "xcamera_config.h"
#include <fstream>
using namespace std;

//��������� �̰߳�ȫ
void XCameraConfig::Push(const XCameraData& data)
{
    unique_lock<mutex> lock(mux_);
    cams_.push_back(data);
}
int XCameraConfig::GetCamCount()
{
    unique_lock<mutex> lock(mux_);
    return cams_.size();
}
XCameraData XCameraConfig::GetCam(int index)
{
    unique_lock<mutex> lock(mux_);
    if (index<0 || index> cams_.size())
        return XCameraData();
    return cams_[index];
}
bool XCameraConfig::DelCam(int index)
{
    unique_lock<mutex> lock(mux_);
    if (index<0 || index> cams_.size())
        return false;
    cams_.erase(cams_.begin() + index);
    return true;
}
bool XCameraConfig::SetCam(int index, const XCameraData& data)
{
    unique_lock<mutex> lock(mux_);
    if (index<0 || index> cams_.size())
        return false;
    cams_[index] = data;
    return true;
}
bool XCameraConfig::Load(const char* path)
{
    if (!path)return false;
    ifstream ifs(path, ios::binary);
    if (!ifs)return false;
    XCameraData data;
    unique_lock<mutex> lock(mux_);
    cams_.clear();
    for (;;)
    {
        ifs.read((char*)&data, sizeof(data));
        if (ifs.gcount() != sizeof(data))
        {
            ifs.close();
            return true;
        }
        cams_.push_back(data);
    }
    ifs.close();
    return true;
}
bool XCameraConfig::Save(const char* path)
{
    if (!path)return false;
    ofstream ofs(path,ios::binary);
    if (!ofs)return false;
    unique_lock<mutex> lock(mux_);
    for (auto cam : cams_)
    {
        ofs.write((char*)&cam, sizeof(cam));
    }
    ofs.close();
    return true;
}