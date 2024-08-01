#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include<bits/stdc++.h>
using namespace std;
// Define your Node structure here
class Node {
public:
    char data;
    Node* left;
    Node* right;

    Node(char data) : data(data), left(nullptr), right(nullptr) {}
};

// Function to reconstruct the Huffman tree from the encoded tree structure
// Function to reconstruct the Huffman tree from the encoded tree structure
Node* reconstructHuffmanTree(std::ifstream& inFile) {
    char bit;
    inFile.get(bit); // Read a bit from the file

    if (bit == '1') {
        // If the current bit is '1', it indicates an internal node.
        // Recursively reconstruct left and right subtrees.
        Node* leftChild = reconstructHuffmanTree(inFile);
        Node* rightChild = reconstructHuffmanTree(inFile);
        
        // Create and return a new internal node with left and right children.
        Node* internalNode = new Node('\0'); // Internal nodes don't contain data.
        internalNode->left = leftChild;
        internalNode->right = rightChild;
        return internalNode;
    } else {
        // If the current bit is '0', it indicates a leaf node.
        // Read the character (data) associated with the leaf node.
        char data;
        inFile.get(data);

        // Create and return a new leaf node with the read character.
        Node* leafNode = new Node(data);
        return leafNode;
    }
}


// Function to decode the compressed data using the Huffman tree
void decodeData(ifstream& inFile, ofstream& outFile, Node* root, int fileSize) {
    Node* currentNode = root;
    cout<<"currentNode:"<<currentNode->data<<endl;
    char bit;

    while (fileSize > 0) {
        // Read a bit from the compressed file
        inFile.read(&bit, sizeof(char));
        //cout<"bit:"<<static_cast<int>(bit)<<endl;
        // Traverse the Huffman tree based on the bit
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        // If a leaf node is reached, write its data to the output file
        if (currentNode->left == nullptr && currentNode->right == nullptr) {
        	cout<<"currentNode:"<<currentNode->data<<endl;
            outFile.put(currentNode->data);
            currentNode = root; // Reset to the root of the Huffman tree
            fileSize--; // Decrement remaining file size
        }
    }
}

// Function to perform decompression
void decompressFile(const std::string& inputFile, const std::string& outputFile) {
    ifstream inFile(inputFile, ios::binary);
    if (!inFile) {
        cerr << "Error: Unable to open input file." << std::endl;
        return;
    }

    ofstream outFile(outputFile, ios::binary);
    if (!outFile) {
        cerr << "Error: Unable to open output file." << endl;
        inFile.close();
        return;
    }

    // Read the size of the compressed file
    int fileSize;
    inFile.read(reinterpret_cast<char*>(&fileSize), sizeof(int));

    // Reconstruct the Huffman tree from the encoded tree structure
    Node* root = reconstructHuffmanTree(inFile);
    cout<<"fileSize:"<<fileSize<<endl;
    // Decode the compressed data using the Huffman tree
    decodeData(inFile, outFile, root, fileSize);

    // Close the files
    inFile.close();
    outFile.close();
}

int main() {
    // Example usage: Decompress "compressed_file.bin" to "output.txt"
    decompressFile("outputFile.txt", "decompressed.txt");
    return 0;
}

