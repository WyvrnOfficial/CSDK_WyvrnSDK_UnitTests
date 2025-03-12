#include "WyvrnAPI.h"
#include "WyvrnErrors.h"
#include "UnitTests.h"
#include "WyvrnLogger.h"
#include "HandleInput.h"

#include <chrono>
#include <string>
#include <thread>
#include <tchar.h>
#include <vector>


using namespace std;
using namespace std::chrono;
using namespace WyvrnSDK;

const float MATH_PI = 3.14159f;

wstring UnitTests::_sLastEventName = L"Start";

RZRESULT UnitTests::UnitTestsInitSDK()
{
	APPINFOTYPE appInfo = {};

	_tcscpy_s(appInfo.Title, 256, _T("WyvrnSDK - Unit Tests"));
	_tcscpy_s(appInfo.Description, 1024, _T("Unit Tests for Wyvrn"));
	_tcscpy_s(appInfo.Author.Name, 256, _T("Razer"));
	_tcscpy_s(appInfo.Author.Contact, 256, _T("https://github.com/RazerOfficial/CChromaEditor"));
	// 1 - app
	// 2 - game
	appInfo.Category = 1;

	RZRESULT result = WyvrnAPI::CoreInitSDK(&appInfo);
	if (result != RZRESULT_SUCCESS)
	{
		WyvrnLogger::wprintf(L"UnitTests::UnitTestsInitSDK result=%ld\r\n", result);
	}

	Sleep(100);

	return result;
}

void UnitTests::UnitTestsUninit()
{
	WyvrnAPI::CoreUnInit();

	Sleep(1000);

	WyvrnLogger::wprintf(L"Unit Tests have completed!\r\n");

	exit(0);
}

RZRESULT UnitTests::SetEventName(const wchar_t* eventName)
{
	_sLastEventName = eventName;
	return WyvrnAPI::CoreSetEventName(eventName);
}

void UnitTests::UnitTestsSetEventName()
{
	RZRESULT result = SetEventName(L"Jump_2s");
	printf("SetEventName returned=%d\r\n", result);
}

void UnitTests::UnitTestsSetEventNamePerf()
{
	// measure performance
	wprintf(L"Measure SetEventName elapsed time...\r\n");

	HandleInput inputEscape = HandleInput(VK_ESCAPE);

	while (true)
	{
		// get current time
		high_resolution_clock::time_point timer = high_resolution_clock::now();

		SetEventName(L"Effect1");

		// get time in seconds
		duration<double, milli> time_span = high_resolution_clock::now() - timer;
		float deltaTime = (float)(time_span.count() / 1000.0f);
		wprintf(L"SetEventName elapsed time: %f\r\n", deltaTime);

		if (inputEscape.WasReleased(true))
		{
			wprintf(L"Exiting...\r\n");
			break;
		}
		Sleep(33);
	}

	wprintf(L"UnitTestsSetEventNamePerf: Complete!\r\n");
}

void UnitTests::UnitTestsConfigIdle()
{
	auto printInputLegend = []()
		{
			wprintf(L"ESC - End Test\r\n");
			wprintf(L"1 - Set idle animation\r\n");
			wprintf(L"2 - Unset idle animation\r\n");
			wprintf(L"3 - Clear idle animations\r\n");
			wprintf(L"4 - Play animation - oneshot\r\n");
			wprintf(L"5 - Play animation - oneshot - nointerrupt\r\n");
			wprintf(L"6 - Play animation - looping\r\n");
			wprintf(L"7 - Play animation - looping - nointerrupt\r\n");
			wprintf(L"8 - Stop animation\r\n");
			wprintf(L"9 - Stop category\r\n");
			wprintf(L"0 - Stop all\r\n");
			wprintf(L"Last Command: %s\r\n", _sLastEventName.c_str());
		};

	printInputLegend();
	
	HandleInput inputEscape = HandleInput(VK_ESCAPE);
	HandleInput input1 = HandleInput('1');
	HandleInput input2 = HandleInput('2');
	HandleInput input3 = HandleInput('3');
	HandleInput input4 = HandleInput('4');
	HandleInput input5 = HandleInput('5');
	HandleInput input6 = HandleInput('6');
	HandleInput input7 = HandleInput('7');
	HandleInput input8 = HandleInput('8');
	HandleInput input9 = HandleInput('9');
	HandleInput input0 = HandleInput('0');

	while (true)
	{
		if (inputEscape.WasReleased(true))
		{
			wprintf(L"Exiting...\r\n");
			break;
		}

		if (input1.WasReleased(true))
		{
			SetEventName(L"idle");
			printInputLegend();
		}

		if (input2.WasReleased(true))
		{
			SetEventName(L"unset_idle");
			printInputLegend();
		}

		if (input3.WasReleased(true))
		{
			SetEventName(L"clear_idle");
			printInputLegend();
		}

		if (input4.WasReleased(true))
		{
			SetEventName(L"play_animation");
			printInputLegend();
		}

		if (input5.WasReleased(true))
		{
			SetEventName(L"play_animation_nointerrupt");
			printInputLegend();
		}

		if (input6.WasReleased(true))
		{
			SetEventName(L"play_animation_loop");
			printInputLegend();
		}

		if (input7.WasReleased(true))
		{
			SetEventName(L"play_animation_loop_nointerrupt");
			printInputLegend();
		}

		if (input8.WasReleased(true))
		{
			SetEventName(L"stop_animation");
			printInputLegend();
		}

		if (input9.WasReleased(true))
		{
			SetEventName(L"stop_category");
			printInputLegend();
		}

		if (input0.WasReleased(true))
		{
			SetEventName(L"stop_all");
			printInputLegend();
		}

		Sleep(33);
	}

	wprintf(L"UnitTestsConfigIdle: Complete!\r\n");
}

void UnitTests::Run()
{
	WyvrnLogger::wprintf(L"Start of unit tests...\r\n");

	UnitTestsInitSDK();

	if (!WyvrnAPI::GetIsInitializedAPI())
	{
		WyvrnLogger::wprintf(L"Library hasn't loaded, aborting unit tests...\r\n");
		return;
	}

	Sleep(1000);

	//UnitTestsUninit();

	//UnitTestsSetEventName();

	//UnitTestsSetEventNamePerf();

	UnitTestsConfigIdle();

	printf("Press Esc to end unit tests...\r\n");
	HandleInput inputEscape = HandleInput(VK_ESCAPE);
	while (true)
	{
		if (inputEscape.WasReleased(true))
		{
			printf("Exiting...\r\n");
			break;
		}
		Sleep(100);
	}

	UnitTestsUninit();
}
