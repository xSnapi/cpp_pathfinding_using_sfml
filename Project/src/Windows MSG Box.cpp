#include "utilities/Windows MSG Box.hpp"

#include <Windows.h>

int CustomMessageBox(const wchar_t* lpText, const wchar_t* lpCaption, unsigned int uType) {
	MSGBOXPARAMS mbp;

	mbp.cbSize = sizeof(MSGBOXPARAMS);
	mbp.hwndOwner = NULL;
	mbp.hInstance = GetModuleHandle(NULL);
	mbp.lpszText = lpText;
	mbp.lpszCaption = lpCaption;
	mbp.dwStyle = uType | MB_USERICON;
	mbp.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
	mbp.lpfnMsgBoxCallback = NULL;
	mbp.dwContextHelpId = 0;
	mbp.lpszIcon = NULL;

	return MessageBoxIndirect(&mbp);
}
