
#ifndef MECLI_NODE_HPP_
#define MECLI_NODE_HPP_

#include <iostream>

class Node //node used to hold data in trie tree structure
{
private:
	char m_cChar;
	bool m_bWord; //true = marks end of word
public:
	Node* m_pChild[128]; //points to next node; each pointer for each char in ASCII table (0-127)

public:
	Node();
	~Node() { }

	void setWord(int boo)
	{
		m_bWord = boo;
	} 
	
	bool isWord()
	{
		return m_bWord;
	}

	void setChar(char ch)
	{
		m_cChar = ch;
	}
	
	char getChar() const
	{
		return m_cChar;
	}
};

#endif