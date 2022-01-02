#include <iostream>
#include <cstring>
#include <Windows.h>
#include <ShlObj.h>
#include <tchar.h>
#pragma comment(lib, "urlmon.lib")
#define menu_title "C++ File Downloader"

void server_downloader(std::string file_url, std::string file_path) {
	HRESULT download_operation = URLDownloadToFile(nullptr, file_url.c_str(), file_path.c_str(), 0, nullptr);
	if (SUCCEEDED(download_operation)) {
		MessageBox(nullptr, "File successfully downloaded.", menu_title, MB_OK);
	}
	else {
		MessageBox(nullptr, "An error has occured while downloading the file.", menu_title, MB_OK);
	}
}

static std::string folder_selection()
{
	TCHAR size_buffer[MAX_PATH] = { 0 };
	BROWSEINFO browse_info;
	ZeroMemory(&browse_info, sizeof(BROWSEINFO));
	browse_info.hwndOwner = GetForegroundWindow();
	browse_info.pszDisplayName = size_buffer;
	browse_info.pidlRoot = nullptr;
	browse_info.lpszTitle = _T("Select the installation folder:");
	browse_info.ulFlags = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST IDL = SHBrowseForFolder(&browse_info);
	if (IDL == nullptr)
	{
		static char fallback[MAX_PATH + 1];
		if (SHGetSpecialFolderPathA(HWND_DESKTOP, fallback, CSIDL_DESKTOP, FALSE)) {
			return fallback;
		}
	}
	else {
		SHGetPathFromIDList(IDL, size_buffer);
	}
	return size_buffer;
}

void head_print() {
	system("CLS");
	CONSOLE_SCREEN_BUFFER_INFO CON_SCR_BUF_INF;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CON_SCR_BUF_INF);
	for (int i = 0; i < (((CON_SCR_BUF_INF.srWindow.Right - CON_SCR_BUF_INF.srWindow.Left + 1) - strlen(menu_title)) / 2); i++) {
		std::cout << " ";
	}
	std::cout << "C++ File Downloader" << std::endl;
	for (int i = 0; i < (CON_SCR_BUF_INF.srWindow.Right - CON_SCR_BUF_INF.srWindow.Left + 1); i++) {
		std::cout << "*";
	}
	std::cout << std::endl << std::endl << std::endl;
}

void user_input() {
	std::string file_url = { 0 };
	std::string file_name = { 0 };
	head_print();
	std::cout << "URL: ";
	std::cin >> file_url;
	std::cout << std::endl << "Filename (incl. ext): ";
	std::cin >> file_name;
	std::cout << std::endl << "Destination? (Default: Desktop)";
	server_downloader(file_url, folder_selection() + "\\" + file_name);
}

int main() {
	unsigned short int user_selection = MessageBox(nullptr, "Would You like to continue?", menu_title, MB_YESNO);
	if (user_selection == IDYES) {
		user_input();
	}
	return EXIT_SUCCESS;
}
