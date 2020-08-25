#include<bits/stdc++.h>
#include "porterStemmer.cpp"
using namespace std;
class CreateIndex{
    map<string,map<int,vector<int>>> table;
    public:
    int length(string &line){
      //  cout<<"inside the length()"<<endl;
        int i;
        for(i=0;line[i]!='\0';i++){
        }
      // cout<<"length= "<<i<<endl;
        return i;
    } 
    void parsing(string line,int line_number,long int id){
       // cout<<"inside the parsing()"<<endl;
        //  cout<<"before get terms"<<line<<endl;
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
          lb: if(pos!= std::string::npos){
              int n=length(stopword);
            
                 //cout<<stopword<<" "<<pos<<endl;
              if(n>0){
                  if(pos==0||line[pos-1]==' '){
                        //cout<<stopword<<endl;
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
       // cout<<"after stop terms"<<line<<endl;
        
    int  i=0;
        while(i<length(line)){
            int j=i;
           // cout<<i<<endl;
            while(line[j]!=' '){ 
                if(line[j]=='\0'){break;} 
                // if(line_number==7){cout<<j<<endl; }
                j++;
                }
           // if(line_number==7){ cout<<i<<" "<<j<<endl;}
                        if(i!=j){
                  PorterStemmer p;
              // if(line_number==7){ cout<<"going innside the porter"<<endl;
             //  cout<<line.substr(i,j-i)<<endl;}
               
                string ans=p.StemWord(line.substr(i,j-i));
              //    if(line_number==7){cout<<"going outside the porter"<<endl;}
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
               //  if(line_number==7){cout<<"i "<<i<<endl;}
            }else{
                i++;
            }
        }
     //  cout<<"end of parsing "<<line<<endl;
          get_terms(line,line_number,id);
        return ;
    }
    long int getid(string line){
       // cout<<"inside the getid()"<<endl;
            line=line.substr(4);
            int n=length(line);
            line=line.substr(0,n-5);
            long int  id=0,i=0;
            while(line[i]!='\0'){
                    id=id*10+line[i]-'0';
                    i++;
            }
          //  cout<<"id"<<id<<endl;
            return id;
    }
    string get_title(string line){
           // cout<<"inside the get_title()"<<endl;
             line=line.substr(7);
            int n=length(line);
            line=line.substr(0,n-8);
         //  cout<<"title"<<line<<endl;
            return line;
    }
    void map_element(){
      map<string,map<int,vector<int>>>::iterator itr;
        for(itr=table.begin();itr!=table.end();itr++){
            map<int,vector<int>>::iterator itr1;
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
    void write_tofile(){
      //  map_element();
        //cout<<"inside the write_tofile()"<<endl;
        fstream  f;
        f.open("result1.txt",ios::out);
        int  u=table.size();
       // cout<<"map size "<<u<<endl;
        map<string,map<int,vector<int>>>::iterator itr;
        for(itr=table.begin();itr!=table.end();itr++){
          //  int m=itr->second.size();
            map<int,vector<int>>::iterator itr1;
            string x=itr->first;
            cout<<x<<endl;
             f<<x<<"|";
            for(itr1=itr->second.begin();itr1!=itr->second.end();itr1++){
                f<<itr1->first<<":";
                int n=itr1->second.size();
                for(int i=0;i<n-1;i++){
                      f<<itr1->second[i]<<",";
                }
                f<<itr1->second[n-1];
            } 
            f<<endl;
        }
      // if(!f){cout<<"not created file"<<endl;}
        f.close();

    }
    void get_terms(string line,int line_number,long int id){
     //   cout<<"inside the get_terms()"<<endl;
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
                    map<int,vector<int>> ans;
                    vector<int> line_vec;
                    line_vec.push_back(line_number);
                    ans[id]=line_vec;
                    table[key]=ans;
                }else{
                    table[key][id].push_back(line_number);
                }
                i=j+1;
            }else{
                i++;
            }
        }
    }
    void createIndex(){
        fstream f;
       // cout<<"inside the class"<<endl;
        string line;
         string title;
         int  line_number=0;
        long int id;
        f.open("testCollection.txt");
        getline(f,line);
        while(line!= "</collection>"||length(line)==0){
               getline(f,line);
               if(line=="<page>"){
                      getline(f,line);
                      id=getid(line);
                       getline(f,line);
                       title=get_title(line);
                        getline(f,line);
               }
               int i=4;
                
               parsing(title,i++,id);
             //  cout<<"text handling\n";
               line=line.substr(6);   
                while(line!="</page>"){
                    if (line.find("/text") != std::string::npos) {
                    int n=length(line);
                    line=line.substr(0,n-7);
                    }
                   // cout<<"line number "<<i<<endl;  
                    parsing(line,i++,id);
                     // get_terms(line,line_number);
                     getline(f,line);

                }

                getline(f,line);
        }
        f.close();
        write_tofile();
    }
};
int  main(){
    cout<<"inside the main()"<<endl;
  
    CreateIndex c;
    c.createIndex();
    cout<<"back to main()"<<endl;
      fstream  f;
      f.open("result1.txt");
    string line;
   if(!f){
        cout<<"file doesn't exist"<<endl;
    }else{
        cout<<"opened"<<endl;
    }
   /*  getline(f,line);
    while(!f.eof()){
        cout<<line<<endl;
         getline(f,line);
    }*/
    f.close();
    return 0;
}