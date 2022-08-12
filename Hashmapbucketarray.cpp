#include<iostream>
#include<string>
using namespace std;
template<typename V>
class Mapnode{
public:
    string key;
    V value;
    Mapnode *next;
    Mapnode(string key,V value){
         this->key=key;
         this->value=value;
         next=NULL;
    }
    ~Mapnode(){
        delete next;
    }
};
template <typename V>
class ourmap{
    Mapnode<V>** buckets;
    int size;
    int numbuckets;
    int count;
public:
    ourmap(){
        count=0;
        numbuckets=5;
        buckets=new Mapnode<V> *[numbuckets];
        for(int i=0;i<numbuckets;i++){
            buckets[i]=NULL;
        }
    }
    ~ourmap(){
        for(int i=0;i<numbuckets;i++){
            delete buckets[i];
        }
        delete[] buckets;
    }
    int sizes(){
        return count;
    }

private:
    int getbucketindex(string key){
        int hashcode=0;
        int currcoeff=1;
        for(int i=key.length()-1;i>=0;i--){
            hashcode+=key[i]*currcoeff;
            hashcode=hashcode%numbuckets;
            currcoeff*=37;
            currcoeff=currcoeff%numbuckets;
        }
        return hashcode%numbuckets;
    }
    void rehash(){
        Mapnode<V> **temp=buckets;
        buckets=new Mapnode<V> *[2*numbuckets];
        for(int i=0;i<2*numbuckets;i++){
            buckets[i]=NULL;
        }
        int oldbucketcount=numbuckets;
        numbuckets*=2;
        count=0;
        for(int i=0;i<oldbucketcount;i++){
            Mapnode<V>* head=temp[i];
            while(head!=NULL){
                string key=head->key;
                V value=head->value;
                insert(key,value);
                head=head->next;
            }
        }
        for(int i=0;i<oldbucketcount;i++){
            Mapnode<V>* head=temp[i];
            delete head;
        }
        delete[] temp;
    }
public:
    double getloadfactor(){
        return (1.0*count)/numbuckets;
    }
public:
    void insert(string key,V value){
        int bucketindex=getbucketindex(key);
        Mapnode<V> *head=buckets[bucketindex];
        while(head!=NULL){
            if(head->key==key){
                head->value=value;
                return;
            }
            head=head->next;
        }
        head=buckets[bucketindex];
        Mapnode<V> *node=new Mapnode<V>(key,value);
        node->next=head;
        buckets[bucketindex]=node;
        count++;
        double loadfactor=(1.0*count)/numbuckets;
        if(loadfactor>0.7){
            rehash();
        }
    }
    V remove(string key){
        int bucketindex=getbucketindex(key);
        Mapnode<V> *head=buckets[bucketindex];
        Mapnode<V> *prev=NULL;
        while(head!=NULL){
            if(head->key==key){
                if(prev==NULL){
                    buckets[bucketindex]=head->next;
                }
                else{
                    prev->next=head->next;
                }
                V value=head->value;
                head->next=NULL;
                delete head;
                count--;
                return value;
            }
            prev=head;
            head=head->next;
        }
        return 0;
    }
    V getvalue(string key){
        int bucketindex=getbucketindex(key);
        Mapnode<V> *head=buckets[bucketindex];
        while(head!=NULL){
            if(head->key==key){
                return head->value;
            }
            head=head->next;
        }
        return 0;
    }
};

int main(){
    ourmap<int> map;
    for(int i=0;i<10;i++){
        char c='0'+i;
        string key="abc";
        key=key+c;
        int value=i+1;
        map.insert(key,value);
        cout<<map.getloadfactor()<<endl;
    }
        for(int i=0;i<10;i++){
            char c='0'+i;
            string key="abc";
            key=key+c;
            cout<<key<<":"<<map.getvalue(key)<<endl;
        }
        cout<<map.sizes()<<endl;
        map.remove("abc2");
        map.remove("abc7");
        cout<<map.sizes()<<endl;
}
