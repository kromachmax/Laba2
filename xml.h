#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <utility>
#include <iterator>
#include <algorithm>

class XML{

    public:
        class Node {
        public:
            Node(const std::string& value, const std::string& tag);
            void Add(std::unique_ptr<Node>);
            std::string ConvertingXMLTreeToText(int) const;
            const std::string& Get_Tag()const{
                return tag;
            }
            const std::string& Get_Value()const{
                return value;
            }
            void Linearization(std::vector<Node*>&) const;
            void Linearization(std::vector<Node*>&, const Node*, int&) const;
            friend std::ostream& operator<< (std::ostream& os, const Node&);
            std::vector<std::unique_ptr<Node>>& Get_Childrens(){
                return childrens;
            }

        private:
            std::string tag;
            std::string value;
            std::vector<std::unique_ptr<Node>> childrens;
        };

    private:

        XML(){};
        explicit XML(const std::string&);
        XML(const XML&) = delete;
        XML& operator= (const XML&) = delete;
        void ParseXML(const std::string&);
        
    private:
        std::unique_ptr<Node> Root_Node;
        int size = 0;

    private:
        std::unique_ptr<Node> parse_xml(int& position, const std::string&);
        std::string Find_Next_Tag(int& position, const std::string&) const;
        std::string Find_Next_Value(int& position, const std::string&) const;
        std::string circumcision(const std::string&) const;
        
    public:

        template<bool IsConst> class CommonIterator;
        using const_iterator = CommonIterator<true>;
        using iterator = CommonIterator<false>;
        static XML& Instance();
        static XML& Instance(const std::string&);
        void LoadXML(const std::string&);
        void WriteToXML(const std::string&); 
        void PrintXMLTree() const;
        const_iterator& add(std::unique_ptr<Node>, const_iterator&); 
        iterator begin() const;
        iterator end() const;
        const_iterator cbegin()const;
        const_iterator cend() const;
        iterator find_by_tag(const std::string&) const;
        iterator find_by_value(const std::string&) const;
        template<bool IsConst>
            bool erase(CommonIterator<IsConst>&);
        template<bool IsConst>
            iterator add(std::unique_ptr<Node>, CommonIterator<IsConst>&);
        template<bool IsConst>
        class CommonIterator{
            private:
                std::vector<Node*> nodes;
                int current_index = 0;
                bool erase(); 
                template<bool isConst>
                    friend bool XML::erase(CommonIterator<isConst>&);
            public:

                CommonIterator(Node* ptr, int idx);
                CommonIterator(Node* ptr, Node* root);
                
                std::conditional_t<IsConst, const Node, Node>& operator*();

                CommonIterator& operator++();

                CommonIterator operator++(int);

                bool operator!=(const CommonIterator&) const;

                bool operator==(const CommonIterator&) const;

                CommonIterator& operator+= (int n);

                CommonIterator& operator+ (int n);

                std::conditional_t<IsConst, const Node*, Node*> operator->();
       };
};

template<bool IsConst>
XML::CommonIterator<IsConst>::CommonIterator(Node* node, int idx){
    nodes.push_back(node);
    node->Linearization(nodes);
    current_index = idx;
}

template<bool IsConst>
XML::CommonIterator<IsConst>::CommonIterator(Node* node, Node* root){
   nodes.push_back(root); 
   if(node == root){
       current_index = 0;
   }
   root->Linearization(nodes, node, current_index);
   if(current_index == -1){
       current_index = nodes.size();
   }
}

template<bool IsConst>
std::conditional_t<IsConst, const XML::Node, XML::Node>& XML::CommonIterator<IsConst>:: operator*(){
    if(current_index >= nodes.size()) throw std::runtime_error("You are out of range!!!"); 
    return *nodes[current_index];
}

template<bool IsConst>
XML::CommonIterator<IsConst>& XML::CommonIterator<IsConst>::operator++(){
    current_index++;
    return *this;
}

template<bool IsConst>
XML::CommonIterator<IsConst> XML::CommonIterator<IsConst>::operator++(int n){
    CommonIterator<IsConst> tmp = *this;
    current_index++;
    return tmp;
}

template<bool IsConst>
bool XML::CommonIterator<IsConst>::operator!=(const CommonIterator& other) const{
    return !(*this == other);
}

template<bool IsConst>
bool XML::CommonIterator<IsConst>::operator==(const CommonIterator& other) const{
    return current_index == other.current_index;
}

template<bool IsConst>
XML::CommonIterator<IsConst>& XML::CommonIterator<IsConst>::operator+= (int n){
    current_index += n;
    if(current_index >= nodes.size()){
        current_index = nodes.size();
    }

    return *this;
}

template<bool IsConst>
XML::CommonIterator<IsConst>& XML::CommonIterator<IsConst>::operator+ (int n){
    *this += n;
    return *this;
}

template<bool IsConst>
std::conditional_t<IsConst, const XML::Node*, XML::Node*> XML::CommonIterator<IsConst>:: operator->(){
    if(current_index >= nodes.size()) throw std::runtime_error("You are out of range!!!"); 
    return nodes[current_index];
}

template<bool IsConst>
bool XML::CommonIterator<IsConst>::erase(){
    if(current_index >= nodes.size()) throw std::runtime_error("You are out of range!!!"); 
    Node* temp = nodes[current_index];
    for( const auto& node : nodes){
        auto& childs = node->Get_Childrens();
        for(auto iter = childs.begin(); iter != childs.end(); ++iter){
            if(iter->get() == temp){
                childs.erase(iter);
                return true;
            }
        }
    }
    return false;
}

template<bool IsConst>
bool XML::erase(CommonIterator<IsConst>& iter){
    return iter.erase();
}

template<bool IsConst>
XML::iterator XML::add(std::unique_ptr<Node> node, CommonIterator<IsConst>& iter){
    iter->Add(std::move(node));
    return iterator(node.get(), Root_Node.get());
}




