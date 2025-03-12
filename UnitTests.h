#pragma once

#include <string>

class UnitTests
{
public:

	static RZRESULT UnitTestsInitSDK();

	static void UnitTestsUninit();

	static RZRESULT SetEventName(const wchar_t* eventName);

	static void UnitTestsSetEventName();

	static void UnitTestsSetEventNamePerf();

	static void UnitTestsConfigIdle();

	static void Run();

private:

	static std::wstring _sLastEventName;
};
