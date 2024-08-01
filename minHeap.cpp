//creation of node
#include<bits/stdc++.h>
using namespace std;
class Node {
public:
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char ch, int freq) : ch(ch), freq(freq), left(NULL), right(NULL) {}
};
//calculate frequncies
void calculateFrequencies(const string& inputFile, map<char, int>& freq) {
    ifstream inFile(inputFile.c_str()); 
    if (!inFile.is_open()) {
        cerr << "Error opening file: " << inputFile << endl;
        exit(1);
    }

    char ch;
    while (inFile.get(ch)) {
        freq[ch]++;
    }

    inFile.close();
}
//build huffman tree
Node* buildHuffmanTree(map<char, int>& freq) {
    priority_queue<Node*, vector<Node*>, greater<Node*> > minHeap;

    for (map<char, int>::iterator it = freq.begin(); it != freq.end(); ++it) {
        minHeap.push(new Node(it->first, it->second));
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top();
        minHeap.pop();

        Node* right = minHeap.top();
        minHeap.pop();

        int sum = left->freq + right->freq;
        Node* node = new Node('\0', sum);
        node->left = left;
        node->right = right;
        minHeap.push(node);
    }

    return minHeap.top();
}
//generate huffman code
void generateCodes(Node* node, string code, map<char, string>& huffmanCodes) {
    if (node == NULL) {
        return;
    }

    if (!node->left && !node->right) {
        huffmanCodes[node->ch] = code;
    }

    generateCodes(node->left, code + "0", huffmanCodes);
    generateCodes(node->right, code + "1", huffmanCodes);
}
void writeTreeNodeToFile(std::ofstream& outFile, Node* node) {
    if (node == NULL) {
       // outFile << 'N'; // Write a special character to indicate a null node
        return;
    }

    // Write the data of the node
    cout<<node->ch;
    //outFile << 'D' << node->ch; // 'D' represents a data node followed by the data itself
    outFile << node->ch;
    // Recursively write the left and right subtrees
    writeTreeNodeToFile(outFile, node->left);
    writeTreeNodeToFile(outFile, node->right);
}
void writeTreeToFile(const std::string& outputFile, Node* root) {
    ofstream outFile(outputFile.c_str(), ios::out);
    if (!outFile) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return;
    }

    // Recursive function to write the tree nodes to the file
    // Pre-order traversal
    writeTreeNodeToFile(outFile, root);

    outFile.close();
}
void writeCompressedData(const string& inputFile, ofstream& outFile, map<char,string>& huffmanCodes) {
   ifstream inFile(inputFile.c_str(),std::ios::binary); 
    // Write the size of the input file to the output file
    inFile.seekg(0, std::ios::end);
    int fileSize = inFile.tellg();
    outFile.write(reinterpret_cast<const char*>(&fileSize), sizeof(int));

    // Reset file pointer to the beginning
    inFile.seekg(0, std::ios::beg);
    
    int numBits = 0;
    char byte = 0;
    char ch;
   // char temp;
     while (inFile.get(ch)) {
        string code = huffmanCodes.at(ch); // Retrieve Huffman code for the character
		cout<<ch<<"->"<<code<<endl;
        for (char bit : code) {
            // Shift the bits to the left
            byte <<= 1;
            cout<<"byte1:"<<byte<<endl;
            // Set the least significant bit if the current bit is '1'
            if (bit == '1') {
                byte |= 1;
//                temp=1;
//				temp=temp<<(7-numBits);		//right shift bits
//				byte=byte | temp;
            }
			cout<<"byte2:"<<byte<<endl;
            numBits++;

            // If the byte is full, write it to the output file
            if (numBits == 8) {
                outFile.put(byte);
                cout<<"byte8:"<<byte<<endl;
                byte = 0;
                numBits = 0;
            }
        }
    }

    // If there are remaining bits in the last byte, pad with zeros and write to the output file
    if (numBits > 0) {
        byte <<= (8 - numBits); // Left-shift to pad with zeros
        outFile.put(byte);
    }

    // Close the input file
    inFile.close();
}
//delete the tree
void deleteHuffmanTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    // Recursively delete the left and right subtrees
    deleteHuffmanTree(node->left);
    deleteHuffmanTree(node->right);

    // Delete the current node
    delete node;
}

//to compress file
void compressFile(const string& inputFile, const string& outputFile) {
    map<char, int> freq;
    calculateFrequencies(inputFile, freq);

    Node* root = buildHuffmanTree(freq);

    map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);
//   for(auto i : huffmanCodes) {
//    cout << i.first << " " << i.second << endl;
//    }
    map<char, std::string>::iterator it;
    for (it = huffmanCodes.begin(); it != huffmanCodes.end(); ++it) {
        std::cout << it->first << " => " << it->second << std::endl;
    }
     // Write the Huffman tree to the output file
    // Assuming writeTreeToFile function writes the Huffman tree to the output file
    writeTreeToFile(outputFile, root);

    // Open the output file in binary mode for writing the compressed data
     ofstream outFile(outputFile, std::ios::out | std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        // Handle error
        return;
    }

    // Write the compressed data to the output file
    // Assuming writeCompressedData function writes the compressed data to the output file
   writeCompressedData(inputFile, outFile, huffmanCodes);

    // Close the output file
    outFile.close();

    // Delete the Huffman tree
   deleteHuffmanTree(root);
}
int main() {
	
    string inputFile = "C:\\Users\\firdo\\OneDrive\\Desktop\\fileCompression\\inputFile.txt.txt";
    string outputFile = "C:\\Users\\firdo\\OneDrive\\Desktop\\fileCompression\\outputFile.txt";
    compressFile(inputFile, outputFile);

    return 0;
}
