
# CDialogモックメモ

```cpp
#ifdef _DEBUG
#define CDialog CDialogMock

class CDialogMock {
public:
void Create(int rId, void*) {};
void ShowWindow(int sw_cmd) {};
};
#endif
```

---

# DLL export/importマクロ

```cpp
#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif
```

---

# イベントのハンドリング,メッセージマップ関連

```cpp
BEGIN_MESSAGE_MAP(SubWind, CDialogEx)
 ON_WM_SYSCOMMAND()
 ON_WM_PAINT()
 ON_WM_QUERYDRAGICON()
 ON_BN_CLICKED(IDC_BUTTON1, &SubWind::OnBnClickedButton1)
 // ↓こんな感じidの範囲を指定してイベントを受け取れる。
 //    ついでにどのidかは引数で受け取ることができる。
 ON_CONTROL_RANGE(BN_CLICKED, 500, 550 + off, &myButton::testClicked)
 // ↓SendMessageはこんな感じで受け取ることができる。
 ON_MESSAGE(550, &myButton::testClicked2)
END_MESSAGE_MAP()
```
