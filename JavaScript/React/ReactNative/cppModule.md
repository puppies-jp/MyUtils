# React-Native でC++ モジュールを使ってみる

[C++ Turbo Native Modules](https://reactnative.dev/docs/the-new-architecture/cxx-cxxturbomodules)
> This guide shows you how to implement a Turbo Native Module using C++ only, a way to share the same implementation with any supported platform (Android, iOS, macOS or Windows).

上記より、React-Nativeは一応C++で作成したモジュールを組み込むことができるらしい。。(まだ、実験的なモジュールで`0.68.0`からの機能らしい)
> 対象OS: Android,iOS,macOS,Windows

## まずはC++モジュールを追加用のお試しプロジェクトを作成

```bash
npx react-native@latest init <プロジェクト名>

# from `ios` directory
cd ./<プロジェクト名>/ios
bundle install && RCT_NEW_ARCH_ENABLED=1 bundle exec pod install
```

---
---

## 作成手順

```md
To create a C++ Turbo Native Module, you need to:
    1. Define the JavaScript specification.
    2. Configure Codegen to generate the scaffolding.
    3. Register the native module.
    4. Write the native code to finish implementing the module.
```
