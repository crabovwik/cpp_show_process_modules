#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <Psapi.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Run as " << argv[0] << " pid" << std::endl;
        std::cout << "Example: " << argv[0] << " 1337" << std::endl;
        return 1;
    }

    unsigned long process_id = (unsigned long) atoll(argv[1]);
    std::cout << "Process id has just been parsed: " << process_id << std::endl;

    HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);
    if (process_handle == nullptr) {
        std::cout << "A process has not been found by the specified process id" << std::endl;
        return 1;
    }

    std::cout << "A process has just been found! It's placed at " << (void *) process_handle << std::endl;
    std::cout << "I am going to get handles for all modules of the process" << std::endl;

    HMODULE modules_handles[1024];
    DWORD count_of_bytes_to_store_modules_handles;
    // const bool modules_enum_result = EnumProcessModules(process_handle, modules, sizeof(modules), &count_of_bytes_to_store_modules);
    const bool modules_handles_enum_result = EnumProcessModulesEx(process_handle, modules_handles, sizeof(modules_handles), &count_of_bytes_to_store_modules_handles, LIST_MODULES_ALL);
    if (modules_handles_enum_result == FALSE) {
        std::cout << "Could not get modules handles of the process" << std::endl;
        return 1;
    }

    std::cout << "Modules handles have just been got" << std::endl;

    unsigned long modules_handles_count = count_of_bytes_to_store_modules_handles / sizeof(HMODULE);

    std::cout << "A count of modules handles is " << modules_handles_count << std::endl;

    for (unsigned long i = 0; i < modules_handles_count; i++) {
        HMODULE module_handle = modules_handles[i];
        WCHAR module_name[MAX_PATH];
        if (GetModuleFileNameEx(process_handle, module_handle, module_name, MAX_PATH) == NULL) {
            std::cout << "Could not get information about a module at 0x" << (void *) module_handle << " (iteration #"
                      << (i + 1) << ")" << std::endl;
            continue;
        }

        std::wcout << "A module has been found at 0x" << (void *) module_handle << " with the name " << module_name
                   << std::endl;
    }

    return 0;
}
