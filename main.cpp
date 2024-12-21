#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<iostream>
#include <pthread.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define DATALEN 1024

WSADATA Data;
int ret;
SOCKET sock;
sockaddr_in addr;

string getIP();
string getIP()
{
	WSADATA WSAData;
	char hostName[256];
	if (!WSAStartup(MAKEWORD(2, 0), &WSAData))
	{
		if (!gethostname(hostName, sizeof(hostName)))
		{
			hostent* host = gethostbyname(hostName);
			if (host != NULL)
			{
				return inet_ntoa(*(struct in_addr*)*host->h_addr_list);
			}
		}
	}
	return "Get IP failed.";
}
void* task0(void* arg)
{
	while (1)
	{
		char buf[DATALEN] = "";
		ret = recv(sock, buf, sizeof(buf), 0);
		if (ret <= 0) {
			WSACleanup();
			cout << "���շ���������ʧ��" << endl;
			system("pause");
			return NULL;
		}
		else
			cout << "��������" << buf << endl;
	}
	return NULL;
}
int main() {
	ret = WSAStartup(MAKEWORD(2, 2), &Data);
	if (ret) {
		cout << "��ʼ���������" << endl;
		WSACleanup();
		system("pause");
		return -1;
	}
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		printf("socket ����ʧ�� %d\n", sock);
		system("pause");
		return -1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.S_un.S_addr = inet_addr("192.168.117.248");
	int rett = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (rett == -1) {	
		cout << "���ӷ�����ʧ��" << endl;
		int error_code = WSAGetLastError();
		cout << "error code : " << error_code;
		WSACleanup();
		system("pause");
		return -1;
	}
	//�����߳�1
	pthread_t tid;
	if ((pthread_create(&tid, NULL, task0, NULL)) != 0) {
		fprintf(stderr, "pthread create error\n");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		char buf[DATALEN] = "";
		cin >> buf;
		ret = send(sock, buf, sizeof(buf), 0); //�����յ������ݷ��ط�����
		if (ret <= 0) {
			WSACleanup();
			cout << "���ͷ���������ʧ��" << endl;
			system("pause");
			return -1;
		}
	}
	WSACleanup();
}