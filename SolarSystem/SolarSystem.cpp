#include "SolarSystem.h"


DWORD WINAPI SS::SolarSystem::SolarSystemThread(LPVOID lpParam)
{
	MSG msg;
	SS::SolarSystem::SolarSystemObject inst = *(SS::SolarSystem::SolarSystemObject*)(lpParam);
	while (1)
	{
		WaitForSingleObject(inst.instance->hSemaphoreRecalc, INFINITE);
		inst.instance->theSystem[inst.name].Next();
		PostThreadMessage(inst.instance->SyncThreadId, WM_RECALC_OK, 0, 0);
	}
	return 0;
}

DWORD WINAPI SS::SolarSystem::SolarSystemThreadSync(LPVOID lpParam)
{
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	SS::SolarSystem::SolarSystemObject inst = *(SS::SolarSystem::SolarSystemObject*)(lpParam);
	while (1)
	{
		Sleep(60);
		ReleaseSemaphore(inst.instance->hSemaphoreRecalc, inst.instance->theSystem.size() - 1, NULL);
		inst.instance->theSystem[inst.name].Next();
		int i = 0;
		while (1)
		{
			if (i >= inst.instance->theSystem.size() - 1)
			{
				i = 0;
				break;
			}
			GetMessage(&msg, 0, 0, 0);
			if (msg.message == WM_RECALC_OK)
				i++;
		}
		PostMessage(inst.instance->hWindow, WM_REPAINT, 0, 0);
	}
	return 0;
}

SS::SolarSystem::SolarSystem()
{
	this->hWindow = NULL;
	this->isometric = false;
	this->sizeRatio = this->speedRatio = this->distanceRatio = 1;
}

SS::SolarSystem::SolarSystem(HWND hWindow, bool isometric, float sizeRatio, float speedRatio, float distanceRatio)
{
	this->hWindow = hWindow;
	this->isometric = isometric;
	this->sizeRatio = sizeRatio;
	this->speedRatio = speedRatio;
	this->distanceRatio = distanceRatio;
}

void SS::SolarSystem::Add(std::string name, std::string nameFocusBody,
	int apoapsis, int periapsis, int a, float e, int speed, int radius, float t0, HBITMAP hBmp)
{
	SS::SolarSystem::SolarSystemObject object(*this, this->hWindow, name, nameFocusBody, 
		apoapsis, periapsis, a, e, speed, radius, t0, hBmp);
	this->Add(name, object);
}

void SS::SolarSystem::Add(std::string name, SS::SolarSystem::SolarSystemObject object)
{
	SS::SolarSystem::theSystem[name] = object;

	theSystem[name].Settings(this->speedRatio, this->sizeRatio, this->distanceRatio, this->isometric);
	if (theSystem.size() == 1)
	{
		theSystem[name].hThread = CreateThread(0, 0, SS::SolarSystem::SolarSystemThreadSync,
				&theSystem[name], CREATE_SUSPENDED, 0);
		this->SyncThreadId = GetThreadId(theSystem[name].hThread);
	}
		
	else
		theSystem[name].hThread = CreateThread(0, 0, SS::SolarSystem::SolarSystemThread, 
				&theSystem[name], CREATE_SUSPENDED, 0);
}

void SS::SolarSystem::Go()
{
	this->hSemaphoreRecalc = CreateSemaphore(NULL, 0, this->theSystem.size() - 1, TEXT("SemaphoreForRecalc"));
	std::map<std::string, SS::SolarSystem::SolarSystemObject>::iterator it = this->theSystem.begin(), it_end = this->theSystem.end();
	for (; it != it_end; ++it)
	{
		ResumeThread(it->second.hThread);
	}
}

SS::SolarSystem::~SolarSystem()
{
	std::map<std::string, SS::SolarSystem::SolarSystemObject>::iterator it = theSystem.begin(), it_end = theSystem.end();
	for (; it != it_end; ++it)
	{
		PostThreadMessage(GetThreadId(it->second.hThread), WM_EXIT_THREAD, 0, 0);
	}
	it = theSystem.begin(), it_end = theSystem.end();
	for (; it != it_end; ++it)
	{
		it->second.~SolarSystemObject();
	}

}

SS::SolarSystem::SolarSystemObject::SolarSystemObject(SS::SolarSystem &instance, HWND hWindow, std::string name, 
	std::string nameFocusBody, int apoapsis, int periapsis, int a, float e, int speed, int radius, float t0, HBITMAP hBmp)
	:SSO::SolarSystemObject(apoapsis, periapsis, a, e, speed, radius, 0)
{
	this->Settings(instance.speedRatio, instance.sizeRatio, instance.distanceRatio, instance.isometric);
	this->instance = &instance;
	this->name = name;
	this->nameFocusBody = nameFocusBody;
	this->hBmp = hBmp;
	HDC hTempDC = GetDC(instance.hWindow);
	this->hObjDC = CreateCompatibleDC(hTempDC);
}

HANDLE SS::SolarSystem::SolarSystemObject::GetHThread()
{
	return this->hThread;
}

void SS::SolarSystem::SolarSystemObject::SetHThread(HANDLE hThread)
{
	this->hThread = hThread;
}

//SS::SolarSystem::SolarSystemObject::~SolarSystemObject()
//{
//
//}