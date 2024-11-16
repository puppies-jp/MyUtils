# KVMについて

- KVM(Kernel-based Virtual Machine)はLinuxにおいて仮想技術を使うためのカーネルモジュール

## KVM1を使ってハイパーバイザを作成する

- ハイパーバイザとは
    `仮想マシン(VM)`を作成/実行/制御するための機能を提供するソフトウェア,ファームウェアのこと
    Type1,Type2の2種類に分けられ、

  - Type1:`ハードウェア上で直接動作する(Xen,Hyper-V,BitVisorなど)`
  - Type2:`他のOS上で動作する(VirtualBoxなど)`
