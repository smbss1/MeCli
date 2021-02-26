
#include <iostream>
#include "Trie.hpp"
#include "Node.hpp"

// constructions trie tree structure
void Trie::Insert(std::string data)
{
	Node* tmp = m_pRoot;
	char cstr[data.length()];

	data.copy(cstr, data.length()); //create char array containing data from string object
	for (int i = 0; i < data.length(); i++)
	{
		if (tmp->m_pChild[static_cast<int>(cstr[i])] != NULL) //walk ptr through struct, preventing duplicate nodes (prefix)
			tmp = tmp->m_pChild[static_cast<int>(cstr[i])];
		else
		{
			tmp->m_pChild[static_cast<int>(cstr[i])] = new Node(); //create new node
			tmp = tmp->m_pChild[static_cast<int>(cstr[i])];
			tmp->setChar(cstr[i]); //insert char in new node
		}
	}
	tmp->setWord(true); //mark word
}

// method that prints query options to completing the prefix
std::vector<std::string> Trie::PrintTree(Node* root, std::string data, std::string str)
{
	std::vector<std::string> vResult;
	for (int i = 0; i < 128; i++) //loops through all 128 pointers in node to check if there's a m_pChild
	{
		Node* adv = root;
		if (adv->m_pChild[i] != nullptr) //m_pChild exists
		{
			adv = adv->m_pChild[i]; //advance pointer
			str += adv->getChar(); //append char to end of prefix
			if (adv->isWord()) //if point at end of word, print word
			{
				if (getSpace() == true) // if the user's search query contained a space, print the search query minus prefix then print word
					std::cout << getNoPrefix();
				vResult.push_back(str);
			}
			std::vector<std::string> vRes = PrintTree(adv, str); //recursive call to keep moving down tree
			vResult.insert(vResult.end(), vRes.begin(), vRes.end());
		}
		str = data; //reset prefix string
	}
	return vResult;
}

std::vector<std::string> Trie::PrintTree(Node* root, std::string str) //overloaded function for recursive calls
{
	std::vector<std::string> vResult;
	std::string str_tmp = str;

	for (int i = 0; i < 128; i++)
	{
		Node* adv = root;
		if (adv->m_pChild[i] != NULL)
		{
			adv = adv->m_pChild[i];
			str += adv->getChar();
			if (adv->isWord())
			{
				if (getSpace() == true)
					std::cout << m_strNoPrefix;
				// std::cout << str << std::endl;
				vResult.push_back(str);
			}
			PrintTree(adv, str);
		}
		str = str_tmp;
	}
	return vResult;
}

std::vector<std::string> Trie::Search(std::string data) //wrapper for function that completes the prefix, generating search options
{
	Node* tmp = m_pRoot;
	char cstr[data.length()];

	data.copy(cstr, data.length(), 0); 
	for (int i = 0; i < data.length(); i++) // for loop advances point through tree structure stopping at node containing last char in user's search prefix
	{
		if (tmp->m_pChild[static_cast<int>(cstr[i])] == nullptr) //lets user know if prefix not in dictionary (next char not a m_pChild)
		{
			return std::vector<std::string>();
		} else
			tmp = tmp->m_pChild[static_cast<int>(cstr[i])];
		if (tmp->getChar() != cstr[i]) // lets user know if prefix not in dictionary (char does not match corresponding char in prefix)
		{
			return std::vector<std::string>();
		}
	}
	std::string str = data; //copy user's query prefix into generic string used for appending operations to complete query prefix in print_tree method
	return PrintTree(tmp, data, str);	//passes pointer that has advanced to last char in query prefix to printing method
}

// checks user query string for space char and sets bool space appropriately
void Trie::IsSpace(std::string data)
{
	char cstr[data.length()];

	data.copy(cstr, data.length());
	for (int i = 0; i < data.length(); i++)
	{
		if (cstr[i] == ' ')
		{
			m_bSpace = true;
			break;
		}
	}
}

// isolates and returns prefix only, if query string contains a space
std::string Trie::BreakString(std::string data)
{
	char cstr[data.length()];
	std::string str;
	data.copy(cstr, data.length());

	for (int i = data.length(); i-- > 0;) //reverse loop
	{
		if (cstr[i] == ' ') //if space, stop and return generic string
			break;
		else
			str.insert(str.begin(),cstr[i]); //copies chars in generic string begining at the last character until loop encounters a space
	}
	return str; //returns the prefix only
}

void Trie::RemovePrefix(std::string data) //method removes the user's query prefix and copies to no_prefix attribute; print to screen along with
												//completed words to offer user search options
{
	char cstr[data.length()];
	data.copy(cstr, data.length());

	for (int i = data.length(); i-- > 0;) //reverse loop
	{
		if (cstr[i] == ' ') //break loop when space encountered
			break;
		else
			cstr[i] = '\0'; //each full iteration, last char changed to null char
	}
	m_strNoPrefix = cstr; //copies chars up to first null char into no_prefix string
}