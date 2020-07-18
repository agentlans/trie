/*

Copyright 2020 Alan Tseng

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _TRIE
#define _TRIE

#include <map>
#include <vector>
#include <string>

using std::map;
using std::string;

// A node in the tree
struct Node
{
	map<char,Node*> next;
	bool end = false;
};


// Prefix tree that can store words and query them by prefix
class Trie
{
public:
	// Trie must have a root node
	Trie() {
		root = new Node();
	}
	// The nodes in the branches are allocated on heap so make sure we clean up
	~Trie() {
		delete_tree(root);
	}
	
	// Stores a string inside the trie
	void store(string str) {
		Node* node = root;
		for (const auto& chr : str) {
			// Make a new node if we can't find the letter
			if (node->next.find(chr) == node->next.end()) {
				Node* new_node = new Node();
				node->next[chr] = new_node;
			}
			// Go to next node
			node = node->next[chr];
		}
		node->end = true; // Signals the word is complete
	}

	// Returns list of words starting with given prefix
	std::vector<string> words_starting_with(string prefix) {
		Node* node = navigate_to(prefix);
		auto c = list_all_children(node);
		// Add prefix to all the listed words
		std::vector<string> out;
		for (const auto& cs : c) {
			out.push_back(prefix + cs);
		}
		return out;
	}

	// Returns the longest prefix of given string that's in the trie
	string longest_prefix(string st) {
		string out = "";
		Node* node = root;
		for (const auto& chr : st) {
			if (node->next.find(chr) != node->next.end()) {
				out += chr;
				node = node->next[chr];
			} else {
				// Couldn't find any more letters of the string
				break;
			}
		}
		return out;
	}

private:
	Node* root;

	// Recursively deletes a node and all its children
	void delete_tree(Node* node)
	{
		for (const auto& x : node->next) {
			delete_tree(x.second);
		}
		delete node;
	}

	// Returns pointer to node starting from the root and following the characters of str
	// If no string in trie starts with str, then will return nullptr
	Node* navigate_to(string str) {
		Node* node = root;
		for (const auto& chr : str) {
			if (node->next.find(chr) == node->next.end()) {
				// Couldn't find the next edge to follow
				return nullptr;
			}
			node = node->next[chr];
		}
		return node;
	}

	// Lists all strings starting from given node
	std::vector<string> list_all_children(Node* node) {
		if (node == nullptr) {
			return std::vector<string>();
		}
		// If a string ends on this node, then include empty string
		std::vector<string> temp;
		if (node->end) {
			temp.push_back("");
		}
		// Move forward one letter
		for (const auto& kv : node->next) {
			auto k = kv.first;
			// Recursively list children of the node
			auto child_strs = list_all_children(kv.second);
			for (const auto& child_str : child_strs) {
				temp.push_back(k + child_str);
			}
		}
		return temp;
	}
};

#endif

