#include <iostream>
#include <filesystem>
#include<windows.h>
#include<fstream>
#include"File.hpp"
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
char* WcharToChar(WCHAR* Name) {
	int k = 0;
	char* Ans = (char*)malloc(k * sizeof(char));
	for (int i = 0; i < MAX_PATH; i++) {
		k++;
		Ans = (char*)realloc(Ans, k * sizeof(char));
		Ans[k - 1] = (char)Name[i];
	}
	k++;
	Ans[k - 1] = '\0';
	return Ans;
}
int main()
{
	std::cout << "Write Directory\n";
	WIN32_FIND_DATA FindFileData;
	std::string Road;
	std::cin >> Road;
	std::string RoadForFile = (Road) + "***.?**";
	int Size = 1;
	File Arr[1000];
	HANDLE hf = FindFirstFile(s2ws(RoadForFile).c_str(), &FindFileData);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::string File1 = static_cast<std::string>(WcharToChar(FindFileData.cFileName));
			if (File1 != "." && File1 != "..") {
				Arr[Size - 1].FileName =Road+File1;
				Arr[Size - 1].OnlyName = File1;
				Arr[Size - 1].KbSize = (static_cast<int>(FindFileData.nFileSizeHigh + FindFileData.nFileSizeLow) / 1024);
				Size++;
				//std::cout << FindFileData.nFileSizeHigh << " " << FindFileData.nFileSizeLow << " "<<(static_cast<int>(FindFileData.nFileSizeHigh) + static_cast<int>(FindFileData.nFileSizeLow)) / 1024 <<" ";
				//wprintf(L"%s\n", FindFileData.cFileName);
			}
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
	for (int i = 0; i < Size-1; i++) {
		for (int j = 0; j < Size - 1; j++) {
			if (Arr[j].KbSize < Arr[j + 1].KbSize) {
				File Copy = Arr[j];
				Arr[j] = Arr[j + 1];
				Arr[j + 1] = Copy;
			}
		}
	}
	std::cout << "Write new Directory\n";
	std::string NewRoad;
	std::cin >> NewRoad;
	int MaxKbSize = 100;
	int j = 0;
	std::string count = "1";
	for (int i = 0; i < Size; i++) {
		MaxKbSize -= Arr[i].KbSize;
		if (MaxKbSize <= 0 || i==Size-1) {

			std::string DirectoryName = "Directory" + count;
			std::string RoadToDirectory = NewRoad + DirectoryName;
			CreateDirectory(s2ws(RoadToDirectory).c_str(), NULL);
			if (i - j != 0) {
				i--;
			}
			for (j; j <= i; j++) {
				std::string NewFileName = RoadToDirectory + "\\" + Arr[j].OnlyName;
				MoveFile(s2ws(Arr[j].FileName).c_str(), s2ws(NewFileName).c_str());
			}
			j = i+1;
			if (j >= Size-1) return 0;
			count+="1";
			MaxKbSize = 100;
		}
	}
	return 0;
}