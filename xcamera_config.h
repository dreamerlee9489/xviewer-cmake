#pragma once
#include <vector>
#include <mutex>
struct XCameraData
{
	char name[1024] = { 0 };
	char url[4096] = { 0 };         //�����������
	char sub_url[4096] = { 0 };     //�����������
	char save_path[4096] = { 0 };   //��Ƶ¼�ƴ��Ŀ¼
};

class XCameraConfig
{
public:

	//Ψһ����ʵ�� ����ģʽ
	static XCameraConfig* Instance()
	{
		static XCameraConfig xc;
		return &xc;
	}
	//��������� �̰߳�ȫ
	void Push(const XCameraData& data);

	/// <summary>
	/// ��ȡ����ͷ
	/// </summary>
	/// <param name="index">����ͷ���� ��0��ʼ</param>
	/// <returns>���ؿն��󣬿����ж�nameΪ���ַ���</returns>
	XCameraData GetCam(int index);


	/// <summary>
	/// �޸����������
	/// </summary>
	/// <param name="index"></param>
	/// <param name="data"></param>
	/// <returns></returns>
	bool SetCam(int index, const XCameraData& data);

	bool DelCam(int index);

	/// <summary>
	/// ��ȡ�������
	/// </summary>
	/// <returns>ʧ�ܷ���0</returns>
	int GetCamCount();

	/// <summary>
	/// �洢�����ļ�
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool Save(const char* path);


	/// <summary>
	/// ��ȡ���� ����cams_
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool Load(const char* path);
private:
	XCameraConfig() {}//����˽�� ����ģʽ
	std::vector<XCameraData> cams_;
	std::mutex mux_;
};

