/*
Huffman Coding Implementation
Author: [Your Name]
Date: [Date]
Description:
This program implements Huffman Coding, a lossless data compression algorithm.
The process involves:
1. Calculating character frequencies.
2. Building a Huffman Tree using a min-heap (priority queue).
3. Generating Huffman Codes for each character.
4. Encoding the input string using Huffman Codes.
5. Decoding the encoded binary representation back into the original string.
6. Comparing the sizes of the original string and the encoded binary representation to demonstrate compression.

Test Cases:
1. Input: "huffman coding is fun"
   Output: Frequency table, Huffman Codes, encoded string, and decoded string.
2. Input: "data compression is essential"
   Output: Frequency table, Huffman Codes, encoded string, and decoded string.
*/

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// Node structure for Huffman Tree
struct Node {
    char ch;
    int freq;
    Node* left, * right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparison object for priority queue (min-heap)
struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

// Generate frequency table
unordered_map<char, int> calculateFrequency(const string& text) {
    unordered_map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }
    return freqMap;
}

// Generate Huffman Codes recursively
void generateCodes(Node* root, const string& code, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = code;
    }
    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

// Encode the input string using Huffman Codes
string encode(const string& text, const unordered_map<char, string>& huffmanCodes) {
    string encodedString = "";
    for (char ch : text) {
        encodedString += huffmanCodes.at(ch);
    }
    return encodedString;
}

// Decode the binary string using the Huffman Tree
string decode(const string& encodedString, Node* root) {
    string decodedString = "";
    Node* current = root;
    for (char bit : encodedString) {
        current = (bit == '0') ? current->left : current->right;
        if (!current->left && !current->right) {
            decodedString += current->ch;
            current = root;
        }
    }
    return decodedString;
}

// Build Huffman Tree
Node* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;
    for (auto& pair : freqMap) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();
        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        minHeap.push(merged);
    }

    return minHeap.top();
}

int main() {
    // Input string from the user
    cout << "Enter a string: ";
    string text;
    getline(cin, text);

    // Step 1: Calculate frequency of characters
    unordered_map<char, int> freqMap = calculateFrequency(text);
    cout << "\nFrequency Table:\n";
    for (auto& pair : freqMap) {
        cout << pair.first << ": " << pair.second << "\n";
    }

    // Step 2: Build Huffman Tree
    Node* root = buildHuffmanTree(freqMap);

    // Step 3: Generate Huffman Codes
    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    cout << "\nCharacter | Frequency | Huffman Code\n";
    for (auto& pair : huffmanCodes) {
        cout << pair.first << "         | " << freqMap[pair.first] << "         | " << pair.second << "\n";
    }

    // Step 4: Encode the input string
    string encodedString = encode(text, huffmanCodes);
    cout << "\nOriginal String: " << text << "\n";
    cout << "Encoded Binary Representation: " << encodedString << "\n";

    // Step 5: Decode the binary representation
    string decodedString = decode(encodedString, root);
    cout << "Decoded String: " << decodedString << "\n";

    // Verify decoded string matches original string
    if (text == decodedString) {
        cout << "The decoded string matches the original string.\n";
    }
    else {
        cout << "Error: The decoded string does not match the original string.\n";
    }

    // Step 6: Analyze compression
    int originalSize = text.size() * 8; // 8 bits per character
    int compressedSize = encodedString.size();
    cout << "\nOriginal Size: " << originalSize << " bits\n";
    cout << "Compressed Size: " << compressedSize << " bits\n";
    cout << "Compression Ratio: " << ((double)compressedSize / originalSize) * 100 << "%\n";

    return 0;
}
