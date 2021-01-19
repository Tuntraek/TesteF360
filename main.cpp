#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;

void populate_map(unordered_map<char,int>& m){
    char alphabet[] = {'k', 'b', 'w', 'r', 'q', 'd', 'n', 'f', 'x', 'j',  'm',  'l',  'v',  'h',  't',  'c',  'g',  'z',  'p',  's'};
    for(int i=0; i<20; i++){
        m.insert({alphabet[i], i});
    }
}

int map_char_to_int(char c, unordered_map<char, int>& m){
    auto search = m.find(c);
    return search->second;
}

long long getNumber(unordered_map<char, int>& m, string wordNumber){
    string::iterator c = wordNumber.begin();
    long long totalSum = 0;
    int base = 20;
    int exponent = 0;
    while(c != wordNumber.end()){
        totalSum += map_char_to_int(*c, m) * pow(base, exponent);
        c++;
        exponent++;
    }
    return totalSum;
}

string getWordFromText(ifstream& text){
    char c;

    string word;
    while((c = text.get()) != ' ' && !text.eof()){
        if(c == '\n')   continue;
        word += c;
    }
    return word;
}

// Numeros bonitos

int prettyNumber(vector<string>& words, unordered_map<char, int>& m){

    vector<long> PP;
    long long num = 0;
    int prettyNumber = 0;

    for(const auto& word : words){
        num = getNumber(m, word);

        if( (num >= 440566) && (num%3 == 0) ){
            prettyNumber++;
        }
    }
    return prettyNumber;
}

// Ordenando

bool compareKlingon(const string& s1,const string& s2){
    unordered_map<char, int> m;
    populate_map(m);
    string::const_iterator c1 = s1.begin();
    string::const_iterator c2 = s2.begin();
    while(c1 != s1.end() && c2 != s2.end()){
        if( *c1 == *c2 ){
            c1++; c2++;
            continue;
        }
        if(map_char_to_int(*c1, m) < map_char_to_int(*c2, m))   return true;
        else return false;
    }
    return false;
}

bool sort_klingon(const string& s1,const  string& s2){
    unordered_map<char, int> m;
    populate_map(m);
    return compareKlingon(s1, s2);
}


// Preposição
bool isTypeFoo(char c){
    vector<char> fooType = { 's', 'l', 'f', 'w', 'k'};
    if(find(fooType.begin(), fooType.end(), c) == fooType.end()) return false;
    return true;
}

#define MAX_PREPOSITION_LENGTH 3
int numberOfPrepositions(vector<string>& words){
    int numPrepositions = 0;
    for( const string& word : words){
        if(!(word.length() > MAX_PREPOSITION_LENGTH) &&               //Não ultrapassa 3 letras
           !(isTypeFoo(*(word.end()-1))) &&                           //A ultima letra é do tipo Bar
           (find(word.begin(), word.end(), 'd') == word.end()) )      //E não encontra a letra 'd'
        {
            numPrepositions++;
        }else{
            continue;
        }
    }
    return numPrepositions;
}
// Verbos
void numberOfVerbs(vector<string>& words, int* verbs, int* firstPerson){
    for(const auto& word : words){
        if(word.length() >= 8 && isTypeFoo(*(word.end()-1)) ){
            (*verbs)++;
            if(!(isTypeFoo(*(word.begin())))){
                (*firstPerson)++;
            }
        }
    }
}

int main(){
    //k b w r q d n f x j  m  l  v  h  t  c  g  z  p  s
    //0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
    unordered_map<char, int> mCharToInt;
    populate_map(mCharToInt);

    string fileName = "TextoB.txt";
    ifstream texto;
    texto.open(fileName.c_str());

    if(!texto.is_open()){
        cout << "Arquivo " << fileName << " nao encontrado. Terminando." << '\n';
        exit(1);
    }

    vector<string> words;
    //Preenche o vector
    while(!texto.eof()){
        words.insert(words.end(), getWordFromText(texto));
    }

    texto.close();

    int verbs = 0, verbsInFirstPerson = 0;

    cout << "Numero de preposicoes: " << numberOfPrepositions(words) << '\n';

    numberOfVerbs(words, &verbs, &verbsInFirstPerson);
    cout << "Verbos: " << verbs << " | Verbos em primeira pessoa: " << verbsInFirstPerson << '\n';

    //Ordena e depois remove os duplicados
    sort(words.begin(), words.end(), sort_klingon);
    words.erase(unique(words.begin(), words.end()), words.end());

    for(const auto& word : words){
        cout << word << '\n';
    }

    cout << "Numeros bonitos: " << prettyNumber(words, mCharToInt) << '\n';

    return 0;
}
