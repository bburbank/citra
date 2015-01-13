// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.


#include "common/common.h"
#include "common/file_util.h"

#include "core/hle/hle.h"
#include "core/hle/kernel/event.h"
#include "core/hle/kernel/mutex.h"
#include "core/hle/kernel/shared_memory.h"
#include "core/hle/service/apt_s.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Namespace APT_S

namespace APT_U {
    extern void GetLockHandle(Service::Interface* self);
    extern void Initialize(Service::Interface* self);
    extern void Enable(Service::Interface* self);
    extern void InquireNotification(Service::Interface* self);
    extern void GetSharedFont(Service::Interface* self);
    extern void AppletUtility(Service::Interface* self);
    extern void GlanceParameter(Service::Interface* self);
    extern void ReceiveParameter(Service::Interface* self);
}

namespace APT_S {

const Interface::FunctionInfo FunctionTable[] = {
    {0x00010040, APT_U::GetLockHandle,         "GetLockHandle"},
    {0x00020080, APT_U::Initialize,            "Initialize"},
    {0x00030040, APT_U::Enable,                "Enable"},
    {0x00040040, nullptr,                      "Finalize"},
    {0x00050040, nullptr,                      "GetAppletManInfo"},
    {0x00060040, nullptr,                      "GetAppletInfo"},
    {0x00070000, nullptr,                      "GetLastSignaledAppletId"},
    {0x00080000, nullptr,                      "CountRegisteredApplet"},
    {0x00090040, nullptr,                      "IsRegistered"},
    {0x000A0040, nullptr,                      "GetAttribute"},
    {0x000B0040, APT_U::InquireNotification,   "InquireNotification"},
    {0x000C0104, nullptr,                      "SendParameter"},
    {0x000D0080, APT_U::ReceiveParameter,      "ReceiveParameter"},
    {0x000E0080, APT_U::GlanceParameter,       "GlanceParameter"},
    {0x000F0100, nullptr,                      "CancelParameter"},
    {0x001000C2, nullptr,                      "DebugFunc"},
    {0x001100C0, nullptr,                      "MapProgramIdForDebug"},
    {0x00120040, nullptr,                      "SetHomeMenuAppletIdForDebug"},
    {0x00130000, nullptr,                      "GetPreparationState"},
    {0x00140040, nullptr,                      "SetPreparationState"},
    {0x00150140, nullptr,                      "PrepareToStartApplication"},
    {0x00160040, nullptr,                      "PreloadLibraryApplet"},
    {0x00170040, nullptr,                      "FinishPreloadingLibraryApplet"},
    {0x00180040, nullptr,                      "PrepareToStartLibraryApplet"},
    {0x00190040, nullptr,                      "PrepareToStartSystemApplet"},
    {0x001A0000, nullptr,                      "PrepareToStartNewestHomeMenu"},
    {0x001B00C4, nullptr,                      "StartApplication"},
    {0x001C0000, nullptr,                      "WakeupApplication"},
    {0x001D0000, nullptr,                      "CancelApplication"},
    {0x001E0084, nullptr,                      "StartLibraryApplet"},
    {0x001F0084, nullptr,                      "StartSystemApplet"},
    {0x00200044, nullptr,                      "StartNewestHomeMenu"},
    {0x00210000, nullptr,                      "OrderToCloseApplication"},
    {0x00220040, nullptr,                      "PrepareToCloseApplication"},
    {0x00230040, nullptr,                      "PrepareToJumpToApplication"},
    {0x00240044, nullptr,                      "JumpToApplication"},
    {0x002500C0, nullptr,                      "PrepareToCloseLibraryApplet"},
    {0x00260000, nullptr,                      "PrepareToCloseSystemApplet"},
    {0x00270044, nullptr,                      "CloseApplication"},
    {0x00280044, nullptr,                      "CloseLibraryApplet"},
    {0x00290044, nullptr,                      "CloseSystemApplet"},
    {0x002A0000, nullptr,                      "OrderToCloseSystemApplet"},
    {0x002B0000, nullptr,                      "PrepareToJumpToHomeMenu"},
    {0x002C0044, nullptr,                      "JumpToHomeMenu"},
    {0x002D0000, nullptr,                      "PrepareToLeaveHomeMenu"},
    {0x002E0044, nullptr,                      "LeaveHomeMenu"},
    {0x002F0040, nullptr,                      "PrepareToLeaveResidentApplet"},
    {0x00300044, nullptr,                      "LeaveResidentApplet"},
    {0x00310100, nullptr,                      "PrepareToDoApplicationJump"},
    {0x00320084, nullptr,                      "DoApplicationJump"},
    {0x00330000, nullptr,                      "GetProgramIdOnApplicationJump"},
    {0x00340084, nullptr,                      "SendDeliverArg"},
    {0x00350080, nullptr,                      "ReceiveDeliverArg"},
    {0x00360040, nullptr,                      "LoadSysMenuArg"},
    {0x00370042, nullptr,                      "StoreSysMenuArg"},
    {0x00380040, nullptr,                      "PreloadResidentApplet"},
    {0x00390040, nullptr,                      "PrepareToStartResidentApplet"},
    {0x003A0044, nullptr,                      "StartResidentApplet"},
    {0x003B0040, nullptr,                      "CancelLibraryApplet"},
    {0x003C0042, nullptr,                      "SendDspSleep"},
    {0x003D0042, nullptr,                      "SendDspWakeUp"},
    {0x003E0080, nullptr,                      "ReplySleepQuery"},
    {0x003F0040, nullptr,                      "ReplySleepNotificationComplete"},
    {0x00400042, nullptr,                      "SendCaptureBufferInfo"},
    {0x00410040, nullptr,                      "ReceiveCaptureBufferInfo"},
    {0x00420080, nullptr,                      "SleepSystem"},
    {0x00430040, nullptr,                      "NotifyToWait"},
    {0x00440000, APT_U::GetSharedFont,         "GetSharedFont"},
    {0x00450040, nullptr,                      "GetWirelessRebootInfo"},
    {0x00460104, nullptr,                      "Wrap"},
    {0x00470104, nullptr,                      "Unwrap"},
    {0x00480100, nullptr,                      "GetProgramInfo"},
    {0x00490180, nullptr,                      "Reboot"},
    {0x004A0040, nullptr,                      "GetCaptureInfo"},
    {0x004B00C2, APT_U::AppletUtility,         "AppletUtility"},
    {0x004C0000, nullptr,                      "SetFatalErrDispMode"},
    {0x004D0080, nullptr,                      "GetAppletProgramInfo"},
    {0x004E0000, nullptr,                      "HardwareResetAsync"},
    {0x004F0080, nullptr,                      "SetApplicationCpuTimeLimit"},
    {0x00500040, nullptr,                      "GetApplicationCpuTimeLimit"},
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface class

Interface::Interface() {
    Register(FunctionTable, ARRAY_SIZE(FunctionTable));
}

} // namespace