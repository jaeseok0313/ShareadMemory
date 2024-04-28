#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define SERVICE_NAME L"MyService"
#define LOG_FILE_PATH L"C:\\MyService.log" // �α� ���� ���

SERVICE_STATUS        g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;
wofstream             g_LogFile;

void LogMessage(const wstring& message) {
    g_LogFile.open(LOG_FILE_PATH, ios::app);
    if (g_LogFile.is_open()) {
        wcout << L"Log file is open." << endl;
        g_LogFile << message << endl;
        g_LogFile.close();
    } else {
        wcerr << L"Failed to open log file." << endl;
        wcerr << L"Error code: " << GetLastError() << endl;
    }
}

void WriteToSharedMemory(LPWSTR pBuf) {
    // �����尡 ���鼭 ���� �޸𸮿� �����͸� ���� ����
    int i = 0;
    WCHAR buffer[20];
    while (true) {
        swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%d", i);
        wstring message = L"Hello from the service! " + wstring(buffer);
        wcscpy_s(pBuf, 256, message.c_str()); // 256�� ���� �޸��� ũ���Դϴ�.
		LogMessage(message);
        i++;
        Sleep(1000); // �����带 1�ʰ� �Ͻ� ������Ŵ
    }
}

void WINAPI ServiceCtrlHandler(DWORD CtrlCode) {
    switch (CtrlCode) {
        case SERVICE_CONTROL_STOP:
            if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
                break;

            // ���� ����
            g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
            SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

            // ���� �̺�Ʈ ����
            SetEvent(g_ServiceStopEvent);
            break;
    }
}

void WINAPI ServiceMain(DWORD argc, LPWSTR *argv) {
    g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);
    if (g_StatusHandle == NULL) {
        // ���� ���� �ڵ鷯 ��� ����
        LogMessage(L"Failed to register service control handler.");
        return;
    }

    // ���� �ʱ�ȭ �� ���� ����
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwWin32ExitCode = NO_ERROR;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    // ���� ����
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

	// ���� ��ũ����
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = FALSE;


    // �α� �޽��� �߰�
    LogMessage(L"MyService started successfully.");
	LPSECURITY_ATTRIBUTES lpAttributes = NULL; // �⺻ ���� �Ӽ� ���
    BOOL bInheritHandle = TRUE;
    // ���� �޸� ����
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, &sa, PAGE_READWRITE, 0, 256, L"Global\\MySharedMemory");
    if (hMapFile == NULL) {
        WCHAR buffer[20];
        swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%d", GetLastError());
        wstring errorMessage = L"Failed to create shared memory. Error code: ";
        errorMessage += buffer;
        LogMessage(errorMessage);
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
        return;
    }

    LPWSTR pBuf = (LPWSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 256);
    if (pBuf == NULL) {
        WCHAR buffer[20];
        swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%d", GetLastError());
        wstring errorMessage = L"Failed to map view of file. Error code: ";
        errorMessage += buffer;
        LogMessage(errorMessage);
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
        CloseHandle(hMapFile);
        return;
    }

    // ���� �޸𸮿� ������ ����
    WriteToSharedMemory(pBuf);

    // ���� �޸� �� �ڵ� ����
}

int main() {
    // �α� ���� ����

    // ���� ���̺� ����
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };

    // ���񽺸� SCM�� ���
    if (!StartServiceCtrlDispatcher(ServiceTable)) {
        // ���� ��� ����
        WCHAR buffer[20];
        swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%d", GetLastError());
        wstring errorMessage = L"Failed to start service dispatcher. Error code: ";
        errorMessage += buffer;
        LogMessage(errorMessage);
        g_LogFile.close();
        return 1;
    }
    // �α� ���� �ݱ�
    g_LogFile.close();
    return 0;
}
