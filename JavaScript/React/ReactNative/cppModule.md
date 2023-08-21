# React-Native でC++ モジュールを使ってみる

[C++ Turbo Native Modules](https://reactnative.dev/docs/the-new-architecture/cxx-cxxturbomodules)
> This guide shows you how to implement a Turbo Native Module using C++ only, a way to share the same implementation with any supported platform (Android, iOS, macOS or Windows).

上記より、React-Nativeは一応C++で作成したモジュールを組み込むことができるらしい。。(まだ、実験的なモジュールで`0.68.0`からの機能らしい)
> 対象OS: Android,iOS,macOS,Windows

```bash
# iOSでよく使うので覚えておくこと
RCT_NEW_ARCH_ENABLED=1 bundle exec pod install
```

## まずはC++モジュールを追加用のお試しプロジェクトを作成

```bash
npx react-native@latest init <プロジェクト名>

# from `ios` directory
cd ./<プロジェクト名>/ios
bundle install ;
RCT_NEW_ARCH_ENABLED=1 bundle exec pod install

# Create a tm folder inside the project. 
# It will contain all C++ TurboModules of your application
cd ../
mkdir tm

# The final result should look like this:
CxxTurboModulesGuide
├── android
├── ios
├── js
└── tm
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

[1. Define the JavaScript specification.](#1)  
[2. Configure Codegen to generate the scaffolding.](#2)  
[3. Register the native module.](#3)  
[4. Write the native code to finish implementing the module.](#4)

### <a name=1>1. Define the JavaScript specification.</a>

Create the following spec inside the tm folder:
Typescriptの方がわかりやすいからJavascriptで作らない方がいいかも

```ts
// @tm folder NativeSampleModule.ts
import type {TurboModule} from 'react-native/Libraries/TurboModule/RCTExport';
// import type {TurboModule} from 'react-native'; in future versions
import {TurboModuleRegistry} from 'react-native';

export interface Spec extends TurboModule {
  readonly reverseString: (input: string) => string;
}

export default TurboModuleRegistry.getEnforcing<Spec>(
  'NativeSampleModule',
);
```

```js
// @tm　folder NativeSampleModule.js
// @flow
import type {TurboModule} from 'react-native/Libraries/TurboModule/RCTExport';
// import type {TurboModule} from 'react-native'; in future versions
import {TurboModuleRegistry} from 'react-native';

export interface Spec extends TurboModule {
  +reverseString: (input: string) => string;
}

export default (TurboModuleRegistry.getEnforcing<Spec>(
  'NativeSampleModule',
): Spec);
```

---

### <a name=2>2. Configure Codegen to generate the scaffolding.</a>

1. `package.json`更新

    ```json
    {
    // ...
    // "description": "React Native with Cxx Turbo Native Modules",
    // "author": "<Your Name> <your_email@your_provider.com> (https://github.com/<your_github_handle>)",
    // "license": "MIT",
    // "homepage": "https://github.com/<your_github_handle>/#readme",
    // ...
    // specファイルの読み込み先が分かればいいので、tm フォルダのパスを通せばOK
    "codegenConfig": {
        "name": "AppSpecs",
        "type": "all",
        "jsSrcsDir": "tm",
        "android": {
        "javaPackageName": "com.facebook.fbreact.specs"
        }
    }
    }
    ```

2. `podspec`file作成(iOSの場合)

    2.1 `AppTurboModules.podspec`ファイルを`tm`フォルダに追加

    ```podspec
    require "json"

    # 無理して package.jsonから読まんでもいい
    # package = JSON.parse(File.read(File.join(__dir__, "../../../../package.json")))

    Pod::Spec.new do |s|
      s.name            = "AppTurboModules"
      s.version         = "0.0.1"
      #s.summary         = package["description"]
      #s.description     = package["description"]
      #s.homepage        = package["homepage"]
      #s.license         = package["license"]
      s.platforms       = { :ios => "12.4" }
      s.author          = "Anonymous"
      #s.source          = { :git => package["repository"], :tag => "#{s.version}" }
      s.source_files    = "**/*.{h,cpp}"
      s.pod_target_xcconfig = {
        "CLANG_CXX_LANGUAGE_STANDARD" => "c++17"
      }
      install_modules_dependencies(s)
    end    
    ```

    2.2 ios/Podfileに依存関係を追記
        `use_react_native!(...)`ってとこ

    ```Podfile
    if ENV['RCT_NEW_ARCH_ENABLED'] == '1'
      # ここもパスが通っていればOK
      pod 'AppTurboModules', :path => "./../tm"
    end
    ```

3. Androidの場合
    後日書く

---

### <a name=3>3. Register the native module.</a>

1. iOSの場合
    1.1 `ios/CxxTurboModulesGuide/AppDelegate.mm`を更新する
        (`NativeSampleModule`の名前に合わせて更新すること)

    ```objective-c
    #import "AppDelegate.h"

    #import <React/RCTBundleURLProvider.h>
    + #import <React/CoreModulesPlugins.h>
    + #import <ReactCommon/RCTTurboModuleManager.h>
    + #import <NativeSampleModule.h>

    + @interface AppDelegate () <RCTTurboModuleManagerDelegate> {}
    + @end

    // ...

    ᠆ (Class)getModuleClassFromName:(const char *)name
    {
    return RCTCoreModulesClassProvider(name);
    }

    + #pragma mark RCTTurboModuleManagerDelegate

    + - (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:(const std::string &)name
    +                                                       jsInvoker:(std::shared_ptr<facebook::react::CallInvoker>)jsInvoker
    + {
    +   if (name == "NativeSampleModule") {
    +     return std::make_shared<facebook::react::NativeSampleModule>(jsInvoker);
    +   }
    +   return nullptr;
    + }
    ```

    1.1 Codegenを実行する(🚨JavaScript spec. を変更するたびに実行すること)

    ```bash
    RCT_NEW_ARCH_ENABLED=1 bundle exec pod install
    ```

2. Androidの場合

後日書く

---

### <a name=4>4. Write the native code to finish implementing the module.</a>

* `tm`フォルダで

`NativeSampleModule.h`、
`NativeSampleModule.cpp`を作成し、以下を実行する。
> RCT_NEW_ARCH_ENABLED=1 bundle exec pod install

```h
//NativeSampleModule.h
#pragma once

#if __has_include(<React-Codegen/AppSpecsJSI.h>) // CocoaPod headers on Apple
#include <React-Codegen/AppSpecsJSI.h>
#elif __has_include("AppSpecsJSI.h") // CMake headers on Android
#include "AppSpecsJSI.h"
#endif
#include <memory>
#include <string>

namespace facebook::react {

class NativeSampleModule : public NativeSampleModuleCxxSpec<NativeSampleModule> {
 public:
  NativeSampleModule(std::shared_ptr<CallInvoker> jsInvoker);

  std::string reverseString(jsi::Runtime& rt, std::string input);
};

} // namespace facebook::react
```

`NativeSampleModule.cpp`を作成する

```cpp
// NativeSampleModule.cpp
#include "NativeSampleModule.h"

namespace facebook::react {

NativeSampleModule::NativeSampleModule(std::shared_ptr<CallInvoker> jsInvoker)
    : NativeSampleModuleCxxSpec(std::move(jsInvoker)) {}

std::string NativeSampleModule::reverseString(jsi::Runtime& rt, std::string input) {
  return std::string(input.rbegin(), input.rend());
}

} // namespace facebook::react
```

* ファイル作成後、以下を実行

> RCT_NEW_ARCH_ENABLED=1 bundle exec pod install

ざっくり、Android,iOSでこんな感じになる。

```text
CxxTurboModulesGuide
├── android
│   └── app
│       │── src
│       │   └── main
│       │       └── jni
│       │           ├── CMakeLists.txt
│       │           └── OnLoad.cpp
│       └── build.gradle (updated)
├── ios
│   └── CxxTurboModulesGuide
│       └── AppDelegate.mm (updated)
├── js
│   └── App.tsx|jsx (updated)
└── tm
    ├── CMakeLists.txt
    ├── NativeSampleModule.h
    ├── NativeSampleModule.cpp
    ├── NativeSampleModule.ts|js
    └── TurboModules.podspec
```

App.tsx,jsxを更新して`npm start`

```ts
//...
import {
  Colors,
  DebugInstructions,
  Header,
  LearnMoreLinks,
  ReloadInstructions,
} from 'react-native/Libraries/NewAppScreen';
// importして
+ import NativeSampleModule from './tm/NativeSampleModule';
//...
        <View
          style={{
            backgroundColor: isDarkMode ? Colors.black : Colors.white,
          }}>
+         <Section title="Cxx TurboModule">
+          NativeSampleModule.reverseString(...) ={' '}
            // 実行してるだけ
+          {NativeSampleModule.reverseString(
+            'the quick brown fox jumps over the lazy dog'
+          )}
+         </Section>
          <Section title="Step One">
            Edit <Text style={styles.highlight}>App.tsx</Text> to change this
            screen and then come back to see your edits.
          </Section>
//...
```
