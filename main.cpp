#include "xml.h"

int main(void){

    try{
        XML& document = XML::Instance();
        document.LoadXML("./input.xml");

        //Displaying the tree to the console
        document.PrintXMLTree();

        //Create a new node with children
        std::unique_ptr<XML::Node> address = std::make_unique<XML::Node>("", "Address");
        std::unique_ptr<XML::Node> Name = std::make_unique<XML::Node>("Maxim Kromachev", "Name");
        std::unique_ptr<XML::Node> City = std::make_unique<XML::Node>("Saint-Peterburg", "City");
        std::unique_ptr<XML::Node> Country = std::make_unique<XML::Node>("Russia", "Country");
        address->Add(std::move(Name));
        address->Add(std::move(City));
        address->Add(std::move(Country));

        //Adding a new node to xml
        XML::iterator iter = document.find_by_tag("PurchaseOrder");
        XML:: iterator it = document.add(std::move(address), iter);
        std::cout << std::endl << std::endl;
        std::cout << *it << std::endl << std::endl; 

        //We traverse the entire tree using an iterator and display the opening tag and value
        std::cout << "Tags and Values:" << std::endl;
        for(auto it = document.begin(); it != document.end(); ++it){
            std::cout << *it;
        }

        std::cout << std::endl << std::endl;

        //Removing the element pointed to by the iterator
        auto iter2  = document.begin();
        iter2  = iter2 + 1;
        if(document.erase(iter2)){
            std::cout << "Successful removal!" << std::endl;
        }
        else std::cout << "Unsuccessful removal!" << std::endl;


        std::cout << std::endl << std::endl;

        auto iter3 = document.cbegin() + 5;
        std::cout << *iter3 << std::endl << std::endl;

        XML::iterator iter4 = document.find_by_value("Maxim Kromachev");
        std::cout << *iter4 << std::endl;

        document.WriteToXML("./output.xml"); 
    }
    catch(const std::runtime_error& obj){
        std::cout << obj.what() << std::endl;
    }

    return 0;
}
