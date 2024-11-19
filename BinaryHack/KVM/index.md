# KVMについて

- KVM(Kernel-based Virtual Machine)はLinuxにおいて仮想技術を使うためのカーネルモジュール

## KVM1を使ってハイパーバイザを作成する

- ハイパーバイザとは
    `仮想マシン(VM)`を作成/実行/制御するための機能を提供するソフトウェア,ファームウェアのこと
    Type1,Type2の2種類に分けられ、

  - Type1:`ハードウェア上で直接動作する(Xen,Hyper-V,BitVisorなど)`
  - Type2:`他のOS上で動作する(VirtualBoxなど)`

## KVMのAPIについて

KVMは`/dev/kvm`に対して`ioctl`でAPIを呼び出すことができる。
`KVM_CREATE_VM`でVMを作成し、`vCPU`を作成したりVMの設定後、
`KVM_RUN`でVMを実行する流れとなる。
`KVM_RUN`が実行されるとVMでプログラムが動作している間は`KVM_RUN`でブロックされる。
VMで処理できない命令(センシティブ命令(IO命令/マップドIO等))が来たタイミングで`vmexit`が発生し、処理がホストに戻る。
まとめると以下

1. VMを作成する。(`KVM_CREATE_VM`でVMを作成する)
2. VMのvCPU,メモリ等の設定をする
3. メモリに動作させるバイナリを読み込ませる
4. `KVM_RUN`でVMを実行させる

|KVM API名(代表例)|概要|
|:--|:--|
|KVM_GET_API_VERSION|KVMのAPIバージョンを取得する|
|KVM_CREATE_VM|VMを作成する|
|KVM_SET_USER_MEMORY_REGION|VMのメモリを設定する|
|KVM_CREATE_VCPU|`vCPU`を作成する|
|KVM_GET_VCPU_MMAP_SIZE|`KVM_RUN`で使用する`vCPU`毎のメモリサイズを取得する|
|KVM_GET_REGS/KVM_SET_REGS|VMの汎用レジスタの値を設定/取得する|
|KVM_GET_SREGS/KVM_SET_SREGS|VMのセグメントレジスタの値を設定/取得する|
|KVM_RUN|VMを実行する。vmexit(センシティブ命令)が起こるまでブロック|

|exit_reason|概要|
|:--|:--|
|KVM_EXIT_IO|ゲストがI/O命令を実行した|
|KVM_EXIT_MMIO|ゲストがメモリマップドI/O命令を実行した|
|KVM_EXIT_HLT|ゲストがHLT命令を実行した|
|KVM_EXIT_INTERNAL_ERROR|VM実行中にエラーが発生した|

```cpp
int dev = open("/dev/kvm",O_RDWR);
if (dev < 0) 
{
  perror("open failed");
  return 1;
}

// KVM API versionの取得
long kvm_ver = ioctl(dev,KVM_GET_API_VERSION);
printf("KVM API VERSION: %ld\n",kvm_ver);
```
