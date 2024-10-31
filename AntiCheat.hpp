//By AlSch092 @github
#pragma once

#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#include "Detections.hpp"
#include "Preventions.hpp"
#include "Common/Logger.hpp"
#include "Common/Settings.hpp"

/*
	The `AntiCheat` class is a container for the necessary classes of our program, including the monitor, barrier, netclient, and anti-debugger
*/
class AntiCheat
{
public:

	AntiCheat(Settings* config, WindowsVersion WinVersion) : Config(config), WinVersion(WinVersion)
	{		
		if (config == nullptr)
		{
			Logger::logf("UltimateAnticheat.log", Err, "Settings pointer was NULL @ AntiCheat::AntiCheat");
			return;
		}
		
		try
		{
			this->NetworkClient = std::make_shared<NetClient>();

			this->_AntiDebugger = std::make_unique<Debugger::AntiDebug>(config, NetworkClient); //any detection methods need the netclient for comms

			this->Monitor = std::make_unique<Detections>(config, false, NetworkClient, UnmanagedGlobals::ModulesAtStartup);

			this->Barrier = std::make_unique<Preventions>(config, true, Monitor.get()->GetIntegrityChecker()); //true = prevent new threads from being made
		}
		catch (const std::bad_alloc& e)
		{
			Logger::logf("UltimateAnticheat.log", Err, "Critical allocation failure in AntiCheat::AntiCheat: %s", e.what());
			std::terminate();  //do not allow proceed if any pointers fail to alloc
		}
	}

	~AntiCheat() //the destructor is now empty since all pointers of this class were recently switched to unique_ptrs
	{
	}

	AntiCheat& operator=(AntiCheat&& other) = delete; //delete move assignments

	AntiCheat operator+(AntiCheat& other) = delete; //delete all arithmetic operators, unnecessary for context
	AntiCheat operator-(AntiCheat& other) = delete;
	AntiCheat operator*(AntiCheat& other) = delete;
	AntiCheat operator/(AntiCheat& other) = delete;

	Debugger::AntiDebug* GetAntiDebugger() const { return this->_AntiDebugger.get(); }
	
	NetClient* GetNetworkClient() const  { return this->NetworkClient.get(); }
	
	Preventions* GetBarrier() const  { return this->Barrier.get(); }  //pointer lifetime stays within the Anticheat class, these 'Get' functions should only be used to call functions of these classes
	
	Detections* GetMonitor() const { return this->Monitor.get(); }

	Settings* GetConfiguration() const { return this->Config; }

	__forceinline bool IsAnyThreadSuspended();

private:

	unique_ptr<Detections> Monitor;  //cheat detections

	unique_ptr<Preventions> Barrier;  //cheat preventions

	unique_ptr<Debugger::AntiDebug> _AntiDebugger;

	shared_ptr <NetClient> NetworkClient; //for client-server comms, our other classes need access to this to send detected flags to the server
	
	Settings* Config = nullptr; //the unique_ptr for this is made in main.cpp

	WindowsVersion WinVersion;
};

/*
	IsAnyThreadSuspended - Checks the looping threads of class members to ensure the program is running as normal. An attacker may try to suspend threads to either remap or disable functionalities
	returns true if any thread is found suspended
*/
__forceinline bool AntiCheat::IsAnyThreadSuspended()
{
	if (Monitor->GetMonitorThread()!= nullptr && Thread::IsThreadSuspended(Monitor->GetMonitorThread()->GetHandle()))
	{
		Logger::logf("UltimateAnticheat.log", Detection, "Monitor was found suspended! Abnormal program execution.");
		return true;
	}
	else if (Config->bUseAntiDebugging && Thread::IsThreadSuspended(_AntiDebugger->GetDetectionThreadHandle()))
	{
		Logger::logf("UltimateAnticheat.log", Detection, "Anti-debugger was found suspended! Abnormal program execution.");
		return true;
	}
	else if (NetworkClient->GetRecvThread() != nullptr && Thread::IsThreadSuspended(NetworkClient->GetRecvThread()->GetHandle()))
	{
		Logger::logf("UltimateAnticheat.log", Detection, "Netclient comms thread was found suspended! Abnormal program execution.");
		return true;
	}

	return false;
}