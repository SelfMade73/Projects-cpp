#include <windows.h>
#include <iostream>
#include <string>
#include <chrono>

std::wstring destination_path = L"";// where to copy

void Recursive_search(std::wstring path)//Recursive traverse of directory
{
    WIN32_FIND_DATAW FindData;//struct for file data
    HANDLE hFile = INVALID_HANDLE_VALUE;
    hFile = FindFirstFileW(path.c_str(),&FindData);
    if (hFile == INVALID_HANDLE_VALUE)
        return;
    while (FindNextFileW(hFile,&FindData) != 0)
    {
        std::wstring temp_str = FindData.cFileName;
        if (temp_str.length() == 1 && temp_str.find('.') !=std::wstring::npos)
            if(FindNextFileW(hFile,&FindData) == 0)
                break;
        if (temp_str.length() == 2 && temp_str.find(L"..") != std::wstring::npos)
            if(FindNextFileW(hFile,&FindData) == 0)
                break;
        if (FindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)//is directory
             Recursive_search(path.substr(0,path.length()-1) + FindData.cFileName + L"\\*");
        else
        {
            std::wcout << FindData.cFileName << std::endl;
            CopyFileW((path.substr(0,path.length()-1) + FindData.cFileName).c_str() ,(destination_path +  FindData.cFileName).c_str() ,false);
        }
    }
}
void ConnectionControl()
{
    std::wcout << L"WAITING FOR CONNECTION... " << std::endl;
    while (true)
    {
        DWORD DriveMask = GetLogicalDrives();//GetLogicalDrives()=> returns bitmask of drives
        const char FirstDrive = 'A';
        wchar_t iDrive;
        for (int i = 0; i < 32; i++)
            if (DriveMask & (1 << i))
            {
                iDrive = FirstDrive + i;
                std::wstring drive_name(1,iDrive);
                drive_name += L":\\";
                if (GetDriveTypeW((wchar_t*)drive_name.c_str()) == DRIVE_REMOVABLE)//is portable storage
                {
                    std::time_t current_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());//connection time
                    std::wcout << L"REMOVABLE DRIVE CONNECTED\n\nTIME: " << std::ctime(&current_time_t) << std::endl<< std::endl;
                    std::wcout << L"COPIED FILES:\n";
                    Recursive_search(drive_name + L"*");
                    return;
                }
            }
    }
}
int main()
{
    setlocale(LC_ALL,"");
    std::wcout <<"ENTER WANTED PATH FOR COPIED FILES: ";
    std::wcin >> destination_path;
    ConnectionControl();
    system("pause");
    return 0;
}
