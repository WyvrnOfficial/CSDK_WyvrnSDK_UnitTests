#include "WyvrnErrors.h"
#include "WyvrnLogger.h"
#include "WyvrnAPI.h"
#include "WyvrnSDKTypes.h"
#include "UnitTests.h"

#include <tchar.h>

using namespace WyvrnSDK;
using namespace std;

int Init()
{
	//WyvrnLogger::fprintf(stderr, "Loaded Chroma Editor DLL!\r\n");
	if (WyvrnAPI::InitAPI() != 0)
	{
		return -1;
	}

	return RZRESULT_SUCCESS;
}

int main()
{
	WyvrnLogger::fprintf(stderr, "App launched!\r\n");

	RZRESULT result = Init();
	if (result != RZRESULT_SUCCESS)
	{
		return result;
	}

	APPINFOTYPE appInfo = {};

	_tcscpy_s(appInfo.Title, 256, _T("WyvrnSDK - Unit Tests"));
	_tcscpy_s(appInfo.Description, 1024, _T("Unit Tests for Wyvrn"));
	_tcscpy_s(appInfo.Author.Name, 256, _T("Razer"));
	_tcscpy_s(appInfo.Author.Contact, 256, _T("https://github.com/RazerOfficial/CChromaEditor"));
	// 1 - app
	// 2 - game
	appInfo.Category = 1;

	result = WyvrnAPI::CoreInitSDK(&appInfo);
	if (result != RZRESULT_SUCCESS)
	{
		WyvrnLogger::fprintf(stderr, "Failed to initialize Chroma! %d", result);
		return result;
	}

	UnitTests::Run();

	WyvrnAPI::CoreUnInit();
	WyvrnLogger::printf("Cpp_UnitTests exited.\r\n");

	return 0;
}
