# C IO 定石ドリル集

## 🎯 目的

- `fopen / fclose`
- `fgetc / fgets`
- `fprintf / fputc / fputs`
- `ferror`
- エラーハンドリングの型

を **反射で書けるようにする**。

---

## 🔒 使用可能関数（縛り）

- `fopen`
- `fclose`
- `fgetc`
- `fgets`
- `fprintf`
- `fputc`
- `fputs`
- `perror` or `strerror(errno)`
- `ferror`

※ `fseek` はまだ使わない

---

# 🟢 Level 1：超基礎（読むだけ）

### 1. ファイルをそのまま表示

- `input.txt` を開いて内容をそのまま標準出力へ出力せよ。

---

### 2. 行数を数えよ

- `fgets` を使うこと。
- `wc -l` と同じ挙動でもよい。
- （余力）末尾改行なしも正しく数えよ。

---

### 3. バイト数を数えよ

- `fgetc` を使う。
- `wc -c` と一致させる。

---

### 4. 行番号を付けて表示

出力例：

```
1: hello
2: world
```

- `fgets`
- 行末改行なしケースにも対応できると尚良い。

---

### 5. 空行の数を数えよ

- `fgets`
- 空行判定ロジックを自分で考える

---

# 🟡 Level 2：軽い加工

### 6. すべて大文字にして表示

- `toupper`
- `fgetc` で1文字ずつ処理

---

### 7. 特定の文字を置換

例：

- `'a'` を `'*'` に置き換える

---

### 8. 10文字以上の行だけ表示

- `strlen`
- `fgets`

---

### 9. 改行を削除して表示

- 行末 `\n` を除去する処理を書く

---

### 10. 逆順で表示

- 一度配列に格納してから出力

---

# 🔵 Level 3：書き込み

### 11. ファイルコピー

- `input.txt → output.txt`

---

### 12. 行番号付きで保存

---

### 13. 偶数行だけ保存

---

### 14. 行数とバイト数を result.txt に保存

---

### 15. 大文字化した結果を保存

---

# 🧠 定石テンプレ

## 開く

```c
FILE *fp = fopen("file.txt", "rb");
if (!fp) { perror("fopen"); return 1; }
```

## 1文字ループ

```c
int ch;
while ((ch = fgetc(fp)) != EOF) {
}
if (ferror(fp)) { perror("fgetc"); }
```

## 1行ループ

```c
char buf[256];
while (fgets(buf, sizeof buf, fp)) {
}
if (ferror(fp)) { perror("fgets"); }
```

## 閉じる

```c
if (fclose(fp) != 0) { perror("fclose"); }
```
