#include<bits/stdc++.h>
#include "porterStemmer.cpp"
#include "Get_map.cpp"
using namespace std;
class QueryIndex{
    string line;
    unordered_map<string,unordered_map<int,vector<int>>> table=get_map();
    unordered_map<long int,long int> page_word=get_page_word();
    public:
        int length(string &line){
            int i;
            for(i=0;line[i]!='\0';i++){ }
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
        //to rank the documents on the bases of tf-idf
        vector<int> rank(vector<int> page_id){
            //cout<<"inside the rank"<<endl;
            vector<pair<float,int>> tf_idf;
            //cout<<"page_id size "<<page_id.size()<<endl;
            for(int pi=0;pi<page_id.size();pi++){
                float tf_idf_value=0.0;
                int  i=0;
                while(i<length(line)){
                    int j=i;
                    while(line[j]!=' '){ 
                        if(line[j]=='\0'){break;} 
                        j++;
                    }
                    if(i!=j){
                        string key=line.substr(i,j-i);
                        float tf=1.0*table[key][page_id[pi]].size();
                        tf=tf/(page_word[page_id[pi]]*1.0);
                        float idf=(1.0*page_word.size())/(1.0*table[key].size());
                        idf=log2(idf);
                        tf_idf_value+=(tf*1.0)*(idf*1.0);
                        i=j+1;
                    }else{
                        i++;
                    }
                }
                pair<float,int> x;
                x.first=tf_idf_value;
                x.second=page_id[pi];
                tf_idf.push_back(x);
            }
            sort(tf_idf.begin(),tf_idf.end());
            vector<int> ans;
            for(int t=tf_idf.size()-1;t>=0;t--){
                ans.push_back(tf_idf[t].second);
            }
            return ans;
        }
        void map_element(){
            //  cout<<"inside the map_element\n";
            cout<<"map_element\n";
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
                cout<<itr1->second[n-1]<<";";
                } 
                cout<<endl;
            }
        }
        void parsing(){
        //cout<<"inside the parsing()"<<endl;
            transform(line.begin(), line.end(), line.begin(), ::tolower);
             while(line[0]==' '){
                line.erase(line.begin());
            }
            for(int i=0;line[i]!='\0';i++){
                if(!isalnum(line[i])){
                    if(line[i]=='"'&&(i==0||i==length(line)-1)){
                    }else{
                      line[i]=' ';
                    }
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
            // cout<<"after stop terms"<<line<<endl;
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
            i=0;
            while(line[i+1]!='\0'){
                if(i==0&&line[i]==' '){
                    line.erase(line.begin());
                }else if(line[i]==' '&&line[i+1]==' '){
                    line.erase(line.begin()+i);
                }else{
                i++;
                }
            }
            int n=length(line);
            while(line[n-1]==' '){
                line.erase(line.begin()+n-1);
                n=length(line);
            }
            //cout<<"after the parsing()"<<endl;
            //cout<<"line "<<line<<endl;
            return ;
        }
        int querytype(){
             // cout<<"inside the querytype\n";
            int i=0, n=0;
            if (line[0]=='"'&&line[length(line)-1]=='"'){
                    return 3;
            } else{
                while(line[i]!='\0'){
                    if(line[i]==' '&&i!=length(line)-1){n++;}
                    i++;
                }
                if(n==0){
                    return 1;
                }else{
                    return 2;
                }
            }
        }
        vector<int> get_query(string key){
            // cout<<"inside the get_query\n";
            vector<int> ans;
            if(table.find(key)!=table.end()){
                unordered_map<int,vector<int>>:: iterator itr;
                for(itr=table[key].begin();itr!=table[key].end();itr++){
                    ans.push_back(itr->first);
                }
            }
            //cout<<"end of get_query\n";
            return ans;
        }
        void print_ans(vector<int> ans){
            if(ans.size()==0){
                cout<<"no matching document\n";
            }
            for(int i=0;i<ans.size();i++){
                cout<<ans[i]<<" "<<endl;
            }
            cout<<endl;
        }
        vector<int> owq(){
           // cout<<"inside the owq "<<line<<"\n";
            // parsing();
            vector<int> ans;
            if(line=="\0"){return ans;}
            ans=get_query(line);
            if(ans.size()!=0){
                 ans=rank(ans);
            }
            return ans;
        }
        vector<int> ftq(){
            // cout<<"inside the ftq\n";
            int  i=0;
            if(line[0]=='"'){
                line.erase(line.begin());
            }
            parsing();
            vector<vector<int>> page_id;
            while(i<length(line)){
                int j=i;
                while(line[j]!=' '){ 
                    if(line[j]=='\0'){break;} 
                        j++;
                }
                if(i!=j){
                    string key=line.substr(i,j-i);
                    vector<int> k=get_query(key);
                    sort(k.begin(),k.end());
                    page_id.push_back(k);
                    i=j+1;
                    }else{
                        i++;
                    }
            }
            vector<int> ans=page_id[0];
            // cout<<"ans size"<<ans.size()<<endl;
            for(int i=1;i<page_id.size();i++){
                vector<int> v(ans.size() + page_id[i].size()); 
                vector<int>::iterator it, st; 
                vector<int>  common_pos;
                it = set_intersection(ans.begin(),ans.end(),page_id[i].begin(),page_id[i].end(),v.begin()); 
                for (st = v.begin(); st != it; ++st) {
                    common_pos.push_back( *st); 
                }
                ans=common_pos;
            }
            // cout<<"outside the ftq\n";
            if(ans.size()!=0){
                ans=rank(ans);
            }
            return ans;
        }
        vector<int> pq(){
            //cout<<"inside the pq\n";
            line.erase(line.begin());
            if(line[length(line)-1]=='"'){
            line.erase(line.begin()+length(line)-1);
            }
            parsing();
            vector<int> page_id=ftq();
            //cout<<"common documents in pq "<<page_id.size()<<endl;
            vector<int> ans;
            for(int i=0;i<page_id.size();i++){
                vector<int>  position;
                int word_number=0;
                int j=0;
                while(j<length(line)){
                    int k=j;
                    while(line[k]!=' '){ 
                        if(line[k]=='\0'){break;} 
                        k++;
                    }
                    if(j!=k){
                        string key=line.substr(j,k-j);
                        vector<int> position_1=table[key][page_id[i]];
                        //cout<<position_1.size()<<" size of position of "<<key<<endl;
                        for(int p=0;p<position_1.size();p++){
                            position_1[p]=position_1[p]-word_number;
                            // cout<<position_1[p]<<" ";
                        }
                        sort(position_1.begin(),position_1.end());
                        if(position.size()==0){
                            position=position_1;
                        }else{
                            vector<int> v(position.size() + position_1.size()); 
                            vector<int>::iterator it, st; 
                            vector<int>  common_pos;
                            it = set_intersection(position.begin(),position.end(),position_1.begin(),position_1.end(),v.begin()); 
                            for (st = v.begin(); st != it; ++st) {
                               common_pos.push_back( *st); 
                            }
                            position=common_pos;
                        }
                        // position.push_back(position_1);
                        word_number++;
                        j=k+1;
                    }else{
                        j++;
                    }
                }
                if(position.size()!=0){
                    ans.push_back(page_id[i]);
                }
            }
            if(ans.size()!=0){
                 ans=rank(ans);
            }
            return ans;
        } 
        void query(string x){
            //cout<<"inside the query\n";
            line=x;
            //get_map();
            // map_element();
            parsing();
            int t=querytype();
            //cout<<"type "<<t<<endl;
            if(line=="\0"){return ;}
            vector<int> ans;
            if(t==1){
                ans=owq();
            }else if(t==2){
                ans=ftq();
            }else{
                ans=pq();
            }
            print_ans(ans);
            // cout<<"table size "<<table.size()<<endl;
        }
};
int main(){
    string x;
    QueryIndex q;
    cout<<"to search the sentence in exact order write the sentence inside the double quotation marks"<<endl;
    cout<<"\n enter your query , if you don't want search anymore enetr no queries\n";
    getline(cin,x);
    while(x!="no queries"){
        q.query(x);
        cout<<"enter your query , if you don't want search anymore enetr no queries\n";
        getline(cin,x);
    }
    return 0;
}
