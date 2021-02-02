#include<bits/stdc++.h>
#include "porterStemmer.cpp"
using namespace std;
class CreateIndex{
    //to store the key(string) and in which document it present  and at what position.
    unordered_map<string,unordered_map<int,vector<int>>> table;
    unordered_map<int,int> page_word;
    int  word_number=0;
    public:
      //to find the length od the string
      int length(string line){
          int i;
          for(i=0;line[i]!='\0';i++){ }
          return i;
      } 
      //to remove the stopwords and put spaces instead of non-alphanumeric characters 
      //  use Porter Stemmer to remove common endings from words. For example the stemmed version of the words fish, fishes, fishing, fisher, fished are all fish.
      void parsing(string line,long int id){
          //cout<<"inside the parsing()"<<endl;
          transform(line.begin(), line.end(), line.begin(), ::tolower);
          for(int i=0;line[i]!='\0';i++){
              if(!isalnum(line[i])){
                  line[i]=' ';
              }
          }
          fstream f;
          f.open("stopwords.txt");
          string stopword;
          getline(f,stopword);
          while(!f.eof()){
              // cout<<"inside the stopword file\n";
              size_t pos=line.find(stopword);
             lb:if(pos!= std::string::npos){
                    int n=length(stopword);
                    if(n>0){
                        if(pos==0||line[pos-1]==' '){
                            if(line[pos+n]==' '||line[pos+n]=='\0'){
                                line.erase(pos,n);
                            } 
                        }
                    }
                    pos=line.find(stopword,pos+1);
                    goto lb;
                }else{
                    getline(f,stopword);
                }
          } 
          int  i=0;
          while(i<length(line)){
              int j=i;
              while(line[j]!=' '){ 
                  if(line[j]=='\0'){break;} 
                  j++;
              }
              if(i!=j){
                  PorterStemmer p;
                  string ans=p.StemWord(line.substr(i,j-i));
                  int z=0;
                  int ans_length=length(ans);
                  for(int k=i;k<j;k++){
                      if(z<ans_length){
                          line[k]=ans[z];
                          z++;
                      }else{
                          break;
                      }
                  }
                  i=j+1;
              }else{
                  i++;
              }
          }
          get_terms(line,id);
          return ;
      }
      long int getid(string line){
          //cout<<"inside the getid()"<<endl;
          line=line.substr(4);
          int n=length(line);
          line=line.substr(0,n-5);
          long int  id=0,i=0;
          while(line[i]!='\0'){
              id=id*10+line[i]-'0';
              i++;
          }
          return id;
      }
      string get_title(string line){
          //cout<<"inside the get_title()"<<endl;
          line=line.substr(7);
          int n=length(line);
          line=line.substr(0,n-8);
          return line;
      }
      void map_element(){
          unordered_map<string,unordered_map<int,vector<int>>>::iterator itr;
          for(itr=table.begin();itr!=table.end();itr++){
              unordered_map<int,vector<int>>::iterator itr1;
              cout<<itr->first<<"|";
              for(itr1=itr->second.begin();itr1!=itr->second.end();itr1++){
                  cout<<itr1->first<<":";
                  int n=itr1->second.size();
                  for(int i=0;i<n-1;i++){
                      cout<<itr1->second[i]<<",";
                  }
                  cout<<itr1->second[n-1];
              } 
              cout<<endl;
          }
      }
      // to  make a file to store the value of map we created 
      void write_tofile(){
          // map_element();
          //cout<<"inside the write_tofile()"<<endl;
          fstream  f;
          f.open("index.txt",ios::out);
          int  u=table.size();
          unordered_map<string,unordered_map<int,vector<int>>>::iterator itr;
          for(itr=table.begin();itr!=table.end();itr++){
              unordered_map<int,vector<int>>::iterator itr1;
              string x=itr->first;
              f<<x<<"|";
              for(itr1=itr->second.begin();itr1!=itr->second.end();itr1++){
                  int l=itr->second.size();
                  f<<itr1->first<<":";
                  int n=itr1->second.size();
                  for(int i=0;i<n-1;i++){
                      f<<itr1->second[i]<<",";
                  }
                  f<<itr1->second[n-1]<<";";
              } 
              f<<endl;
          }
          f.close();
          f.open("page_words.txt",ios::out);
          // int  u=page_word.size();
          //cout<<"map size "<<page_word.size()<<endl;
          unordered_map<int,int>::iterator itr100;
          for(itr100=page_word.begin();itr100!=page_word.end();++itr100){
              f<<itr100->first<<":"<<itr100->second<<endl;
          }
          f.close();
      }
      //to create the index of the word, in other words to fill the value in map for a key(word)
      void get_terms(string line,long int id){
          //cout<<"inside the get_terms()"<<endl;
          int  i=0;
          while(i<length(line)){
              int j=i;
              while(line[j]!=' '){ 
                  if(line[j]=='\0'){break;} 
                  j++;
              }
              if(i!=j){
                  string key=line.substr(i,j-i);
                  if(table.find(key)==table.end()){
                      unordered_map<int,vector<int>> ans;
                      vector<int> line_vec;
                      line_vec.push_back(word_number++);
                      ans[id]=line_vec;
                      table[key]=ans;
                  }else{
                      if(table[key].find(id)==table[key].end()){      
                          vector<int> line_vec;
                          line_vec.push_back(word_number++);
                          table[key][id]=line_vec;      
                      }else{
                          table[key][id].push_back(word_number++);
                      }
                  }
                  i=j+1;
              }else{
                  i++;
              }
          }
      }
      void createIndex(){
          fstream f;
          //cout<<"inside the class"<<endl;
          string line;
          string title;
          long int id;
          f.open("testCollection.txt");
          getline(f,line);
          getline(f,line);
          while(line!= "</collection>"||length(line)==0){
              word_number=1;
              //getline(f,line);
              if(line=="<page>"){
                  getline(f,line);
                  id=getid(line);
                  getline(f,line);
                  title=get_title(line);
                  getline(f,line);
              }
              int i=1;
              parsing(title,id);
              line=line.substr(6);   
              while(line!="</page>"){
                  if (line.find("/text") != std::string::npos) {
                      int n=length(line);
                      line=line.substr(0,n-7);
                  } 
                  parsing(line,id);
                  // get_terms(line,line_number);
                  getline(f,line);
              }
              page_word[id]=word_number;
              getline(f,line);
          }
          f.close();
          write_tofile();
      }
};
int main(){
   // cout<<"inside the main()\n";
    CreateIndex c;
    c.createIndex();
    return 0;
}
