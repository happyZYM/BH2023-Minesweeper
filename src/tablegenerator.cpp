#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "miniz.h"

/**
 * The following code is copied from
 * https://github.com/ReneNyffenegger/cpp-base64 and modified by me.
 */
static const char *base64_chars[2] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "+/",

    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "-_"};

static unsigned int pos_of_char(const unsigned char chr) {
  //
  // Return the position of chr within base64_encode()
  //

  if (chr >= 'A' && chr <= 'Z')
    return chr - 'A';
  else if (chr >= 'a' && chr <= 'z')
    return chr - 'a' + ('Z' - 'A') + 1;
  else if (chr >= '0' && chr <= '9')
    return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
  else if (chr == '+' || chr == '-')
    return 62;  // Be liberal with input and accept both url ('-') and non-url
                // ('+') base 64 characters (
  else if (chr == '/' || chr == '_')
    return 63;  // Ditto for '/' and '_'
  else
    //
    // 2020-10-23: Throw std::exception rather than const char*
    //(Pablo Martin-Gomez, https://github.com/Bouska)
    //
    throw std::runtime_error("Input is not valid base64-encoded data.");
}

std::string base64_encode(unsigned char const *bytes_to_encode, size_t in_len,
                          bool url) {
  size_t len_encoded = (in_len + 2) / 3 * 4;

  unsigned char trailing_char = url ? '.' : '=';

  //
  // Choose set of base64 characters. They differ
  // for the last two positions, depending on the url
  // parameter.
  // A bool (as is the parameter url) is guaranteed
  // to evaluate to either 0 or 1 in C++ therefore,
  // the correct character set is chosen by subscripting
  // base64_chars with url.
  //
  const char *base64_chars_ = base64_chars[url];

  std::string ret;
  ret.reserve(len_encoded);

  unsigned int pos = 0;

  while (pos < in_len) {
    ret.push_back(base64_chars_[(bytes_to_encode[pos + 0] & 0xfc) >> 2]);

    if (pos + 1 < in_len) {
      ret.push_back(base64_chars_[((bytes_to_encode[pos + 0] & 0x03) << 4) +
                                  ((bytes_to_encode[pos + 1] & 0xf0) >> 4)]);

      if (pos + 2 < in_len) {
        ret.push_back(base64_chars_[((bytes_to_encode[pos + 1] & 0x0f) << 2) +
                                    ((bytes_to_encode[pos + 2] & 0xc0) >> 6)]);
        ret.push_back(base64_chars_[bytes_to_encode[pos + 2] & 0x3f]);
      } else {
        ret.push_back(base64_chars_[(bytes_to_encode[pos + 1] & 0x0f) << 2]);
        ret.push_back(trailing_char);
      }
    } else {
      ret.push_back(base64_chars_[(bytes_to_encode[pos + 0] & 0x03) << 4]);
      ret.push_back(trailing_char);
      ret.push_back(trailing_char);
    }

    pos += 3;
  }

  return ret;
}
/**
 * @brief The definition of function base64_decode
 * @param const std::string& string_to_decode The string to decode
 * @param unsigned char const * bytes_to_decode The decoded string
 * @param size_t max_len The max length of the decoded string
 * @param bool url If true, then the url base64 alphabet is used
 * @return size_t The real length of the decoded string
 */
size_t base64_decode(const std::string &string_to_decode,
                     unsigned char *const bytes_to_decode, size_t max_len,
                     bool url = false) {
  size_t real_length = 0;
  unsigned char trailing_char = url ? '.' : '=';
  const char *base64_chars_ = base64_chars[url];
  size_t len = string_to_decode.length();
  for (size_t i = 0; i < len; i += 4) {
    unsigned int pos0 = pos_of_char(string_to_decode[i + 0]);
    unsigned int pos1 = pos_of_char(string_to_decode[i + 1]);
    bytes_to_decode[real_length++] = ((pos0 << 2) + ((pos1 & 0x30) >> 4));
    if (string_to_decode[i + 2] != trailing_char) {
      unsigned int pos2 = pos_of_char(string_to_decode[i + 2]);
      bytes_to_decode[real_length++] =
          (((pos1 & 0x0f) << 4) + ((pos2 & 0x3c) >> 2));
      if (string_to_decode[i + 3] != trailing_char) {
        unsigned int pos3 = pos_of_char(string_to_decode[i + 3]);
        bytes_to_decode[real_length++] = (((pos2 & 0x03) << 6) + pos3);
      }
    }
  }
  return real_length;
}
using namespace std;
typedef long long LL;
int rid[15] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2};
int cid[15] = {0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4};
// unordered_map<LL,LL> visible_to_inner;
unordered_map<LL, vector<LL>> visible_to_inner;
unordered_map<LL, LL> inner_to_visible;
vector<LL> valid_visible_status;
unordered_map<LL, double> visible_to_probability;
void dfs(int depth, LL status) {
  // if (depth == 15) {
  for (int status = 0; status < 14348907; status++) {
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
    inner_to_visible[status] = visible_status;
    visible_to_inner[visible_status].push_back(status);
  }
  //   return;
  // }
  // dfs(depth + 1, status * 3 + 0);  // 0 = mine and unclicked
  // dfs(depth + 1, status * 3 + 1);  // 1 = no mine and unclicked
  // dfs(depth + 1, status * 3 + 2);  // 2 = no mine and clicked
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
size_t compressData(const unsigned char *inputData, size_t inputSize,
                    unsigned char *compressedData, size_t compressedSize) {
  mz_ulong compressedSizeOut = compressedSize;
  int compressResult = mz_compress2(compressedData, &compressedSizeOut,
                                    inputData, inputSize, MZ_BEST_COMPRESSION);

  if (compressResult != Z_OK) {
    throw(std::runtime_error("Compression failed."));
  }

  return compressedSizeOut;
}

size_t decompressData(const unsigned char *compressedData,
                      size_t compressedSize, unsigned char *decompressedData,
                      size_t decompressedSize) {
  mz_ulong decompressedSizeOut = decompressedSize;
  int decompressResult = mz_uncompress(decompressedData, &decompressedSizeOut,
                                       compressedData, compressedSize);

  if (decompressResult != Z_OK) {
    throw(std::runtime_error("Decompression failed."));
  }

  return decompressedSizeOut;
}
int main() {
  dfs(0, 0);
  cout << visible_to_inner.size() << endl;
  CalculateProbability();
  freopen("tmp/data.txt", "w", stdout);
  string raw = base64_encode(buf, bcnt, false);
  cout << raw << endl;
  freopen("tmp/compressed.txt", "w", stdout);
  // buf is the raw data;
  // buf2 is the compressed data;
  // compressed is the base64-encoded form of buf2;
  // buf3 is the base64_decoded form of compressed,it should be the same as buf2;
  // buf4 is the decompressed data, it should be the same as buf;
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