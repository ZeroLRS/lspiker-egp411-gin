#pragma once

#include <wtypes.h>

class Event;
class TransactionHandler;

extern "C"
{
	__declspec(dllexport) void drawCardRequest(const Event& theEvent, TransactionHandler* pHandler);
	__declspec(dllexport) void discardCardRequest(const Event& theEvent, TransactionHandler* pHandler);
	__declspec(dllexport) void passCardRequest(const Event& theEvent, TransactionHandler* pHandler);
	__declspec(dllexport) void cardPlayedNotification(const Event& theEvent, TransactionHandler* pHandler);
	__declspec(dllexport) void cardDrawnNotification(const Event& theEvent, TransactionHandler* pHandler);
	__declspec(dllexport) void cardPassedNotification(const Event& theEvent, TransactionHandler* pHandler);
	__declspec(dllexport) void generalGameNotification(const Event& theEvent, TransactionHandler* pHandler);
}
