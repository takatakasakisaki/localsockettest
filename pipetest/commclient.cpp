#include "commclient.h"

commclient::commclient()
{
	m_wptr=0;
	m_msglen=0;
	
	m_socket.reset( new QLocalSocket(this));
}

commclient::~commclient()
{
	
}

bool commclient::send(char msg[], int len, int tmo)
{
	char buf[4];
	buf[0] = len;
	m_socket->write(buf, 1);
	m_socket->write(msg, len);
	bool rv = m_socket->waitForBytesWritten(tmo);
	return rv;
}

bool commclient::recv(char msg[], int len, int tmo)
{
	m_socket->waitForReadyRead(tmo);
	char buf[4];
	int lentoread;
	if(m_socket->read(buf, 1) > 0){
		lentoread = uchar(buf[1]);
	}
	else{
		return false;
	}
	if(lentoread > len){
		lentoread = len;
	}
	auto rlen= m_socket->read(msg, lentoread);
	if(rlen > 0){
		return true;
	}
	return false;
}
void commclient::connected()
{
	
}
bool commclient::Connect(int tmo)
{
	QObject::connect(m_socket.get(), &QLocalSocket::readyRead, this, &commclient::recv);
    QObject::connect(m_socket.get(), &QLocalSocket::errorOccurred, this, &commclient::recv);
    QObject::connect(m_socket.get(), &QLocalSocket::connected, this, &commclient::connected);
	m_socket->connectToServer(PIPENAME);
	bool rv =m_socket->waitForConnected(tmo);
	return rv;
}
