#pragma once
#include "AntiDebugger.hpp"

class DebuggerDetections : public Debugger::AntiDebug
{
public:
    DebuggerDetections(Settings* s, std::shared_ptr<NetClient> netClient) : Debugger::AntiDebug(s, netClient) 
    {
        AddDetectionFunction([this]() -> bool { return _IsDebuggerPresent(); });
        AddDetectionFunction([this]() -> bool { return _IsDebuggerPresent_HeapFlags(); });
        AddDetectionFunction([this]() -> bool { return _IsDebuggerPresent_CloseHandle(); });
        AddDetectionFunction([this]() -> bool { return _IsDebuggerPresent_RemoteDebugger(); });

        AddDetectionFunction([this]() -> bool { return _IsDebuggerPresent_VEH(); });
        AddDetectionFunction([this]() -> bool { return _IsDebuggerPresent_DbgBreak(); });
        AddDetectionFunction([this]() -> bool { return _IsDebuggerPresent_PEB(); });

        AddDetectionFunction([this]() -> bool { return _IsDebuggerPresent_DebugPort(); });
        AddDetectionFunction([this]() -> bool { return _IsDebuggerPresent_ProcessDebugFlags(); });
        AddDetectionFunction([this]() -> bool { return _IsKernelDebuggerPresent(); });
        AddDetectionFunction([this]() -> bool { return _IsKernelDebuggerPresent_SharedKData(); });
        //AddDetectionFunction([this]() { return _IsHardwareDebuggerPresent(this->); });  //will add this back in soon
    }

    bool _IsDebuggerPresent() { return IsDebuggerPresent(); }
    bool _IsDebuggerPresent_HeapFlags();
    bool _IsDebuggerPresent_CloseHandle();
    bool _IsDebuggerPresent_RemoteDebugger();
    bool _IsDebuggerPresent_VEH();
    bool _IsDebuggerPresent_DbgBreak();
    bool _IsDebuggerPresent_PEB();
    bool _IsDebuggerPresent_DebugPort();
    bool _IsDebuggerPresent_ProcessDebugFlags();
    bool _IsKernelDebuggerPresent();
    bool _IsKernelDebuggerPresent_SharedKData();
    void _IsHardwareDebuggerPresent(LPVOID AD); //we need this particular routine threaded due to requirement of suspending threads to properly fetching thread context, if this is not threaded it means one thread (the anti-debugger one) will be lacking DR checks
};
