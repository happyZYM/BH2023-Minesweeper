#include <unordered_map>
#include <unordered_set>

#include "data.h"
#include "zb64.h"
namespace DataLoad {
typedef long long LL;
const int buf_size = 4412555 * 4;
unsigned char buf[buf_size], data[buf_size];
bool already_have[14348907];
// std::unordered_map<LL,unsigned char> visible_to_probability;
int rid[15] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2};
int cid[15] = {0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4};
class HashTable
{
	struct Node
	{
		LL key;
		unsigned char value;
		Node* next;
		Node(LL k,unsigned char v):key(k),value(v),next(nullptr){}
	};
	Node* table[14348907];
public:
	HashTable()
	{
		for(int i=0;i<14348907;i++)
			table[i]=nullptr;
	}
	~HashTable()
	{
		for(int i=0;i<14348907;i++)
		{
			Node* p=table[i];
			while(p)
			{
				Node* q=p->next;
				delete p;
				p=q;
			}
		}
	}
	unsigned char& operator[](LL key)
	{
		int index=key%14348907;
		Node* p=table[index];
		while(p)
		{
			if(p->key==key)
				return p->value;
			p=p->next;
		}
		p=new Node(key,0);
		p->next=table[index];
		table[index]=p;
		return p->value;
	}
	bool have(LL key)
	{
		int index=key%14348907;
		Node* p=table[index];
		while(p)
		{
			if(p->key==key)
				return true;
			p=p->next;
		}
		return false;
	}
}visible_to_probability;
}  // namespace DataLoad
void LoadData() {
  using namespace DataLoad;
  size_t raw_length = base64_decode(pre_calc_prob, buf, buf_size);
  size_t data_length = decompressData(buf, raw_length, data, buf_size);
//   std::cout<<"decompress finished.\n"<<std::endl;
//   already_have.rehash(4412555);
//   visible_to_probability.rehash(4412555);
  const LL raw_line_base = 243;
  const LL vis_line_base = 100000;
  int cnt=0;
  for (int status = 0; status < 14348907; status++) {
    LL inverse_status =
        (status % raw_line_base) * raw_line_base * raw_line_base +
        ((status / raw_line_base) % raw_line_base) * raw_line_base +
        (status / (raw_line_base * raw_line_base));
    if (already_have[inverse_status]) continue;
    // assert(already_have.find(status) == already_have.end());
    already_have[status]=true;
    int inner_mp[3][5] = {0}, visible_map[3][5];
    LL status_tmp = status;
    for (int i = 0; i < 15; i++) {
      int row = rid[i], col = cid[i];
      inner_mp[row][col] = (status_tmp % 3);  // uncode the inner_status
      status_tmp /= 3;
    }
    for (int row = 0; row < 3; row++)
      for (int col = 0; col < 5; col++) {
        if (inner_mp[row][col] == 0 || inner_mp[row][col] == 1) {
          visible_map[row][col] = 9;  // 9 means unshown to player
        } else {
          int mcnt = 0;
          const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1},
                    dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
          for (int i = 0; i < 8; i++) {
            int nr = row + dx[i], nc = col + dy[i];
            if (nr < 0 || nr >= 3 || nc < 0 || nc >= 5) continue;
            mcnt += (inner_mp[nr][nc] == 0 ? 1 : 0);
          }
          visible_map[row][col] = mcnt;
        }
      }
    LL visible_status = 0;
    for (int i = 0; i < 15; i++) {
      int row = rid[i], col = cid[i];
      visible_status = visible_status * 10 + visible_map[row][col];
    }
	visible_to_probability[visible_status] = data[cnt++];
  }
//   std::cout<<"Load data finished.\n"<<std::endl;
}