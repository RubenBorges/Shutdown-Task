import std;
#include <cstdlib> // For the system() function int main() { // Replace "path_to_exe" with the actual path to your .exe file 

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
		return 0;
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