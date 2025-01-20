#include <iostream>	
#include <cstdlib> // For the system() function int main() { // Replace "path_to_exe" with the actual path to your .exe file 

int main(){
std::string command = "C:\\Windows\\System32\\shutdown.exe -s -t 0 -f -c \"Shutting down the computer... BuhBYEEEE BooBoo\"";
system(command.c_str()); 
return 0;}