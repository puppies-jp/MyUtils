
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
