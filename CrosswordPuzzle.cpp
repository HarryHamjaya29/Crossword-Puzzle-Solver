#include <bits/stdc++.h>
#include <fstream>
#include <ctime>
 
#define se second
#define fi first
#define mp makepair
 
using namespace std;
 
vector<string> tts; // tts nya
string kata[1000];
int index=0,panjang=0;
string s;
int n;
map<string, int> cekkata; //pengecekan kata
vector<string> pengelompok[1000]; // pengelompok kata berdasar panjangnya
vector<pair<bool,pair<pair<int, int>,int>>> pengisi;
// kalo bool true maka yang diisi horizontal, kalo bool false maka yang diperhatikan vertical lalu diikuti oleh index (a, b) tempat mulainya
// dan terakhir terisi dengan panjangnya
// cek horizontal dan vertical -> pengisi.fi
// panjang pengisi.se.se
// index (i,j) -> i = pengisi.se.fi.fi, j = pengisi.se.fi.se
bool SudahSelesai = false;
 
void bruteforce(vector<string> tts, map<string, int> cekkata, int idx){
    if(SudahSelesai)
        return;
    if(idx == pengisi.size()){ // ketika ukuran index sama dengan banyak blank berarti pengisian telah selesai dan langsung outputkan
        for(int i=0;i<tts.size(); i++){
            cout<<tts[i]<<"\n";
        }
        SudahSelesai = true;
        return;
    }
    int panjang = pengisi[idx].se.se; // pencarian panjang dari blank
    for(int i=0; i<pengelompok[panjang].size(); i++){ // iterasi untuk setiap kemungkinan kata
        if(cekkata[pengelompok[panjang][i]]==0) //kata telah digunakan :(
            continue;//cek jika bisa diisi di tts
        bool bisa = true;
        bool horizontal=false;
        bool vertikal=false;
        if(pengisi[idx].fi){ //jika horizontal
            for(int j=0;j<panjang;j++){
                if(tts[pengisi[idx].se.fi.fi][j+pengisi[idx].se.fi.se]!='-'&&tts[pengisi[idx].se.fi.fi][j+pengisi[idx].se.fi.se]!=pengelompok[panjang][i][j])
                    bisa = false;
            }
            if(bisa){
                horizontal=true;
            }
        }
        else{   //vertikal
            for(int j=0;j<panjang;j++){
                if(tts[j+pengisi[idx].se.fi.fi][pengisi[idx].se.fi.se]!='-'&&tts[j+pengisi[idx].se.fi.fi][pengisi[idx].se.fi.se]!=pengelompok[panjang][i][j])
                    bisa = false;
            }
            if(bisa){
                vertikal=true;
            }
        }
        if(bisa){ // kalo bisa, diisi
            if(horizontal){
                string temp=""; //temporary untuk kembali ke wujud asal
                for(int j=0;j<panjang;j++){ //ngisi horizontal
                    temp +=tts[pengisi[idx].se.fi.fi][j+pengisi[idx].se.fi.se];
                    tts[pengisi[idx].se.fi.fi][j+pengisi[idx].se.fi.se]=pengelompok[panjang][i][j];
                }
                cekkata[pengelompok[panjang][i]] -= 1; //kurangi satu, karna kepake
                bruteforce(tts, cekkata, idx+1);//kembalikkan ke asal untuk iterasi selanjutnya
                cekkata[pengelompok[panjang][i]] += 1; //tambahkan satu, karna ga jadi pake
                for(int j=0;j<panjang;j++){ //kembaikan keasal ttsnya
                    tts[pengisi[idx].se.fi.fi][j+pengisi[idx].se.fi.se]=temp[j];
                }
            }
            else{
                string temp=""; //temporary untuk kembali ke wujud asal
                for(int j=0;j<panjang;j++){ //ngisi horizontal
                    temp +=tts[j+pengisi[idx].se.fi.fi][pengisi[idx].se.fi.se];
                    tts[j+pengisi[idx].se.fi.fi][pengisi[idx].se.fi.se]=pengelompok[panjang][i][j];
                }
                cekkata[pengelompok[panjang][i]] -= 1; //kurangi satu, karna kepake
                bruteforce(tts, cekkata, idx+1);//kembalikkan ke asal untuk iterasi selanjutnya
                cekkata[pengelompok[panjang][i]] += 1; //tambahkan satu, karna ga jadi pake
                for(int j=0;j<panjang;j++){ //kembaikan keasal ttsnya
                    tts[j+pengisi[idx].se.fi.fi][pengisi[idx].se.fi.se]=temp[j];
                }
            }
        }
        if(SudahSelesai)
            return;
    }
}
 
int main(){
    string answerlist;
    ifstream infile;
    infile.open("Test1.txt");
    infile >> n;
    for(int i=0; i<n; i++){
        infile>>s;
        tts.push_back(s);
    }
    infile>>answerlist;
    infile.close();
    int row,col;
	//Mencari semua blank yang berbentuk horizontal
    for(row=0;row<tts.size();row++){
        for(col=0;col<tts[row].size();col++){
            if(tts[row][col]=='-'&& ((col>=1 && tts[row][col-1]=='#')||col==0))
                panjang++;
            else if(panjang && tts[row][col]=='-')
                panjang++;
            else{
                if(panjang>1){
                    pengisi.push_back({true,{{row,col-panjang},panjang}});
                }
                panjang=0;
            }
        }
        if(panjang>1){
            pengisi.push_back({true,{{row,col-panjang},panjang}});
        }
        panjang=0;
    }
	//Mencari semua blank yang berbentuk vertikal
    for(col=0;col<tts.size();col++){
        for(row=0;row<tts[col].size();row++){
            if(tts[row][col]=='-' && ((row>=1 && tts[row-1][col]=='#')||row==0))
                panjang++;
            else if(panjang && tts[row][col]=='-')
                panjang++;
            else{
                if(panjang>1){
                    pengisi.push_back({false,{{row-panjang,col},panjang}});
                }
                panjang=0;
            }
        }
        if(panjang>1){
            pengisi.push_back({false,{{row-panjang,col},panjang}});
        }
        panjang=0;
    }
	//parsing string menjadi list kata kunci
    int sollength=answerlist.length(),temp=0;
    while(temp<sollength){
        if(answerlist[temp]==';' || answerlist[temp]=='\n'){
            index++;
        }
        else{
            kata[index]+=answerlist[temp];
        }
        temp++;
    }
    for(int i=0; i<index; i++){
        cekkata[kata[i]]+=1;// menghitung banyak kata[i];
		pengelompok[kata[i].size()].push_back(kata[i]);// Melakukan pengelompokkan kata
	}
	const clock_t begin_time = clock();
    bruteforce(tts, cekkata, 0); // 0 adalah indeks dari elemen pertama pengisian
	cout<< float(clock()-begin_time) / CLOCKS_PER_SEC;
    return 0;
}