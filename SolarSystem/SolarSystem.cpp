#include "SolarSystem.h"


DWORD WINAPI SS::SolarSystem::SolarSystemThread(LPVOID lpParam)
{
	MSG msg;
	SS::SolarSystem::SolarSystemObject inst = *(SS::SolarSystem::SolarSystemObject*)(lpParam);
	while (1)
	{
		WaitForSingleObject(inst.instance->hEventStop, INFINITE);
		WaitForSingleObject(inst.instance->hSemaphoreRecalc, INFINITE);
		inst.instance->theSystem[inst.name].Next();
		PostThreadMessage(inst.instance->SyncThreadId, WM_RECALC_OK, 0, 0);

	}
	return 0;
}

DWORD WINAPI SS::SolarSystem::SolarSystemThreadSync(LPVOID lpParam)
{
	MSG msg;
	bool fl = 0;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	SS::SolarSystem::SolarSystemObject inst = *(SS::SolarSystem::SolarSystemObject*)(lpParam);
	while (1)
	{
		Sleep(60);
		if (fl == 1)
		{
			Sleep(2000);
			SetEvent(inst.instance->hEventStop);
			fl = 0;
		}
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
			if (msg.message == WM_STOP)
			{
				fl = 1;
				ResetEvent(inst.instance->hEventStop);
			}
				
		}
		PostMessage(inst.instance->hWindow, WM_REPAINT, 0, 0);
	}
	return 0;
}

SS::SolarSystem::SolarSystem()
{
	this->hWindow = NULL;
	this->isometricRatio = 1;
	this->sizeRatio = this->speedRatio = this->distanceRatio = 1;
}

SS::SolarSystem::SolarSystem(HWND hWindow, float isometricRatio, float sizeRatio, float speedRatio, float distanceRatio)
{
	this->hWindow = hWindow;
	this->isometricRatio = isometricRatio;
	this->sizeRatio = sizeRatio;
	this->speedRatio = speedRatio;
	this->distanceRatio = distanceRatio;
}

void SS::SolarSystem::Add(std::string name, std::string nameFocusBody,
	int apoapsis, int periapsis, int a, float e, int speed, int radius, float t0, HBITMAP hBmp)
{
	SS::SolarSystem::SolarSystemObject object(*this, name, nameFocusBody, 
		apoapsis, periapsis, a, e, speed, radius, t0, hBmp);
	this->Add(name, object);
}

void SS::SolarSystem::Add(std::string name, std::string nameFocusBody,
	int apoapsis, int periapsis, int a, float e, int speed, int radius, float t0, std::vector<HBITMAP> hBmps)
{
	SS::SolarSystem::SolarSystemObject object(*this, name, nameFocusBody,
		apoapsis, periapsis, a, e, speed, radius, t0, hBmps);
	this->Add(name, object);
}

void SS::SolarSystem::Add(std::string name, SS::SolarSystem::SolarSystemObject object)
{
	SS::SolarSystem::theSystem[name] = object;
	theSystem[name].Settings(this->speedRatio, this->sizeRatio, this->distanceRatio, this->isometricRatio);
	if (theSystem.size() == 1)
	{
		theSystem[name].hThread = CreateThread(0, 0, SS::SolarSystem::SolarSystemThreadSync,
				&theSystem[name], CREATE_SUSPENDED, 0);
		this->SyncThreadId = GetThreadId(theSystem[name].hThread);
		this->curr = theSystem.begin();
	}
	else
		theSystem[name].hThread = CreateThread(0, 0, SS::SolarSystem::SolarSystemThread, 
				&theSystem[name], CREATE_SUSPENDED, 0);
}


SS::SolarSystem::SolarSystemObject* SS::SolarSystem::Get(std::string name)
{
	return &(this->theSystem[name]);
}

SS::SolarSystem::SolarSystemObject* SS::SolarSystem::Get()
{
	if (this->curr == this->theSystem.end())
	{
		this->curr = theSystem.begin();
		return NULL;
	}
	return &((*this->curr++).second);
}

void SS::SolarSystem::Go()
{
	this->hSemaphoreRecalc = CreateSemaphore(NULL, 0, this->theSystem.size() - 1, TEXT("SemaphoreForRecalc"));
	this->hEventStop = CreateEvent(NULL, true, true, TEXT("EventStop"));
	std::map<std::string, SS::SolarSystem::SolarSystemObject>::iterator it = this->theSystem.begin(), it_end = this->theSystem.end();
	for (; it != it_end; ++it)
	{
		ResumeThread(it->second.hThread);
	}
}

void SS::SolarSystem::SpeedInc(float ratioInc)
{
	this->speedRatio += ratioInc;
	std::map<std::string, SS::SolarSystem::SolarSystemObject>::iterator it = this->theSystem.begin(), it_end = this->theSystem.end();
	for (; it != it_end; ++it)
	{
		//it->second.SetSpeedRatio(it->second.GetSpeedRatio() + ratioInc);
		it->second.SetSpeedRatio(this->speedRatio);
	}
}

void SS::SolarSystem::SizeInc(float ratioInc)
{
	this->sizeRatio += ratioInc;
	std::map<std::string, SS::SolarSystem::SolarSystemObject>::iterator it = this->theSystem.begin(), it_end = this->theSystem.end();
	for (; it != it_end; ++it)
	{
		it->second.SetSizeRatio(this->sizeRatio);
	}
}

