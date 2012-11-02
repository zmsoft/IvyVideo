#include "IvyClient.h"

CIvyClient::CIvyClient()
{}

CIvyClient::~CIvyClient()
{}

bool CIvyClient::init()
{
	return false;
}

void CIvyClient::uninit()
{}

bool CIvyClient::startSelfVideo()
{
	return false;
}

bool CIvyClient::stopSelfVideo()
{
	return false;
}

bool CIvyClient::requestPeerVideo()
{
	return false;
}

bool CIvyClient::cancelPeerVideo()
{
	return false;
}

void CIvyClient::onPacked(char *data, int size, int type)
{
}

void CIvyClient::onDecoded(char *data, int size, int format)
{
}

