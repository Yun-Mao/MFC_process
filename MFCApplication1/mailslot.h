#pragma once
class mailslot
{
private:
	HANDLE hmail;
public:
	mailslot(void);
	int send(CString content);
	CString read();
	int connect(CString server,CString name);
	int create(CString server, CString name);
	int free();
	int have();

};

