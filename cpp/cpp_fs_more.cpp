// filesystem_path_example.cpp  
// compile by using: /EHsc  
#include <string>  
#include <iostream>  
#include <sstream>  
#include <experimental/filesystem>  

using namespace std;
namespace fs = std::experimental::filesystem;

std::uintmax_t ComputeFileSize(const fs::path& pathToCheck)
{
	if (fs::exists(pathToCheck) && fs::is_regular_file(pathToCheck))
	{
		auto err = std::error_code{};
		auto filesize = fs::file_size(pathToCheck, err);
		if (filesize != static_cast<uintmax_t>(-1))
			return filesize;
	}

	return static_cast<uintmax_t>(-1);
}

void DisplayFileInfo(const std::experimental::filesystem::v1::directory_entry & entry, std::string &lead, std::experimental::filesystem::v1::path &filename)
{
	time_t cftime = chrono::system_clock::to_time_t(fs::last_write_time(entry));
	cout << lead << " " << filename << ", " 
		 << ComputeFileSize(entry) 
		 << ", time: " << std::asctime(std::localtime(&cftime));
}

void DisplayDirectoryTreeImp(const fs::path& pathToShow, int level)
{
	if (fs::exists(pathToShow) && fs::is_directory(pathToShow))
	{
		auto lead = std::string(level * 3, ' ');
		for (const auto& entry : fs::directory_iterator(pathToShow))
		{
			auto filename = entry.path().filename();
			if (fs::is_directory(entry.status()))
			{
				cout << lead << "[+] " << filename << "\n";
				DisplayDirectoryTreeImp(entry, level + 1);
				cout << "\n";
			}
			else if (fs::is_regular_file(entry.status()))
				DisplayFileInfo(entry, lead, filename);
			else
				cout << lead << " [?]" << filename << "\n";
		}
	}
}

// adapted from Modern C++ Programming Cookbook
void DisplayDirectoryTree(const fs::path& pathToShow)
{
	DisplayDirectoryTreeImp(pathToShow, 0);
}


int main(int argc, char* argv[])
{
	const fs::path pathToShow{ argc >= 2 ? argv[1] : fs::current_path() };

	DisplayDirectoryTree(pathToShow);
}