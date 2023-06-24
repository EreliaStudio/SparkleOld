#include "playground.hpp"

int main() {
	spk::cout << " --- " << __LINE__ << std::endl;
    // Test pour methodName et className
    std::wstring prettyFunction = L"void spk::MyClass::TestFunction()";
    std::wstring name = spk::methodName(prettyFunction);
    std::wstring class_name = spk::className(prettyFunction);
    std::wcout << L"Method Name: " << name << std::endl;
    std::wcout << L"Class Name: " << class_name << std::endl;

	spk::cout << " --- " << __LINE__ << std::endl;
    // Test pour stringSplit
    std::wstring test_string = L"This is a test string";
    std::wstring delim = L" ";
    auto result = spk::stringSplit(test_string, delim);
    for (const auto& word : result) {
        std::wcout << word << std::endl;
    }

	spk::cout << " --- " << __LINE__ << std::endl;
    // Test pour getStr
    std::wfstream file(L"test_file.txt");
    std::wstring file_content = spk::getStr(file);
    std::wcout << file_content << std::endl;

	spk::cout << " --- " << __LINE__ << std::endl;
    // Test pour getStringSplit
    std::wfstream file2(L"test_file2.txt");
    std::wstring delim2 = L",";
    auto result2 = spk::getStringSplit(file2, delim2);
    for (const auto& word : result2) {
        std::wcout << word << std::endl;
    }

	spk::cout << " --- " << __LINE__ << std::endl;
    // Test pour positive_modulo
    int modulo_result = spk::positive_modulo(-5, 3);
    std::wcout << L"Modulo Result: " << modulo_result << std::endl;

	spk::cout << " --- " << __LINE__ << std::endl;
    // Test pour listFile
    std::wstring test_path = L".";
    std::wstring test_extension = L".txt";
    auto file_list = spk::listFile(test_path, test_extension);
    for (const auto& file : file_list) {
        std::wcout << file << std::endl;
    }

	spk::cout << " --- " << __LINE__ << std::endl;
    // Test pour listDir
    auto dir_list = spk::listDir(test_path);
    for (const auto& file : dir_list) {
        std::wcout << file << std::endl;
    }
	spk::cout << " --- " << __LINE__ << std::endl;

    return 0;
}