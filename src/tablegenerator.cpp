#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "zb64.h"

using namespace std;
typedef long long LL;
int rid[15] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2};
int cid[15] = {0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4};
// unordered_map<LL,LL> visible_to_inner;
unordered_map<LL, vector<LL>> visible_to_inner;
unordered_map<LL, LL> inner_to_visible;
vector<LL> valid_visible_status;
unordered_map<LL, double> visible_to_probability;
unordered_set<LL> already_have;
void FindStatus() {
  const LL raw_line_base = 243;
  const LL vis_line_base = 100000;
  for (int status = 0; status < 14348907; status++) {
    int inner_mp[3][5] = {0}, visible_map[3][5];
    LL status_tmp = status;
    LL inverse_status =
        (status % raw_line_base) * raw_line_base * raw_line_base +
        ((status / raw_line_base) % raw_line_base) * raw_line_base +
        (status / (raw_line_base * raw_line_base));
    if (already_have.find(inverse_status) != already_have.end()) continue;
    assert(already_have.find(status) == already_have.end());
    already_have.insert(status);
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
    inner_to_visible[status] = visible_status;
    visible_to_inner[visible_status].push_back(status);
  }
}
const int buf_size = 4412555 * 4;
unsigned char buf[buf_size], buf2[buf_size], buf3[buf_size], buf4[buf_size];
int bcnt = 0;
void CalculateProbability() {
  for (auto it = visible_to_inner.begin(); it != visible_to_inner.end(); ++it) {
    assert(it->second.size() > 0);
    int mine_cnt = 0;
    for (int i = 0; i < it->second.size(); i++) {
      mine_cnt += ((it->second[i] / 729) % 3 == 0 ? 1 : 0);
    }
    visible_to_probability[it->first] = double(mine_cnt) / it->second.size();
    buf[bcnt++] = int((double(mine_cnt) / it->second.size()) * 255);
  }
}
int main() {
  FindStatus();
  cout << visible_to_inner.size() << endl;
  CalculateProbability();
  freopen("tmp/data.txt", "w", stdout);
  string raw = base64_encode(buf, bcnt, false);
  cout << raw << endl;
  freopen("tmp/compressed.txt", "w", stdout);
  // buf is the raw data;
  // buf2 is the compressed data;
  // compressed is the base64-encoded form of buf2;
  // buf3 is the base64_decoded form of compressed,it should be the same as
  // buf2; buf4 is the decompressed data, it should be the same as buf;
  size_t real_size = compressData(buf, bcnt, buf2, buf_size);
  string compressed = base64_encode(buf2, real_size, false);
  cout << compressed << endl;
  // check the correctness of the compression and base64 encoding
  freopen("tmp/decompressed.txt", "w", stdout);
  size_t real_size3 = base64_decode(compressed, buf3, buf_size, false);
  for (int i = 0; i < real_size3; i++) assert(buf3[i] == buf2[i]);
  size_t real_size4 = decompressData(buf3, real_size3, buf4, buf_size);
  for (int i = 0; i < real_size4; i++) assert(buf4[i] == buf[i]);
  return 0;
}