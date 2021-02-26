
#ifndef MECLI_TRIE_HPP_
#define MECLI_TRIE_HPP_

#include <string>
#include <vector>
#include "Node.hpp"

class Trie
{
private:
	bool m_bSpace; //true - string that stores user query contains space
	std::string m_strNoPrefix; //if space in string exists - string that stores user query minux prefix used for query completion
	Node* m_pRoot;
public:
	Trie()
	{
		m_pRoot = new Node();
		m_bSpace = false;
	}
	~Trie() { }
	void IsSpace(std::string data); //searches user's query string for space and sets bool space appropriately
	
	void Insert(std::string data); //builds trie tree struction
	std::vector<std::string> Search(std::string data); //wrapper for main auto-completion method; advances to last char in user's prefix
	std::vector<std::string> PrintTree(Node* root, std::string data, std::string str); //prints suggested queries
	std::vector<std::string> PrintTree(Node* root, std::string str); //overloaded print_tree method used for recursion
	void RemovePrefix(std::string data); //removes the prefix (if space in string)
	std::string BreakString(std::string); //returns only prefix (if space in string) used in search and print_tree methods

	bool getSpace()
	{
		return m_bSpace;
	}

	std::string getNoPrefix()
	{
		return m_strNoPrefix;
	}
};

#endif
