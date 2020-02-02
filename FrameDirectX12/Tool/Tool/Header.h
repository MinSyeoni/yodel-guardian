#pragma once

const unsigned int g_iWinCX = 1920;
const unsigned int g_iWinCY = 1080;

#define		MSG_BOX(_message)			MessageBoxW(NULL, _message, L"System Message", MB_OK)
#define		TAGMSG_BOX(_tag,_message)	MessageBoxW(NULL, TEXT(_message), _tag, MB_OK)