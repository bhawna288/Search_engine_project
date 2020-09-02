#include<bits/stdc++.h>
using namespace std;
  int length(string &line){
      int i;
      for(i=0;line[i]!='\0';i++){  }
      return i;
  }
  long int getint(string line){      
      long int  id=0,i=0;
      while(line[i]!='\0'){
          id=id*10+line[i]-'0';
          i++;
      }
      return id;
    }
    //to convert the page_word.txt file into a map
  map<long int,long int> get_page_word(){
      map<long int,long int> page_word;
      //cout<<"inside the get_page\n";
      fstream f;
      f.open("page_words.txt");
      while(!f.eof()){
          string x;
          getline(f,x);
          int pos=x.find(":");
          string key=x.substr(0,pos);
          long int page_number=getint(key);
          x=x.substr(pos+1);
          long int i=getint(x);
          page_word[page_number]=i;
      }
      //cout<<"out of get_page_word\n"<<endl;
      f.close();
      return page_word;
  }
  // to convert the index file into a map
  map<string,map<int,vector<int>>> get_map(){
      map<string,map<int,vector<int>>> table;
      //cout<<"inside the get_map\n";
      int line_number=0;
      fstream f;
      f.open("index.txt");
      while(!f.eof()){
          string x;
          getline(f,x);
          int pos=x.find("|");
          string key=x.substr(0,pos);
          x=x.substr(pos);
          map<int,vector<int>> ans;
          int i=0;
          while(length(x)>1){
              i=0;
              i++;
              x=x.substr(i);
              pos=x.find(':');
              string l=x.substr(0,pos);
              long int l1=getint(l);
              vector<int> l2;
              x=x.substr(pos);
              i=0;
              while(x[i]!=';'){
                  i++;
                  x=x.substr(i);
                  i=0;
                  while(x[i]!=','&&x[i]!=';'){
                      i++;
                  }
                  string l3=x.substr(0,i);
                  x=x.substr(i);
                  long int l4=getint(l3);
                  l2.push_back(l4);
                  i=0;
              }
              ans[l1]=l2;
          }
          table[key]=ans;
      }
      //cout<<"out of file\n"<<endl;
      return table;
  }