void SS::SolarSystem::DistanceInc(float ratioInc)
{
	this->distanceRatio += ratioInc;
	std::map<std::string, SS::SolarSystem::SolarSystemObject>::iterator it = this->theSystem.begin(), it_end = this->theSystem.end();
	for (; it != it_end; ++it)
	{
		it->second.SetDistanceRatio(this->distanceRatio);
	}
}

void SS::SolarSystem::IsometricAngleInc(float ratioInc)
{
	this->isometricRatio += ratioInc;
	std::map<std::string, SS::SolarSystem::SolarSystemObject>::iterator it = this->theSystem.begin(), it_end = this->theSystem.end();
	for (; it != it_end; ++it)
	{
		it->second.SetIsometricRatio(this->isometricRatio);
	}
}

void SS::SolarSystem::Stop()
{
	PostThreadMessage(this->SyncThreadId, WM_STOP, 0, 0);
}

float SS::SolarSystem::GetSpeedRatio()
{
	return this->speedRatio;
}

float SS::SolarSystem::GetSizeRatio()
{
	return this->sizeRatio;
}

float SS::SolarSystem::GetDistanceRatio()
{
	return this->distanceRatio;
}

float SS::SolarSystem::GetIsometricRatio()
{
	return this->isometricRatio;
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

SS::SolarSystem::SolarSystemObject::SolarSystemObject(SS::SolarSystem &instance, std::string name,
	std::string nameFocusBody, int apoapsis, int periapsis, int a, float e, int speed, int radius, float t0, HBITMAP hBmp)
	:SSO::SolarSystemObject(apoapsis, periapsis, a, e, speed, radius, 0)
{
		this->Settings(instance.speedRatio, instance.sizeRatio, instance.distanceRatio, instance.isometricRatio);
		this->instance = &instance;
		this->name = name;
		this->nameFocusBody = nameFocusBody;
		this->curr = 0;
		this->AddHBmp(hBmp);
}

SS::SolarSystem::SolarSystemObject::SolarSystemObject(SS::SolarSystem &instance, std::string name, 
	std::string nameFocusBody, int apoapsis, int periapsis, int a, float e, int speed, int radius, float t0, std::vector<HBITMAP> hBmps)
	:SSO::SolarSystemObject(apoapsis, periapsis, a, e, speed, radius, 0)
{
	this->Settings(instance.speedRatio, instance.sizeRatio, instance.distanceRatio, instance.isometricRatio);
	this->instance = &instance;
	this->name = name;
	this->nameFocusBody = nameFocusBody;
	this->curr = 0;
	std::vector<HBITMAP>::iterator itB = hBmps.begin(), itE = hBmps.end();
	for (; itB != itE; ++itB)
		this->AddHBmp(*itB);
}

int SS::SolarSystem::SolarSystemObject::GetX() const
{
	return this->SSO::SolarSystemObject::GetX() +
		this->instance->theSystem[this->nameFocusBody].SSO::SolarSystemObject::GetX() - 
		this->SSO::SolarSystemObject::GetRadius()/2;
}

int SS::SolarSystem::SolarSystemObject::GetY() const
{
	return this->SSO::SolarSystemObject::GetY() +
		this->instance->theSystem[this->nameFocusBody].SSO::SolarSystemObject::GetY() -
		this->SSO::SolarSystemObject::GetRadius() / 2;
}

int SS::SolarSystem::SolarSystemObject::GetA() const
{
	return this->instance->theSystem[this->nameFocusBody].SSO::SolarSystemObject::GetX() -
		this->SSO::SolarSystemObject::GetA();
}

int SS::SolarSystem::SolarSystemObject::GetB() const
{
	return this->instance->theSystem[this->nameFocusBody].SSO::SolarSystemObject::GetY() -
		this->SSO::SolarSystemObject::GetB();
		
}

HANDLE SS::SolarSystem::SolarSystemObject::GetHThread() const
{
	return this->hThread;
}

void SS::SolarSystem::SolarSystemObject::SetHThread(HANDLE hThread)
{
	this->hThread = hThread;
}

void SS::SolarSystem::SolarSystemObject::AddHBmp(HBITMAP hBmp)
{
	HDC hTempDC = GetDC(this->instance->hWindow);
	this->hBmps.push_back(std::pair<HBITMAP, HDC>(hBmp, CreateCompatibleDC(hTempDC)));
	HBITMAP hBmpTemp = (HBITMAP)SelectObject(this->hBmps.back().second, this->hBmps.back().first);
	if (hBmpTemp) DeleteObject(hBmpTemp);
}

HDC SS::SolarSystem::SolarSystemObject::GetHDC()
{
	if (this->curr == this->hBmps.size()-1)
		this->curr = 0;
	else
		this->curr++;
	return this->hBmps.at(this->curr).second;
}

HBITMAP SS::SolarSystem::SolarSystemObject::GetHBmp() const
{
	return this->hBmps.at(this->curr).first;
}

int SS::SolarSystem::SolarSystemObject::GetBmpWidth() const
{
	BITMAP bmp;
	GetObject(this->hBmps.at(this->curr).first, sizeof(bmp), &bmp);
	return bmp.bmWidth;
}

int SS::SolarSystem::SolarSystemObject::GetBmpHeight() const
{
	BITMAP bmp;
	GetObject(this->hBmps.at(this->curr).first, sizeof(bmp), &bmp);
	return bmp.bmHeight;
}

//SS::SolarSystem::SolarSystemObject::~SolarSystemObject()
//{
//
//}