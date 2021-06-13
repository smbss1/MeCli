
#include "Node.hpp"

Node::Node()
{
	setChar('\0');
	setWord(false);
	for (int i = 0; i < 128; i++)
		m_pChild[i] = nullptr;
}

