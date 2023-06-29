#include "playground.hpp"

void MementoTest()
{
	std::wstring str(L"Hello, World!");

	spk::Memento<std::wstring> memento(str);

	spk::Memento<std::wstring>::Snapshot* snapshot = memento.takeSnapshot();

	spk::cout << L"Begin: str = " << str << std::endl;

	str = L"Goodbye, World!";
	auto secondSnapshot = memento.takeSnapshot();

	spk::cout << L"\tAssignation: str = " << str << std::endl;

	memento.restoreSnapshot(snapshot);

	spk::cout << L"\t\tRestored: str = " << str << std::endl;

	str = L"Whatev, World!";

	spk::cout << L"\tAssignation: str = " << str << std::endl;

	memento.restoreSnapshot(snapshot);

	spk::cout << L"\t\tRestored: str = " << str << std::endl;

	memento.restoreSnapshot(secondSnapshot);

	spk::cout << L"\t\tRestored: str = " << str << std::endl;

	spk::Memento<std::wstring>::Snapshot standaloneSnapshot;

	standaloneSnapshot.save(&str);

	str = L"Ahahah";

	spk::cout << L"\tAssignation: str = " << str << std::endl;

	// standaloneSnapshot.load(&str); // Both work.
	memento.restoreSnapshot(&standaloneSnapshot);

	spk::cout << L"\t\tRestored: str = " << str << std::endl;
}

int main()
{
	MementoTest();

	return 0;
}