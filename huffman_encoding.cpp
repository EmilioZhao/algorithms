
#include <iostream>
#include <vector>
#include <queue> //for priority_queue
#include <map>

#include <stdint.h>

using namespace std;

// basic data structore for huffman encoding
struct Node {
    char val;
    uint32_t freq;
    Node* p_left;
    Node* p_right;
    Node (char c, uint32_t f) : val(c), freq(f), p_left(nullptr), p_right(nullptr) {}
    Node (char c, uint32_t f, Node* p1, Node* p2) :
        val(c), freq(f), p_left(p1), p_right(p2) {}
};

typedef struct Node Node;

// count every character's occurrences 
// @Args s: string to be parsed 
// @Args m: occurrences map
void char_count(string& s, map<char, uint32_t>& m) {
    if (!s.empty()) {
        for (auto c : s) {
            m[c]++;
        }
    }
}

//comparison functor for Node*
struct cmp {
    bool operator() (Node* p1, Node* p2) {
        return p1->freq > p2->freq;
    }
};

// Build Huffman encoding tree
// @Args src: string to be encoded
// @Return Node* node 
Node* Huffman_tree(string& src, map<char, uint32_t>& m) {
    if (src.empty()) {
        return nullptr;
    }
    
    char_count(src, m);
    
    
    //construct min_heap ordered by character occurrences
    //priority_queue<Node*, vector<Node*>, cmp> min_heap; //Solution1: use functor
    
    //Solution2: lambda expression for comparison
    constexpr auto lambda_cmp = [](Node* p_1, Node* p_2) {
        return p_1->freq > p_2->freq;
    }; //lambda expr has no default constructor, so we need to pass it in arguments
    
    priority_queue<Node*, vector<Node*>, decltype(lambda_cmp)> min_heap(lambda_cmp);
    for (auto& p : m) {
        min_heap.push(new Node(p.first, p.second));
    }
    
    //build tree
    while (min_heap.size() > 1) {
        Node* p_1 = min_heap.top(); min_heap.pop();
        Node* p_2 = min_heap.top(); min_heap.pop();
        //add && connect
        min_heap.push(new Node('0', p_1->freq + p_2->freq, p_1, p_2)); 
    }
    
    return min_heap.top();
}

// print encoding recursively, store result in encoding_map 
void print(Node* p_head, string& s, map<char, string>& encoding_map) {
    if (nullptr == p_head) {
        return;
    }
    
    if (nullptr == p_head->p_left && nullptr == p_head->p_right) {
        //cout << p_head-> val << ": " << s << endl;
        encoding_map[p_head-> val] = s;
        return;
    }
    
    if (p_head->p_left) {
        s.push_back('0');
        print(p_head->p_left, s, encoding_map);
        s.pop_back();
    }
    
    if (p_head->p_right) {
        s.push_back('1');
        print(p_head->p_right, s, encoding_map);
        s.pop_back();
    }
    
    return;
}

int main() {
    string src("Hello World! No tressspassing! What do you want from me?");
    map<char, uint32_t> occurrences_map;
    Node* p_head = Huffman_tree(src, occurrences_map);
    
    string s;
    map<char, string> encoding_map;
    print(p_head, s, encoding_map);
    
    cout << "size: " << encoding_map.size() << endl;
    for (auto pair : encoding_map) {
        cout << pair.first << ": " << pair.second
            << "\t occurrences: " << occurrences_map[pair.first] << endl;
    }
    
    return 0;
}
