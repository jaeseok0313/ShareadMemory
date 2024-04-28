# ShareadMemory

## MFC 프로그램 공유메모리 읽기

```cpp
// Global로 전역설정을 해줘야 사용자 프로세스와 서비스 프로세스 간 공유메모리 사용이 가능함
m_hMapFile = OpenFileMapping(FILE_MAP_READ || FILE_MAP_WRITE, FALSE, L"Global\\MySharedMemory");
if (m_hMapFile == NULL) 
{
    cerr << "OpenFileMapping failed: " << GetLastError() << endl;
    m_strName = "OpenFileMapping failed";
    m_strInfo.Append(m_strName);
    m_strInfo.Append(_T("\r\n"));
    m_strName.Empty();
    UpdateData(false);
}

m_pBuf = (LPWSTR)MapViewOfFile(m_hMapFile, FILE_MAP_READ, 0, 0, 256);
if (m_pBuf == NULL) 
{
    m_strName = "MapViewOfFile failed";
    m_strInfo.Append(m_strName);
    m_strInfo.Append(_T("\r\n"));
    m_strName.Empty();
    CloseHandle(m_hMapFile);
    UpdateData(false);
}
// 사용자 프로세스에서 관리자 권한이 없어도 동작 가능하도록 보안설정을 추가해줘야함
// 기본 보안설정 (NULL)로 진행할 시 관리자권한으로 실행되지 않으면 mfc프로그램에서 공유메모리에 접근하지 못함
SECURITY_ATTRIBUTES sa;
SECURITY_DESCRIPTOR sd;
InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
sa.nLength = sizeof(SECURITY_ATTRIBUTES);
sa.lpSecurityDescriptor = &sd;
sa.bInheritHandle = FALSE;

// 로그 메시지 추가
LogMessage(L"MyService started successfully.");

LPSECURITY_ATTRIBUTES lpAttributes = NULL; // 기본 보안 속성 사용
BOOL bInheritHandle = TRUE;

// 공유 메모리 생성
// Global로 전역설정을 해줘야 사용자 프로세스와 서비스 프로세스 간 공유메모리 사용이 가능함
HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, &sa, PAGE_READWRITE, 0, 256, L"Global\\MySharedMemory");
if (hMapFile == NULL) 
{
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
if (pBuf == NULL) 
{
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

void WriteToSharedMemory(LPWSTR pBuf) {
    // 쓰레드가 돌면서 공유 메모리에 데이터를 쓰는 예제
    int i = 0;
    WCHAR buffer[20];
    while (true) {
        swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%d", i);
        wstring message = L"Hello from the service! " + wstring(buffer);
        wcscpy_s(pBuf, 256, message.c_str()); // 256은 공유 메모리의 크기입니다.
        LogMessage(message);
        i++;
        Sleep(1000); // 쓰레드를 1초간 일시 정지시킴
    }
}
