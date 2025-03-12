#include "WyvrnAPI.h"
#include "WyvrnErrors.h"
#include "WyvrnLogger.h"
#include "VerifyLibrarySignature.h"
#include <iostream>
#include <tchar.h>


#ifdef _WIN64
#define WYVRNSDK_LIBRARY_DLL	L"WyvrnSDK64.dll"
#else
#define WYVRNSDK_LIBRARY_DLL	L"WyvrnSDK.dll"
#endif


using namespace WyvrnSDK;
using namespace std;

HMODULE WyvrnAPI::_sLibrary = nullptr;
bool WyvrnAPI::_sInvalidSignature = false;
bool WyvrnAPI::_sIsInitializedAPI = false;

#define WYVRNSDK_DECLARE_METHOD_IMPL(Signature, FieldName) Signature WyvrnAPI::FieldName = nullptr;

#pragma region API declare assignments
WYVRNSDK_DECLARE_METHOD_IMPL(PLUGIN_CORE_INIT_SDK, CoreInitSDK);
WYVRNSDK_DECLARE_METHOD_IMPL(PLUGIN_CORE_SET_EVENT_NAME, CoreSetEventName);
WYVRNSDK_DECLARE_METHOD_IMPL(PLUGIN_CORE_UNINIT, CoreUnInit);
#pragma endregion

#define WYVRNSDK_VALIDATE_METHOD(Signature, FieldName) FieldName = (Signature) GetProcAddress(library, "Plugin" #FieldName); \
if (FieldName == nullptr) \
{ \
	cerr << "Failed to find method: " << ("Plugin" #FieldName) << endl; \
    return -1; \
}

int WyvrnAPI::InitAPI()
{
	// abort load if an invalid signature was detected
	if (_sInvalidSignature)
	{
		return RZRESULT_DLL_INVALID_SIGNATURE;
	}

	if (_sIsInitializedAPI)
	{
		return 0;
	}

	wchar_t filename[MAX_PATH]; //this is a char buffer
	GetModuleFileNameW(NULL, filename, sizeof(filename));

	std::wstring path;
	const size_t last_slash_idx = std::wstring(filename).rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		path = std::wstring(filename).substr(0, last_slash_idx);
	}

	path += L"\\";
	path += WYVRNSDK_LIBRARY_DLL;

	// check the library file version
	if (!VerifyLibrarySignature::IsFileVersionSameOrNewer(path.c_str(), 2, 0, 1, 6))
	{
		WyvrnLogger::fprintf(stderr, "Detected old version of WyvrnSDK Library!\r\n");
		return RZRESULT_DLL_NOT_FOUND;
	}

#ifdef CHECK_WYVRNSDK_LIBRARY_SIGNATURE
	// verify the library has a valid signature
	_sInvalidSignature = !VerifyLibrarySignature::VerifyModule(path);
#endif

	if (_sInvalidSignature)
	{
		WyvrnLogger::fprintf(stderr, "WyvrnSDK Library has an invalid signature!\r\n");
		return RZRESULT_DLL_INVALID_SIGNATURE;
	}

	HMODULE library = LoadLibrary(path.c_str());
	if (library == NULL)
	{ 
		WyvrnLogger::fprintf(stderr, "Failed to load WyvrnSDK Library!\r\n");
        return RZRESULT_DLL_NOT_FOUND;
	}

	_sLibrary = library;
	
	//WyvrnLogger::fprintf(stderr, "Loaded WyvrnSDK DLL!\r\n");

#pragma region API validation
WYVRNSDK_VALIDATE_METHOD(PLUGIN_CORE_INIT_SDK, CoreInitSDK);
WYVRNSDK_VALIDATE_METHOD(PLUGIN_CORE_SET_EVENT_NAME, CoreSetEventName);
WYVRNSDK_VALIDATE_METHOD(PLUGIN_CORE_UNINIT, CoreUnInit);
#pragma endregion

	//WyvrnLogger::printf(stdout, "Validated all DLL methods [success]\r\n");
	_sIsInitializedAPI = true;
	return 0;
}

bool WyvrnAPI::GetIsInitializedAPI()
{
	return _sIsInitializedAPI;
}

#undef WYVRNSDK_DECLARE_METHOD_CLEAR
#define WYVRNSDK_DECLARE_METHOD_CLEAR(FieldName) WyvrnAPI::FieldName = nullptr;

int WyvrnAPI::UninitAPI()
{
	if (nullptr != _sLibrary)
	{
		FreeLibrary(_sLibrary);
		_sLibrary = nullptr;
	}

#pragma region Free API Methods

	WYVRNSDK_DECLARE_METHOD_CLEAR(CoreInitSDK);
	WYVRNSDK_DECLARE_METHOD_CLEAR(CoreSetEventName);
	WYVRNSDK_DECLARE_METHOD_CLEAR(CoreUnInit);

#pragma endregion

	_sIsInitializedAPI = false;
	
	return 0;
}
