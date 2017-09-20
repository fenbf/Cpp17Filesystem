// filesystem_path_example.cpp  
// compile by using: /EHsc  
#include <string>  
#include <iostream>  


int main(int argc, char* argv[])
{
	for (int i = 0; i < argc; ++i)
		std::cout << argv[i] << "\n";
}