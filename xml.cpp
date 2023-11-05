#include "xml.h"

XML& XML::Instance(){
    static XML document;
    return document;
}

XML& XML::Instance(const std::string& path){
    static XML document(path);
    return document;
}

XML::XML(const std::string& path){
    LoadXML(path);
}

void XML::LoadXML(const std::string& path){

    std::ifstream file(path);
    if(!file){
        throw std::runtime_error("FILE NOT FOUND!!!");
    }
    std::string XML =  std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    ParseXML(XML);
}

void XML::PrintXMLTree() const{
    std::cout << Root_Node->ConvertingXMLTreeToText(1);
}

void XML::WriteToXML(const std::string& path){
    std::ofstream out;
    out.open(path);
    if(out.is_open()){
        out << Root_Node->ConvertingXMLTreeToText(1);
    }
    else{
        throw std::runtime_error("FILE NOT OPEN!!!");
    }
}

void XML::ParseXML(const std::string& XML){
    int Start_Position = 0;
    Root_Node = parse_xml(Start_Position, XML);
}

std::unique_ptr<XML::Node> XML::parse_xml(int& position, const std::string& XML){

    auto tag = Find_Next_Tag(position, XML);
    auto value = Find_Next_Value(position, XML);
    auto node = std::make_unique<Node>(value, tag);
    auto Next_Tag = Find_Next_Tag(position, XML);
    size++;

    while(Next_Tag != ("/" + tag) && position < XML.size()){
        position -= Next_Tag.size() + 2;
        node->Add(parse_xml(position, XML));
        Next_Tag = Find_Next_Tag(position, XML);
    }

    return node;
}

void XML::Node::Linearization(std::vector<XML::Node*>& nodes) const{

    for (auto& children : childrens){
        nodes.push_back(children.get());
        children->Linearization(nodes);
    }
}

void XML::Node::Linearization(std::vector<XML::Node*>& nodes, const Node* node, int& idx) const{

    for (auto& children : childrens){ 
        Node* tmp = children.get();
        if(tmp == node) idx = nodes.size();
        nodes.push_back(children.get());
        children->Linearization(nodes, node, idx);
    }

}

std::ostream& operator<< (std::ostream& os, const XML::Node& node){
    std::cout << node.tag << ' ' <<node.value << std::endl;
    return os;
}

std::string XML::Find_Next_Tag(int& position, const std::string& XML) const{
    position  = XML.find("<", position) + 1;
    int end = XML.find(">", position);
    std::string tag =  XML.substr(position, end - position);
    position = end + 1;
    return tag;
}

std::string XML::Find_Next_Value(int& position, const std::string& XML) const{
    int start = position;
    position = XML.find("<", position);
    return circumcision(XML.substr(start, position-start));
}

std::string XML::circumcision(const std::string& str) const{
  std::string result = "";
  int start_pos = 0;
  int end_pos = str.length() - 1;

  for (; start_pos < str.length(); start_pos += 1) {
    char ch = str[start_pos];

    if (isspace(ch)) {
      continue;
    } else {
      break;
    }
  };
  for (; end_pos >= 0; end_pos -= 1) {
    char ch = str[end_pos];

    if (isspace(ch)) {
      continue;
    } else {
      break;
    }
  };

  return str.substr(start_pos, end_pos - start_pos + 1);
};

XML::Node::Node(const std::string& value, const std::string& tag):value(value), tag(tag){};

void XML::Node::Add(std::unique_ptr<Node> node){
    childrens.push_back(std::move(node));
}

std::string XML::Node::ConvertingXMLTreeToText(int depth) const{

    std::string indent(depth, ' ');
    std::string result;
    result += indent;
    result += "<" + tag + ">" + value;
    if(childrens.size()){
        result += '\n';
    }
    for(const auto& children : childrens){
        result += children->ConvertingXMLTreeToText(depth * 2);
    }

    if(childrens.size()){
        result += indent;
    }
    result += "</" + tag + ">";
    result += '\n';
    return result;
}


XML::iterator XML::begin() const{
    return iterator(Root_Node.get(), 0);
}

XML::iterator XML::end() const {
    return iterator(Root_Node.get(), size);
}

XML::const_iterator XML::cbegin() const {
    return const_iterator(Root_Node.get(), 0);
}

XML::const_iterator XML::cend() const {
    return const_iterator(Root_Node.get(), size);
}

XML::iterator XML::find_by_tag(const std::string& tag) const{

   auto iter = begin();
   for( ; iter != end(); ++iter){
       if(iter->Get_Tag() == tag){
           break;
       }
   }
   return iter;
}

XML::iterator XML::find_by_value(const std::string& value) const{

    auto iter = begin();
    for( ; iter != end(); ++iter){
        if(iter->Get_Value() == value){
            break;
        }
    }
    return iter;
}




