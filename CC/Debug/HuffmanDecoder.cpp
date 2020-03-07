#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

#include "HuffmanTree.h"
#include "HuffmanDecoder.h"
#include "dFile.h"

HuffmanDecoder::HuffmanDecoder(std::string &s) : filesize(0)
{
	ifstream file( string(s + ".meta").c_str() );
	ofstream done( s.c_str(), ios::binary | ios::out);

	file >> filesize;

	while (!file.eof())
	{
		int i, j;
		file >> i >> j;

		if (!file.fail())
			table.push_back(new TreeNode(std::make_pair(i, j)));
	}

	root = HuffmanTree::CreateTree(table, string("demo"));
	TreeNode *tmp;

	Input inFile(s + ".huff");

	for (int i = 0; i < filesize; ++i)
	{
		tmp = root;

		while (tmp->data.first == -1)
		{
			if (inFile.GetBit() == 0)
				tmp = tmp->left;
			else
				tmp = tmp->right;
		}

		done.write((char *)(&tmp->data.first), 1);
	}
}