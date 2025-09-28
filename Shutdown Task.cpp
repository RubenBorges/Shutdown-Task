import std;
#include <cstdlib> // For the system() function int main() { // Replace "path_to_exe" with the actual path to your .exe file 
// ForcedShutdown.cpp : Forces Windows to shutdown/reboot/logoff.
// Usage:
//   ForcedShutdown.exe            -> Force shutdown (power off)
//   ForcedShutdown.exe /reboot    -> Force reboot
//   ForcedShutdown.exe /logoff    -> Force logoff
//   ForcedShutdown.exe /hibernate -> Hibernate (not forced)
// Requires: Administrator (SeShutdownPrivilege) for shutdown/reboot/logoff.

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#include <windows.h>

// Runtime-loaded hibernate function signature (avoid compile-time dependency on powrprof.h)
typedef BOOLEAN(WINAPI* PSetSuspendState)(BOOLEAN, BOOLEAN, BOOLEAN);

bool EnablePrivilege(LPCTSTR privName)
{
	HANDLE hToken = nullptr;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		std::cerr << "OpenProcessToken failed: " << GetLastError() << "\n";
		return false;
	}

	TOKEN_PRIVILEGES tp{};
	LUID luid;
	if (!LookupPrivilegeValue(nullptr, privName, &luid))
	{
		std::cerr << "LookupPrivilegeValue failed: " << GetLastError() << "\n";
		CloseHandle(hToken);
		return false;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), nullptr, nullptr))
	{
		std::cerr << "AdjustTokenPrivileges failed: " << GetLastError() << "\n";
		CloseHandle(hToken);
		return false;
	}
	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		std::cerr << "Privilege not assigned. Run as Administrator.\n";
		CloseHandle(hToken);
		return false;
	}

	CloseHandle(hToken);
	return true;
}

int ForceAction(UINT flags, DWORD reason)
{
	if (!EnablePrivilege(SE_SHUTDOWN_NAME))
		return 1;

	if (!ExitWindowsEx(flags, reason))
	{
		std::cerr << "ExitWindowsEx failed: " << GetLastError() << "\n";
		return 2;
	}
	return 0;
}
void SetCursorPosandClear() {
	std::cout << "\033[7;1H";
	std::cout << "\033[2K";
	std::cout << "\033[7;1H";
};
void SetCursorPosandMark() {
	std::cout << "\033[7;1HCommand: ";
	std::cout << "\033[2K";
	std::cout << "\033[7;1HCommand: ";
};
void ExitAnimation(const char* message) {
	std::string dots = ".";
	for (int i = 0; i < 4; i++) {
		SetCursorPosandClear();
		std::print("{0}", message);
		std::cout << dots;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		dots += ".";
		if (dots.length() > 4) dots = ".";
	};
};
void drawHeader() {
    std::cout << "\033[1;1H\033[44mPC KiLLSWiTCh\033[0m"; // Move to top-left, blue background
}

void drawFooter() {
    std::cout << "\033[20;1H\033[42mProgram Written by: R.M. Borges\033[0m"; // Move to row 20, green background
}

void drawMainContent() {
    std::cout << "\033[5;1HTo Shutdown PC enter 'Yes'/'Y' or 'Okay'/'K'...";

}
int ProgramLogic(std::string& userInput, std::string& selection) {
	DWORD reason = SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED;
	SetCursorPosandMark();
	std::cin >> userInput; //MAKESHIFT Switch!
	for (auto& c : userInput) selection += std::toupper(c);
	if (selection == "Q" || selection == "Quit"
		|| selection == "C" || selection == "Cancel"
		|| selection == "E" || selection == "Exit") {
		std::println("CANCELLING the shutdown process.");
		SetCursorPosandClear();
		ExitAnimation("Exiting Program");
		system("cls"); // Windows-specific
		return 1;
	}
	else if (selection == "Y" || selection == "YES"
		|| selection == "K" || selection == "OK"
		|| selection == "S" || selection == "Shutdown") {
		SetCursorPosandClear();
		std::string command = "C:\\Windows\\System32\\shutdown.exe -s -t 0 -f -c";
		ExitAnimation("Shutting down the computer... BuhBYEEEE BooBoo");
		system(command.c_str());
		system("cls"); // Windows-specific
		return ForceAction(EWX_POWEROFF | EWX_FORCEIFHUNG, reason);
	}
	else {
		SetCursorPosandClear();
		selection.clear();
		std::cout << "\nInvalid input. \n\nSHUTDOWN PC: Yes(Y)/OK(K)/Shutdown(S)\n\nEXIT PROGRAM: Quit(Q)/Exit(E)/Cancel(C)" << std::endl;
	}
};
	
int main() {
	std::cout << "\033[2J"; // Clear screen
	std::string userInput, selection;
	int result{ -1 }, counter{ 0 };
	while (true) {
		counter++;
		drawHeader();
		drawMainContent();
		drawFooter();
		result = ProgramLogic(userInput, selection);
		if (result >= 0) break;
		if (counter >= 5) {
			SetCursorPosandClear;
			std::println("Too many invalid attempts.Exiting program.");
			ExitAnimation("Exiting Program");
			system("cls"); // Windows-specific
			return 1;
		}

	}	
	return 0;
}