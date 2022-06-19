
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
# DLL export/importマクロ

```cpp
#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif
```
