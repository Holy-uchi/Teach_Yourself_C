## 1本目：`mallocして返す` 基本

```c
int make_copy(const char *s, char **out);
```

**契約を言語化して、正しい呼び出し側コードを書け。**

* 成功時：`*out` に新規領域
* 失敗時：どうなるべき？

---

## 2本目：失敗時に out を触らない契約

```c
int read_line(FILE *fp, char **out_line); // 0 success
```

**契約**：失敗時は `*out_line` を変更しない。
`char *line = NULL;` から始めて、free事故が起きない呼び出し例を書け。

---

## 3本目：失敗時に out を NULL にする契約

```c
int get_token(const char *s, char **out_tok); // 0 success
/* 契約: 失敗時は *out_tok = NULL を保証する */
```

**Q**：呼び出し側で「安全に free できる形」にする書き方は？

---

## 4本目：`lenも返す`（バイナリ向け）

```c
int load_all(const char *path, unsigned char **out_buf, size_t *out_len);
```

**Q**：成功したら何を free する？
失敗したら何を free する？
呼び出し側コードで示せ。

---

## 5本目：2段階API（必要サイズ問い合わせ）

```c
int format_msg(char *buf, size_t cap, size_t *out_needed);
```

**契約**：

* `buf == NULL` のときはサイズ問い合わせのみ（成功扱い）
* `cap < needed` なら `ENOBUFS`

**Q**：`malloc` して1回で成功させる呼び出し側を書け。

---

## 6本目：`realloc` を含む out 契約

```c
int append_str(char **io_buf, size_t *io_cap, const char *s);
```

**契約**：

* `*io_buf` は NULL でもよい（初回確保）
* 成功時：必要なら拡張して `*io_buf` が更新される
* 失敗時：`*io_buf` は元のまま（リークしない）

**Q**：呼び出し側の初期化と、最後の解放をどう書く？

---

## 7本目：オブジェクト + destructor 契約

```c
typedef struct { char *p; size_t n; } Blob;
int blob_from_str(const char *s, Blob *out);
void blob_destroy(Blob *b);
```

**Q**：呼び出し側で `Blob b;` を作って安全に扱うコードを書け。
（成功/失敗どちらでも destructor を呼べる形に）

---

## 8本目：`返したポインタは借用`（freeしてはいけない）

```c
const char* config_get(const Config *cfg, const char *key);
```

**契約**：返るポインタは cfg 内部の借用。呼び出し側は free 不可。
**Q**：`make_copy` を使って「自分所有」に変換する呼び出し側例を書け。

---

## 9本目：`呼び出し側が用意したバッファ`（書き込み契約）

```c
int hex_encode(const unsigned char *in, size_t in_len,
               char *out, size_t out_cap, size_t *out_written);
```

**契約**：

* outは呼び出し側が所有
* `out_cap` が足りなければ `ENOBUFS`、`out_written` は必要サイズ

**Q**：2回呼び出しで安全に実行するコードを書け。

---

## 10本目：所有権移譲（move）契約

```c
void take_ownership(char **io_p);
/* 契約: *io_p を受け取って内部で管理し、呼び出し側の *io_p は NULL にする */
```

**Q**：ダブルfreeを防ぐ呼び出し側の書き方は？
（before/after が分かるように）

---

# おまけ：採点基準（超やさしめ）

各問でこれが書けてれば満点：

* 成功時に free すべき対象が合ってる
* 失敗時に free してはいけないものを free してない
* out / io 引数の初期化（NULLや0）ができている
* 「借用」か「所有」かの区別ができている

