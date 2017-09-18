#include <string>
#include <iostream>
#include <variant>

struct SampleVisitor
{
    void operator()(const int &i) const { std::cout << "int: " << i << "\n"; }
    void operator()(const float& f) const { std::cout << "float: " << f << "\n"; }
};
 
int main()
{
    std::variant<int, float> intOrFloat;
    static_assert(std::variant_size_v<decltype(intOrFloat)> == 2);
    
    // default initialized to the first alternative, should be 0
    std::visit(SampleVisitor(), intOrFloat);
    
    // won't compile:
    // error: no match for 'operator=' (operand types are 'std::variant<int, float>' and 'std::__cxx11::basic_string<char>')
    // intOrFloat = std::string("hello");
    
    // index will show the currently used 'type'
    std::cout << "index = " << intOrFloat.index() << std::endl;
    intOrFloat = 100.0f;
    std::cout << "index = " << intOrFloat.index() << std::endl;

    // try with get_if:
    if (const auto intPtr (std::get_if<int>(&intOrFloat)); intPtr) 
        std::cout << "int!" << *intPtr << "\n";
    else if (const auto floatPtr (std::get_if<float>(&intOrFloat)); floatPtr) 
        std::cout << "float!" << *floatPtr << "\n";

    // visit:
    std::visit(SampleVisitor(), intOrFloat);
    intOrFloat = 10;
    std::visit(SampleVisitor(), intOrFloat);
}